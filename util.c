#include <stdio.h>
#include <string.h>

// You must free the result if result is non-NULL.
char* str_replace (char *orig, char *rep, char *with) {

    char *result;   // the return string
    char *ins;      // the next insert point
    char *tmp;      // varies
    int len_rep;    // length of rep
    int len_with;   // length of with
    int len_front;  // distance between rep and end of last rep
    int count;      // number of replacements

    if (!orig) { return NULL; }
    if (!rep)  { rep  = "";   }
    if (!with) { with = "";   }

    len_rep  = strlen(rep);
    len_with = strlen(with);

    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result) { return NULL; }

    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}
