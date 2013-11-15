/*
 Copyright (c) 2013 by XiongBo.  All Rights Reserved.
*/

#include "head/unp.h"
#include "head/svr.h"
#include "head/log.h"
#include "head/conf.h"
#include "evlib/include/event.h"
#include "evlib/include/evhttp.h"

#define MAX_DIR_LENGTH 50

/**
 * Callback function
 * @param req 
 * @param arg
 */ 
static void RequestHandler(struct evhttp_request *req, void *arg)
{
  
  struct evbuffer *returnbuffer = evbuffer_new();

  evbuffer_add_printf(returnbuffer, "Thanks for the request!");
  evhttp_send_reply(req, HTTP_OK, "Client", returnbuffer);
  evbuffer_free(returnbuffer);
  return;
}

/**
 * Run http server
 */
/** TODO: daemonize */
void RunServer()
{
    /* Parse configure file */
    char *http_opt_dir = (char *) malloc(sizeof(char) * (MAX_DIR_LENGTH));
    ParseCfgSeverDir(http_opt_dir);
    int http_opt_port = ParseCfgServerPort();
    char *http_opt_listen = "0.0.0.0";

    char info[50];
    sprintf(info, "Server started at port no. %d with root directory as %s", http_opt_port, http_opt_dir);
    ErrorLog("NORMAL", info);

    /* Initialize http object */
    struct evhttp *http;
    event_init();
    http = evhttp_start(http_opt_listen, http_opt_port);

    /* Set callback function */
    evhttp_set_gencb(http, RequestHandler, NULL);
    event_dispatch();
}
