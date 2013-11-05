#include "head/log.h"
#include "head/unp.h"

void AcessLog(char *ip, char *info)
{
    FILE *acess_log_fp = fopen("log/acess.log", "a");
    char buff[25];
    if (acess_log_fp == NULL)
    {
        printf("Open acess.log fail\n");
        exit (EXIT_FAILURE);
    }
    time_t timer;
    time(&timer);
    strftime(buff, 25, "%Y-%m-%d %H:%M:%S", localtime (&timer));
    fprintf(acess_log_fp, "%s  - - [%s]  \"%s\"\n", ip, buff, info);
    fclose(acess_log_fp);
}

void ErrorLog(char *level, char *info)
{
    FILE *error_log_fp = fopen("log/error.log", "a");
    char buff[25];
    if (error_log_fp == NULL)
    {
        printf("Open error.log fail\n");
        exit (EXIT_FAILURE);
    }
    time_t timer;
    time(&timer);
    strftime(buff, 25, "%Y-%m-%d %H:%M:%S", localtime (&timer));
    fprintf(error_log_fp, "%s [%s]:  %s\n", buff, level, info);
    fclose(error_log_fp);
}
