#include "integers.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define UNUSED __attribute__((unused))

/* Return the value of the outcome. Input is what the opponent and the player
 * have chosen. The return will be 0 if the opponent wins, 3 if it is a draw and
 * finally, if the players wins it will be 6.
 * If one of the inputs is malformed, a -1 will be returned */
i8 outcome_value(const char opponent, const char player)
{
    if (opponent == 'A') {
        switch (player) {
        case 'X':
            return 3;
        case 'Y':
            return 6;
        case 'Z':
            return 0;
        default:
            return -1;
        }
    }
    if (opponent == 'B') {
        switch (player) {
        case 'X':
            return 0;
        case 'Y':
            return 3;
        case 'Z':
            return 6;
        default:
            return -1;
        }
    }
    if (opponent == 'C') {
        switch (player) {
        case 'X':
            return 6;
        case 'Y':
            return 0;
        case 'Z':
            return 3;
        default:
            return -1;
        }
    }

    return -1;
}

i8 outcome_value2(const char opponent, const char player)
{
    if (player == 'X') {
        /* Always choose the wrong answer and only get points for it, no points
           for the outcome */
        switch (opponent) {
        case 'A':
            return 3;
        case 'B':
            return 1;
        case 'C':
            return 2;
        default:
            return -1;
        }
    }

    if (player == 'Y') {
        /* Draw is valued as three, now we need to add the choice depending on
           what the opponent has chosen (we always choose the same) */
        switch (opponent) {
        case 'A':
            return 4;
        case 'B':
            return 5;
        case 'C':
            return 6;
        default:
            return -1;
        }
    }

    if (player == 'Z') {
        /* Winning is valued at six, now we need to add the choice depending on
           what the opponent has chosen (always what is winning against them) */
        switch (opponent) {
        case 'A':
            return 8;
        case 'B':
            return 9;
        case 'C':
            return 7;
        default:
            return -1;
        }
    }

    return -1;
}

i8 choice_value(const char choice)
{
    switch (choice) {
    case 'X':
        return 1;
    case 'Y':
        return 2;
    case 'Z':
        return 3;
    default:
        return -1;
    }
}

i8 choice_value2(UNUSED const char choice)
{
    // No longer a factor, since it's handled in outcome_value2
    return 0;
}

i8 calculate_next_outcome(FILE *file, i8 (*ov_func)(const char, const char),
                          i8 (*cv_func)(const char))
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
    free(line);

    i8 ov = ov_func(opponent, player);
    i8 cv = cv_func(player);

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
    while ((current_result =
                calculate_next_outcome(file, &outcome_value, &choice_value))
           != -1)
    {
        total_points += current_result;
    }

    printf("Total points for part one are: %d\n", total_points);

    rewind(file);
    total_points = 0;
    while ((current_result =
                calculate_next_outcome(file, &outcome_value2, &choice_value2))
           != -1)
    {
        total_points += current_result;
    }

    printf("Total points for part two are: %d\n", total_points);

    fclose(file);
    return 0;
}
