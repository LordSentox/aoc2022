#include "integers.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Pair
{
    u32 range_a_start;
    u32 range_a_end;
    u32 range_b_start;
    u32 range_b_end;
};

char *split_through_next(char *string, char c)
{
    char *next = strchr(string, c);

    // Check that the character was found
    if (next == NULL) {
	printf("String was improperly formatted\n");
	exit(EXIT_FAILURE);
    }

    // Split the string at this position, removing the character
    *next = '\0';
    return next + 1;
}

bool pair_one_range_hugs_other(struct Pair *pair)
{
    bool a_hugs_b = pair->range_a_start <= pair->range_b_start
	&& pair->range_a_end >= pair->range_b_end;
    bool b_hugs_a = pair->range_b_start <= pair->range_a_start
	&& pair->range_b_end >= pair->range_a_end;

    return a_hugs_b || b_hugs_a;
}

struct Pair line_to_pair(char *string)
{
    // Discard all but the numbers, placing \0 in their place.
    char *a_start = string;
    char *a_end = split_through_next(string, '-');
    char *b_start = split_through_next(a_end, ',');
    char *b_end = split_through_next(b_start, '-');
    split_through_next(b_end, '\n');

    struct Pair pair;
    pair.range_a_start = strtoul(a_start, NULL, 10);
    pair.range_a_end = strtoul(a_end, NULL, 10);
    pair.range_b_start = strtoul(b_start, NULL, 10);
    pair.range_b_end = strtoul(b_end, NULL, 10);

    return pair;
}

bool line_describes_hugging_ranges(char *line)
{
    struct Pair pair = line_to_pair(line);
    return pair_one_range_hugs_other(&pair);
}

int main()
{
    FILE *file = fopen("input/day4.txt", "r");
    if (file == NULL) {
	printf("Unable to read input file\n");
	return EXIT_FAILURE;
    }

    char *line = NULL;
    size_t line_alloc = 0;
    ssize_t read;
    u32 number_of_hugging_ranges = 0;
    while ((read = getline(&line, &line_alloc, file)) != -1) {
	if (line_describes_hugging_ranges(line))
	    number_of_hugging_ranges++;
    }

    printf("Number of elves that do all the work another elf was also assigned to: %d\n", number_of_hugging_ranges);
    
    free(line);
    fclose(file);
    return EXIT_SUCCESS;
}
