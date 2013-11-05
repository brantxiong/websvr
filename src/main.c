#include "head/unp.h"
#include "head/init.h"
#include "head/conf.h"
#include "head/log.h"
#define MAX_DIR_LENGTH 50

int main(int argc, char const *argv[])
{
    char *dir = (char *) malloc(sizeof(char) * (MAX_DIR_LENGTH));
    int port;
    ReadServerDir(dir);
    port = ReadServerPort();
    printf("dir: %s\n", dir);
    printf("port: %d\n", port);
    AcessLog("127.0.0.1", "test");
    ErrorLog("WARNING", "fail to bind()");
    test();
    return 0;
}
