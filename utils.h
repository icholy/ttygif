
#ifndef _UTILS_H
#define _UTILS_H

void fatalf(const char *format, ...);
char * osx_get_window_id();
char * linux_get_window_id();
int exec_with_output(const char *command);

#endif /* _UTILS_H */
