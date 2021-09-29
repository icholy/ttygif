#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

typedef struct {
    size_t size;
    size_t cap;
    char *s;
} StringBuilder;

#define INITIAL_CAPACITY 256

StringBuilder * StringBuilder_new(void);
void StringBuilder_free(StringBuilder *sb);
char * StringBuilder_str(StringBuilder *sb);
void StringBuilder_write_char(StringBuilder *sb, char c);
void StringBuilder_write(StringBuilder *sb, const char *s);
void StringBuilder_trim(StringBuilder *sb);

#endif /* STRING_BUILDER_H */

