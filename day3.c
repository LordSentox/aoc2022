#include "integers.h"
#include "string_ex.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

u8 priority(unsigned char c) {
    // Lowercase priority
    if (c >= 'a' && c <= 'z')
	return c - 'a' + 1;
    if (c >= 'A' && c <= 'Z')
	return c - 'A' + 27;

    printf("Not a valid character: '%c'", c);
    exit(EXIT_FAILURE);
}

unsigned char mispacked_item(const char *comp_a, const char *comp_b, size_t size)
{
    // Go through the items in the first backpack compartment
    for (size_t i = 0; i < size; ++i) {
	char *wrongly_packed = strnchr(comp_b, comp_a[i], size);
	if (wrongly_packed != NULL)
	    return *wrongly_packed;
    }

    printf("A rucksack was packed correctly???");
    exit(EXIT_FAILURE);
}

int main()
{
    FILE *file = fopen("input/day3.txt", "r");
    if (file == NULL) {
	printf("Unable to open input file");
	return 1;
    }

    char *line = NULL;
    size_t line_len = 0;
    ssize_t read;
    u32 priority_sum = 0;
    while ((read = getline(&line, &line_len, file)) != -1) {
	size_t half = read / 2;
	unsigned char mispacked = mispacked_item(line, line + half, half);
	priority_sum += priority(mispacked);
    }
    free(line);

    printf("Sum of mispacked item priorities: %d\n", priority_sum);

    fclose(file);
    return EXIT_SUCCESS;
}
