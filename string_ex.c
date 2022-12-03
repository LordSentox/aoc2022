#include "string_ex.h"

#include <stdbool.h>
#include <stdlib.h>

static inline bool is_whitespace(char c)
{
    return c == ' ' || c == '\n' || c == '\t';
}

char *strtrim(char *string, size_t *size) {
    if (string == NULL)
	return NULL;

    // Shift right until we hit a non-whitespace character or the end of the string
    size_t pos = 0;
    while (pos < *size && string[pos] != '\0' && is_whitespace(string[pos])) {
	++pos;
    }
    char *trimmed = string + pos;
    *size -= pos;

    if (*size == 0)
	return trimmed;

    // Remove trailing whitespaces from the end of the string
    // Note that 
    while (*size > 1 && (is_whitespace(trimmed[*size - 1]) || trimmed[*size - 1] == '\0')) {
	trimmed[*size - 1] = '\0';
	--*size;
    }

    return trimmed;
}
