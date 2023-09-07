#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "string_builder.h"

void *
StringBuilder_malloc(size_t size)
{
    void *p = malloc(size);
    if (p == NULL) {
        fprintf(stderr, "StringBuilder: OUT OF MEMORY\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

StringBuilder *
StringBuilder_new(void)
{
    StringBuilder *sb = StringBuilder_malloc(sizeof(StringBuilder));
    sb->size = 1;
    sb->cap = INITIAL_CAPACITY;

    char *s = StringBuilder_malloc(sizeof(char) * INITIAL_CAPACITY);
    s[0] = '\0';
    sb->s = s;

    return sb;
}

void
StringBuilder_free(StringBuilder *sb)
{
    free(sb->s);
    free(sb);
}

void
StringBuilder_resize(StringBuilder *sb, size_t capacity)
{
    char *s = StringBuilder_malloc(sizeof(char) * capacity);
    strncpy(s, sb->s, sb->size);
    free(sb->s);
    sb->s = s;
    sb->cap = capacity;
}

char *
StringBuilder_str(StringBuilder *sb)
{
    return sb->s;
}

void
StringBuilder_write_char(StringBuilder *sb, char c)
{
    size_t size = sb->size;
    if (size >= sb->cap) {
        StringBuilder_resize(sb, sb->cap * 2);
    }
    sb->s[size - 1] = c;
    sb->s[size - 0] = '\0';
    sb->size++;
}

void
StringBuilder_write(StringBuilder *sb, const char *s)
{
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        StringBuilder_write_char(sb, s[i]);
    }
}

void
StringBuilder_trim(StringBuilder *sb)
{
    int start, end;
    for (start = 0; start < sb->size; start++) {
        if (!isspace(sb->s[start])) {
            break;
        }
    }
    for (end = sb->size-2; end > start; end--) {
        if (!isspace(sb->s[end])) {
            break;
        }
    }
    int size = end - start + 1;
    char *s = StringBuilder_malloc(size);
    strncpy(s, sb->s + start, size);
    s[size] = '\0';
    free(sb->s);
    sb->s = s;
    sb->cap = size;
    sb->size = size;
}
