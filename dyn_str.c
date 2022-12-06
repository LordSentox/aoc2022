#include "dyn_str.h"

#include <stdlib.h>
#include <string.h>

enum DynStrRes str_from_file(FILE *file, struct DynStr *string)
{
    // Go to the end of the file to determine the size we will have to allocate,
    // then return to the start for the actual read.
    size_t len;
    fseek(file, 0, SEEK_END);
    len = ftell(file);
    rewind(file);

    // Allocate memory for the string, including the trailing 0
    char *data = malloc(len + 1);
    if (data == NULL)
        return STR_ERR_ALLOC;

    // Copy the contents of the file into the string
    if (fread(data, len, 1, file) != 1) {
        free(data);
        return STR_ERR_READ;
    }

    string->data = data;
    string->data[len] = '\0';
    string->len = len;
    return STR_SUCCESS;
}

enum DynStrRes str_from_chars(struct DynStr *string, char *src, size_t len)
{
    // Create new dynamically managed char
    char *data = malloc(len + 1);
    if (data == NULL)
        return STR_ERR_ALLOC;
    string->data = data;

    // Copy the characters and place a trailing \0
    memcpy(string->data, src, len);
    string->len = len;
    string->data[len] = '\0';

    return STR_SUCCESS;
}

void str_init_empty(struct DynStr *str)
{
    // Init with only the trailing \0
    str->data = calloc(1, 1);
    str->len = 0;
}
void str_deinit(struct DynStr *str)
{
    free(str->data);
    str->data = NULL;
    str->len = 0;
}

enum DynStrRes str_split_next(struct DynStr src, struct DynStr *s1,
                              struct DynStr *s2, const char delim)
{
    char *split = strchr(src.data, delim);
    if (split == NULL) {
        s1->data = src.data;
        s1->len = src.len;
        str_init_empty(s2);
        return STR_NO_MATCH;
    }

    // Replace the delimiter character with a \0
    *split = '\0';
    s1->data = src.data;
    s1->len = split - src.data;

    // Free up space
    char *s1_data = realloc(s1->data, s1->len + 1);
    if (s1_data == NULL) {
        str_deinit(s1);
        return STR_ERR_ALLOC;
    }

    enum DynStrRes res = str_from_chars(s2, split + 1, src.len - s1->len - 1);
    if (res != STR_SUCCESS) {
        str_deinit(s1);
    }

    return res;
}

const char *str_inner(struct DynStr *string)
{
    return string->data;
}

bool str_equals(const struct DynStr *s1, const struct DynStr *s2)
{
    if (s1->len != s2->len)
        return false;

    return str_equals_chars(s1, s2->data);
}

bool str_equals_chars(const struct DynStr *s1, const char *s2)
{
    // Compare everything, including the trailing \0 to make sure the second
    // string isn't just longer than the first.
    return strncmp(s1->data, s2, s1->len + 1) == 0;
}

bool str_starts_with_chars(const struct DynStr *s1, const char *s2)
{
    const size_t start_len = strlen(s2);
    if (start_len > s1->len)
        return false;

    return strncmp(s1->data, s2, start_len) == 0;
}
