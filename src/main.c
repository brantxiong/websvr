#include "head/unp.h"
#include "head/init.h"
#include "head/conf.h"

int main(int argc, char const *argv[])
{
    ReadCfgFile();
    LogInit();
    test();
    return 0;
}
