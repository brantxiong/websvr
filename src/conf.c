#include "lib/libconfig.h"
#define CONF_FILE "conf/websvr.cfg"

void ReadCfgFile()
{
    config_t cfg;               /*Returns all parameters in this structure */
    config_setting_t *setting;
    const char *str1, *str2;
    int tmp;

    char *config_file_name = CONF_FILE;

    /*Initialization */
    config_init(&cfg);


    /* Read the file. If there is an error, report it and exit. */
    if (!config_read_file(&cfg, config_file_name))
    {
        printf("\n%s:%d - %s", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        return -1;
    }

    /* Get the configuration file name. */
    if (config_lookup_string(&cfg, "filename", &str1))
        printf("\nFile Type: %s", str1);
    else
        printf("\nNo 'filename' setting in configuration file.");

    /*Read the parameter group*/
    setting = config_lookup(&cfg, "server");
    if (setting != NULL)
    {
        /*Read the string*/
        if (config_setting_lookup_string(setting, "root_dir", &str2))
        {
            printf("\nroot_dir: %s", str2);
        }
        else
            printf("\nNo 'root_dir' setting in configuration file.");

        /*Read the integer*/
        if (config_setting_lookup_int(setting, "port", &tmp))
        {
            printf("\nport: %d", tmp);
        }
        else
            printf("\nNo 'port' setting in configuration file.");

        printf("\n");
    }

    config_destroy(&cfg);
}
