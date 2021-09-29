#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "string_builder.h"

void fatalf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    exit(EXIT_FAILURE);
}

char * osx_get_window_id()
{
    const char *term = getenv("TERM_PROGRAM");
    if (term == NULL || !strlen(term)) {
        fatalf("Error: TERM_PROGRAM environment variable was empty.");
    }
    if (strcmp(term, "Apple_Terminal") == 0) {
        term = "Terminal.app";
    }

    char command[1024];
    sprintf(command,
            "osascript -so -e 'tell app \"%s\" to id of window 1' 2> /dev/null",
            term);

    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        fatalf("Error: failed to run command: %s", command);
    }

    char *window_id = (char*) malloc(256 * sizeof(char));
    if (fscanf(fp, "%s", window_id) != 1) {
        fatalf("Error: failed to parse window id: %s", command);
    }

    pclose(fp);

    return window_id;
}

int exec_with_output(const char *command)
{
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        fatalf("Error: failed to run command: %s", command);
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }

    return pclose(fp);
}
