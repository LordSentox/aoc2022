#ifndef DYN_STR_H
#define DYN_STR_H

#include <stdbool.h>
#include <stdio.h>

enum DynStrRes
{
    STR_SUCCESS,
    STR_ERR_ALLOC,
    STR_ERR_READ,
    STR_NO_MATCH,
};

struct DynStr
{
    char *data;
    // Length, not including the trailing \0
    size_t len;
};

void str_init_empty(struct DynStr *str);
void str_deinit(struct DynStr *str);

enum DynStrRes str_from_file(FILE *file, struct DynStr *string);
// Create string from char array. The length given should be the length without
// the trailing \0, if there is any.
enum DynStrRes str_from_chars(struct DynStr *string, char *src, size_t len);

// Take src and create two new strings, which will be split at the first
// occurence of the character delim. After this src must not be used
// anymore. The delimiter will not be part of any of the two strings
// anymore. Returns false if the delimiter is not found. The second string
// will be empty, while the first string will be the same as src.
enum DynStrRes str_split_next(struct DynStr src, struct DynStr *s1,
                              struct DynStr *s2, const char delim);

const char *str_inner(struct DynStr *string);

bool str_equals(const struct DynStr *s1, const struct DynStr *s2);
bool str_equals_chars(const struct DynStr *s1, const char *s2);

#endif /* DYN_STR_H */
