#ifndef _LOG_H_
#define _LOG_H_
void AccessLog(char *ip, char *first_line, char *user_agent);
void ErrorLog(char *level, char *info);

#endif
/* _LOG_H_ */
