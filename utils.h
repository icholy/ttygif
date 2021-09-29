
#ifndef _UTILS_H
#define _UTILS_H

void fatalf(const char *format, ...);
char * osx_get_window_id(const char *app_name);
int exec_with_output(const char *command);

#endif /* _UTILS_H */
