#include "head/init.h"
#include "head/log.h"

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

void LogInit()
{
    logh_t h;
    FILE *err_file;
    FILE *warning_file;

    err_file = fopen ("log/err_file", "w");
    if (err_file == NULL)
    {
        perror ("fopen(err_file)");
        exit (EXIT_FAILURE);
    }

    warning_file = fopen ("log/warning_file", "w");
    if (warning_file == NULL)
    {
        perror ("fopen(waring_file)");
        fclose (err_file);
        exit (EXIT_FAILURE);
    }

    h = log_alloc ();

    if (h == NULL)
    {
        perror ("log_alloc");
        return EXIT_FAILURE;
    }

    log_set_attributes (h,
                        LOG_TAG_PREFIX, "MY EXAMPLE",
                        LOG_TAG_OPTIONS, LOG_OPT_DATE | LOG_OPT_PREFIX | LOG_OPT_PRIORITY | LOG_OPT_TIME | LOG_OPT_USE_LOCALE |LOG_OPT_IP,
                        LOG_TAG_FILE, stdout,
                        LOG_TAG_FILE_ERR, err_file,
                        LOG_TAG_FILE_WARNING, warning_file,
                        LOG_TAG_LEVEL, LOG_PRI_WARNING,
                        LOG_TAG_DONE);

    log_write_log (h, LOG_PRI_ERR, "try of an ERR log");
    log_write_log (h, LOG_PRI_DEBUG, "try of a DEBUG log");
    log_write_log (h, LOG_PRI_WARNING, "try of a WARNING log");

    log_free (h);

    fclose (warning_file);
    fclose (err_file);

    return EXIT_SUCCESS;
}

void test()
{
    printf("hello\n");
}
