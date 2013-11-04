#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>


#if defined (__cplusplus)
extern "C" {
#endif /* __cplusplus */


/**
 * @defgroup LOG Log Writer Library
 * @{
 */


/**
 * @brief LOG Tags
 *
 * This is the list of available log_tag_t tags for use with log_set_attributes().
 *
 * @see log_set_attributes()
 */
typedef enum
{
    /** Special tag to end the tag list */
    LOG_TAG_DONE = 0,

    /** Tag to modify the level of a LOG handle */
    LOG_TAG_LEVEL,

    /** Tag to set the prefix of the logged messages */
    LOG_TAG_PREFIX,

    /** Tag to set options to use in a LOG handme */
    LOG_TAG_OPTIONS,

    /** Tag to set the default file used to write messages in */
    LOG_TAG_FILE,

    /** Tag to set the file for EMERG level */
    LOG_TAG_FILE_EMERG,

    /** Tag to set the file for ALERT level */
    LOG_TAG_FILE_ALERT,

    /** Tag to set the file for CRIT level */
    LOG_TAG_FILE_CRIT,

    /** Tag to set the file for ERR level */
    LOG_TAG_FILE_ERR,

    /** Tag to set the file for WARNING level */
    LOG_TAG_FILE_WARNING,

    /** Tag to set the file for NOTICE level */
    LOG_TAG_FILE_NOTICE,

    /** Tag to set the file for INFO level */
    LOG_TAG_FILE_INFO,

    /** Tag to set the file for DEBUG level */
    LOG_TAG_FILE_DEBUG

}
log_tag_t;

/**
 * @brief LOG options
 *
 * This is the list of available LOG options for use with log_set_attributes().
 * The options could be XOR-ed to use more than one option at the same time.
 *
 * Example: LOG_OPT_ADR | LOG_OPT_PID | LOG_OPT_DATE | LOG_OPT_TIME means
 *          you want to use all these options for each logged message.
 *
 * @see log_set_attributes()
 */
typedef enum
{
    /** No option (default) */
    LOG_OPT_NONE=0,

    /** The message is also logged to stderr */
    LOG_OPT_PERROR=1,

    /** Write the pid of the process */
    LOG_OPT_PID=2,

    /** Write the date */
    LOG_OPT_DATE=4,

    /** Write the time */
    LOG_OPT_TIME=8,

    /** Write host's IP address */
    LOG_OPT_IP=16,

    /** Write host's FQDN */
    LOG_OPT_ADR=32,

    /** Write the LOG_PRIORITY of the message */
    LOG_OPT_PRIORITY=64,

    /** Write prefix specified by LOG_TAG_PREFIX */
    LOG_OPT_PREFIX=128,

    /** Don't flush the file after log writing */
    LOG_OPT_NO_FLUSH=256,

    /** Use locale environement settings to display date and time */
    LOG_OPT_USE_LOCALE=512

} log_option_t;

/**
 * @brief LOG priority
 *
 * This is the list of available LOG priorities for use with log_write_log()
 *
 * @see log_write_log()
 */
typedef enum
{
    /** EMERGENCY level */
    LOG_PRI_EMERG=0,

    /** ALERT level */
    LOG_PRI_ALERT=1,

    /** CRITICAL level */
    LOG_PRI_CRIT=2,

    /** ERROR level */
    LOG_PRI_ERR=3,

    /** WARNING level */
    LOG_PRI_WARNING=4,

    /** NOTICE level */
    LOG_PRI_NOTICE=5,

    /** INFORMATION level */
    LOG_PRI_INFO=6,

    /** DEBUG level */
    LOG_PRI_DEBUG=7

} log_priority_t;

/**
 * @brief LOG handle type.
 *
 * This type is voluntary opaque. Variables of this kind could'nt be directly
 * used, but by the functions of this module.
 */
typedef struct _log_handle *logh_t;

/**
 * @brief Create a new LOG handle.
 *
 * Allocate a new LOG handle data structure.
 * The intial log file used is stdout and the initial level is LOG_PRI_DEBUG.
 * To change them and other attributes, uses log_set_attributes() function.
 *
 * Default values are:
 * - LOG_PRI_DEBUG for LOG_TAG_LEVEL
 * - LOG_OPT_NONE for LOG_TAG_OPTIONS
 * - NULL for LOG_TAG_PREFIX
 * - stdout for LOG_TAG_FILE
 * - NULL for all LOG_TAG_FILE_xxx
 *
 * @pre nothing
 * @return the newly allocated LOG handle in case of success.
 * @return NULL in case of insufficient memory.
 * @see log_free()
 */
extern logh_t
log_alloc (void);

/**
 * @brief Destroy a LOG handle.
 *
 * Deallocate and destroy the log HANDLE.
 *
 * @pre HANDLE must be a valid logh_t.
 * @param HANDLE The log handle to deallocate.
 * @see log_alloc()
 */
extern void
log_free (logh_t HANDLE
         );

/**
 * @brief Log a message to the files associated to a LOG handle.
 *
 * Write the given formated message of priority PRI to the HANDLE's files.
 * To change the HANDLE's files, or log level, you can use log_set_attributes()
 * function. If no files were specified by LOG_TAG_FILE_xxx tags, then all logs
 * are written to the file specified by LOG_TAG_FILE (or to the default one if
 * no LOG_TAG_FILE was specified). If a file was specified for a given level, by
 * use of tags like LOG_TAG_FILE_xxx, then the specified file is used instead
 * of the default one.
 * The message FORMAT must be used like printf().
 * The message will be logged only if PRI is <= HANDLE's log level (wich you
 * can consult with log_get_log_level()).
 *
 * Example:
 *
 * log_write_log (my_handle,
 *                LOG_PRI_DEBUG,
 *                "%s:%d-%s ERROR",
 *                __FILE__, __LINE__, __FUNCTION__);
 *
 * @pre HANDLE must be a valid logh_t.
 * @param HANDLE The LOG handle to use.
 * @param PRI The priority of the message to log.
 * @param FORMAT The message to log.
 * @param ... Variables to include into the message.
 * @see log_get_log_level
 * @see log_set_attributes
 */
extern void
log_write_log (logh_t HANDLE,
               log_priority_t PRI,
               char *FORMAT,
               ...
              );

/**
 * @brief Modify the attributes of a LOG handle.
 *
 * To use this function, you must use LOG_TAGs to specify each tag you want
 * to set. The list of tags is fully determinated by the log_tag_t type.
 * Each tag must be followed by its new value, and the tag list must be
 * terminated by LOG_TAG_DONE. If the tag list is not terminated by LOG_TAG_DONE
 * the result of this function is not determined.
 *
 * Example:
 *
 * log_set_attributes (my_handle,
 *                     LOG_TAG_PREFIX, "my example app",
 *                     LOG_TAG_FILE, stderr,
 *                     LOG_TAG_OPTIONS, LOG_OPT_PRIORITY | LOG_OPT_DATE,
 *                     LOG_TAG_DONE);
 *
 * @pre HANDLE must be a valid logh_t.
 * @param HANDLE The LOG handle to modify.
 * @param TAGS, ... A coma separated list of (LOG_TAG_xxx, value) couples,
 * terminated by the special LOG_TAG_DONE log_tag_t.
 * @return 0 in case of success.
 * @return -1 in case of failure.
 * @see log_tag_t
 */
extern int
log_set_attributes (logh_t HANDLE,
                    log_tag_t TAGS,
                    ...
                   );

/**
 * @brief Get the actual log level of a LOG handle.
 * @pre HANDLE must be a valid logh_t.
 * @return the actual log level of HANDLE.
 * @param HANDLE The LOG handle to use.
 */
extern log_priority_t
log_get_log_level (const logh_t HANDLE
                  );

/**
 * @brief Get the actual default file where a LOG handle is logging into.
 * @pre HANDLE must be a valid logh_t.
 * @param HANDLE The LOG handle to use.
 * @return the default file where HANDLE is actually logging.
 */
extern FILE*
log_get_default_log_file (const logh_t HANDLE
                         );

/**
 * @brief Alias for log_get_default_log_file()
 * @see log_get_default_log_file()
 */
#define log_get_log_file(h) log_get_default_log_file(h)

/**
 * @brief Get the hostname of a LOG handle.
 * @pre HANDLE must be a valid logh_t.
 * @param HANDLE The LOG handle to use.
 * @return the hostname if HANDLE's options includes LOG_OPT_IP and/or
 * LOG_OPT_ADR.
 * @return NULL if HANDLE's options does not include LOG_OPT_IP and/or
 * LOG_OPT_ADR.
 */
extern const char*
log_get_hostname (logh_t HANDLE
                 );


/*
 * @}
 */


#ifdef __cplusplus
}
#endif/* __cplusplus */


#endif /* _LOG_H_ */
