#include "head/unp.h"
#include "head/init.h"
#include "head/conf.h"
#include "head/svr.h"
#define MAX_DIR_LENGTH 50

int main(int argc, char const *argv[])
{
    char *dir = (char *) malloc(sizeof(char) * (MAX_DIR_LENGTH));
    ParseCfgSeverDir(dir);
    StartServer(dir, ParseCfgServerPort());
    return 0;
}
