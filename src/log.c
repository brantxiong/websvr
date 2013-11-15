/*
 Copyright (c) 2013 by XiongBo.  All Rights Reserved.
*/

#include "head/log.h"
#include "head/unp.h"

/**
 * Record access log
 * @param ip   user ip
 * @param info request information
 */
void AccessLog(char *ip, char *info)
{
    FILE *access_log_fp = fopen("log/access.log", "a");
    char buff[25];
    if (access_log_fp == NULL)
    {
        printf("Open access.log fail\n");
        exit (EXIT_FAILURE);
    }
    time_t timer;
    time(&timer);
    strftime(buff, 25, "%Y-%m-%d %H:%M:%S", localtime (&timer));
    fprintf(access_log_fp, "%s  - - [%s]  \"%s\"\n", ip, buff, info);
    fclose(access_log_fp);
}

/**
 * Record error or warning log
 * @param level 
 * @param info 
 */
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
