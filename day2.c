#include "integers.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/* Return the value of the outcome. Input is what the opponent and the player have
 * chosen. The return will be 0 if the opponent wins, 3 if it is a draw and
 * finally, if the players wins it will be 6.
 * If one of the inputs is malformed, a -1 will be returned */
i8 outcome_value(const char opponent, const char player)
{
    if (opponent == 'A') {
	switch (player) {
	case 'X': return 3;
	case 'Y': return 6;
	case 'Z': return 0;
	default: return -1;
	}
    }
    if (opponent == 'B') {
	switch (player) {
	case 'X': return 0;
	case 'Y': return 3;
	case 'Z': return 6;
	default: return -1;
	}
    }
    if (opponent == 'C') {
	switch (player) {
	case 'X': return 6;
	case 'Y': return 0;
	case 'Z': return 3;
	default: return -1;
	}
    }

    return -1;
}

i8 choice_value(const char choice)
{
    switch (choice) {
    case 'X': return 1;
    case 'Y': return 2;
    case 'Z': return 3;
    default: return -1;
    }
}

i8 calculate_next_outcome(FILE *file)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    read = getline(&line, &len, file);
    
    if (read != 4) {
	free(line);
	return -1;
    }

    const char opponent = line[0];
    const char player = line[2];

    i8 ov = outcome_value(opponent, player);
    i8 cv = choice_value(player);

    if (ov == -1 || cv == -1)
	return -1;

    return ov + cv;
}

int main()
{
    FILE *file = fopen("input/day2.txt", "r");
    if (file == NULL) {
	printf("Unable to open input file");
	return 1;
    }

    u32 total_points = 0;
    i8 current_result;
    while ((current_result = calculate_next_outcome(file)) != -1) {
	total_points += current_result;
    }

    fclose(file);

    printf("Total points are: %d", total_points);
    return 0;
}
