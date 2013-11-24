/*
 Copyright (c) 2013 by XiongBo.  All Rights Reserved.
*/

#include "head/unp.h"
#include "head/svr.h"
#include "head/log.h"
#include "head/conf.h"

#include "evlib/include/event.h"
#include "evlib/include/evhttp.h"
#include "evlib/include/http-internal.h"

#define MAX_DIR_LENGTH 50

/**
 *
 * @param evb
 * @param decode_path uri path
 * @param whole_path  whole path
 * @return
 */
static int SendDirectoryList(struct evbuffer *evb, char *decode_path, char *whole_path)
{
    int dir, i;
    struct dirent **name_list;

    const char *trailing_slash = "";

    if (!strlen(decode_path) || decode_path[strlen(decode_path)-1] != '/')
        trailing_slash = "/";

    dir = scandir(whole_path, &name_list, 0, alphasort);

    if (dir < 0)
    {
        return 1;
    }

    /* Generate directory content */
    evbuffer_add_printf(evb, "<html>\n \
        <head><title>Index of %s</title></head>\n \
        <base href='%s%s'>\n \
        <body bgcolor=\"white\">\n \
        <h1>Index of %s</h1><hr><pre>",
                        decode_path,
                        decode_path, trailing_slash,
                        decode_path);

    for(i=0; i<dir; i++)
    {
        evbuffer_add_printf(evb,
                            "<a href=\"%s\">%s</a>\n",
                            name_list[i]->d_name, name_list[i]->d_name);
        free(name_list[i]);
    }
    free(name_list);

    evbuffer_add_printf(evb, "</pre><hr></body></html>");
    return 0;
}

/**
 * Send File content to event buffer
 * @param  evb
 * @param  fd
 * @return
 */
static int SendFileData(struct evbuffer *evb, char *file)
{
    char *file_contents;
    long input_file_size;

    FILE *fd = fopen(file, "r");
    fseek(fd, 0, SEEK_END);
    input_file_size = ftell(fd);
    rewind(fd);
    file_contents = malloc(input_file_size * (sizeof(char)));
    fread(file_contents, sizeof(char), input_file_size, fd);
    fclose(fd);

    evbuffer_add_printf(evb, file_contents);
    return 0;
}

/**
 * Send Error information
 * @param req
 */
static void ErrorResponseHandle(struct evhttp_request *req)
{
    evhttp_send_error(req, 404, "Document not found");
}

/**
 * Handle Response
 * @param req
 */
static int ResponseHandler(struct evhttp_request *req, char *doc_root)
{
    /* This holds the content we're sending. */
    struct evbuffer *evb = evbuffer_new();

    /* Decode the URI */
    char *uri_decoded = evhttp_decode_uri(req->uri);
    /* Get whole path */
    char *decode_path = uri_decoded;
    int len = strlen(doc_root)+strlen(decode_path)+2;
    char *whole_path = malloc(len);

    sprintf(whole_path, "%s/%s", doc_root, decode_path);
    char *file = malloc(len+10);
    sprintf(file, "%s/index.html", whole_path);

    int fd;
    if( (fd=open(file, O_RDONLY))!= -1 )
    {
        if(SendFileData(evb, file) != 0)
        {
            ErrorResponseHandle(req);
            return 1;
        }
    }
    else
    {
        if(SendDirectoryList(evb, decode_path, whole_path) != 0)
        {
            ErrorResponseHandle(req);
            return 1;
        }
    }

    evhttp_add_header(req->output_headers, "Content-Tpe", "text/html");

    evhttp_send_reply(req, 200, "OK", evb);

    evbuffer_free(evb);
    return 0;
}

/**
 * Callback function
 * @param req
 * @param arg
 */
static void RequestHandler(struct evhttp_request *req, void *arg)
{
    struct evhttp_connection *evcon = req->evcon;
    struct evbuffer *conn_inpuf_buf = evcon->input_buffer;

    /* Record User request and agent to log */
    // libevent bug EVBUFFER_LENGTH(x)	(x)->len
    conn_inpuf_buf->off = conn_inpuf_buf->totallen;
    char *first_line = evbuffer_readline(conn_inpuf_buf);
    char *user_agent = evhttp_find_header(req->input_headers, "User-Agent");
    AccessLog(req->remote_host, first_line, user_agent);

    ResponseHandler(req, arg);
    return;
    // evhttp_connection_free(req->evcon);
    //evhttp_request_free(req);
}

/**
 * Run http server
 */
void RunServer()
{
    /* Parse configure file */
    char *http_opt_dir = (char *) malloc(sizeof(char) * (MAX_DIR_LENGTH));
    ParseCfgSeverDir(http_opt_dir);
    int http_opt_port = ParseCfgServerPort();
    char *http_opt_addr = "0.0.0.0";

    char info[100];
    sprintf(info, "Server started at port no. %d with root directory as %s", http_opt_port, http_opt_dir);
    ErrorLog("NORMAL", info);

    /* Initialize http object */
    struct event_base *base;
    struct evhttp *http;

    base = event_init();
    if (!base)
    {
        ErrorLog("FATAL", "Couldn't create an event_base: exiting");
        exit(EXIT_FAILURE);
    }

    /* Create a new evhttp object to handle requests. */
    http = evhttp_new(base);
    if (!http)
    {
        ErrorLog("FATAL", "Couldn't create evhttp: exiting");
        exit(EXIT_FAILURE);
    }
    /* Binds an HTTP server on the specified address and port.*/
    if (evhttp_bind_socket(http, http_opt_addr, http_opt_port) != 0)
    {
        ErrorLog("FATAL", "Couldn't bind http server: exiting");
        exit(EXIT_FAILURE);
    }

    /* Set callback function */
    evhttp_set_gencb(http, RequestHandler, http_opt_dir);

    /* loop to handle request */
    event_base_dispatch(base);
}
