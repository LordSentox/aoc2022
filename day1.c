#include "integers.h"

#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

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

i64 read_next_elfs_calories(FILE *file)
{
    char *line = NULL;
    size_t line_len = 0;
    ssize_t read;

    u32 sum = 0;
    while ((read = getline(&line, &line_len, file)) != -1) {
	// Empty line denominates the next elf
	if (strncmp(line, "\n", read) == 0) {
	    free(line);
	    return sum;
	}

	const char *trimmed = strtrim(line, (size_t *) &read);
	uintmax_t cal = strtoumax(trimmed, NULL, 10);
	if (cal == UINTMAX_MAX) {
	    printf("Error parsing calories. %d", errno);
	    free(line);
	    line = NULL;
	    return -2;
	}

	sum += cal;
    }

    free(line);
    return -1;
}

int main()
{
    FILE *file = fopen("input/day1.txt", "r");
    if (file == NULL) {
	printf("Unable to open input file");
	return 1;
    }

    u32 max = 0;
    i64 current;
    while ((current = read_next_elfs_calories(file)) != -1) {
	max = u32_max(max, current);

	printf("Elf carries: %d kcal\n", current);
    }

    printf("Maximum number of calories: %d kcal\n", max);

    return 0;
}
