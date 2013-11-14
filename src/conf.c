#include "cfglib/include/libconfig.h"
#include "head/unp.h"

#define CONF_FILE "conf/websvr.cfg"

void InitCfgStruct(config_t *cfg)
{
    char *config_file_name = CONF_FILE;

    /*Initialization */
    config_init(cfg);

    /* Read the file. If there is an error, report it and exit. */
    if (!config_read_file(cfg, config_file_name))
    {
        printf("\n%s:%d - %s", config_error_file(cfg), config_error_line(cfg), config_error_text(cfg));
        config_destroy(cfg);
        exit(EXIT_FAILURE);
    }
}

void ParseCfgSeverDir(char *dir)
{
    config_t cfg;
    config_setting_t *setting;
    InitCfgStruct(&cfg);
    const char *str;

    /*Read the parameter group*/
    setting = config_lookup(&cfg, "server");
    if (setting != NULL)
    {
        /*Read the string*/
        if(!config_setting_lookup_string(setting, "dir", &str))
        {
            printf("\nNo 'dir' setting in configuration file.");
            config_destroy(&cfg);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        printf("\nNo 'server' setting in configuration file.");
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }
    strcpy(dir, str);
    config_destroy(&cfg);
}

int ParseCfgServerPort()
{
    config_t cfg;
    config_setting_t *setting;
    InitCfgStruct(&cfg);
    int num;

    /*Read the parameter group*/
    setting = config_lookup(&cfg, "server");
    if (setting != NULL)
    {
        /*Read the integer*/
        if(!config_setting_lookup_int(setting, "port", &num))
        {
            printf("\nNo 'port' setting in configuration file.");
            config_destroy(&cfg);
            exit(EXIT_FAILURE);
        }

    }
    else
    {
        printf("\nNo 'server' setting in configuration file.");
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }
    config_destroy(&cfg);
    return num;
}
