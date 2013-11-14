#include "head/unp.h"
#include "head/svr.h"
#include "head/log.h"
#include "evlib/include/event.h"
#include "evlib/include/evhttp.h"

#define CONNMAX 10
#define BYTES 1024

int listenfd, clients[CONNMAX];


static void RequestHandler(struct evhttp_request *req, void *arg)
{
  struct evbuffer *returnbuffer = evbuffer_new();

  evbuffer_add_printf(returnbuffer, "Thanks for the request!");
  evhttp_send_reply(req, HTTP_OK, "Client", returnbuffer);
  evbuffer_free(returnbuffer);
  return;
}

void StartServer(char *dir, int port)
{
    struct evhttp *http_server = NULL;
    event_init();
    http_server = evhttp_start("0.0.0.0", port);
    evhttp_set_gencb(http_server, RequestHandler, NULL);
    printf("Server started at port no. %s%d%s with root directory as %s%s%s\n","\033[92m",port,"\033[0m","\033[92m",dir,"\033[0m");
    event_dispatch();
}
