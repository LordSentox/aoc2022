#include "dyn_str.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define START_SEQ_LEN (4u)

bool are_unique(const char *items)
{
    for (size_t i = 0; i < START_SEQ_LEN - 1; ++i) {
        for (size_t j = i + 1; j < START_SEQ_LEN; ++j) {
            if (items[i] == items[j])
                return false;
        }
    }

    return true;
}

void shift_out_left(char *items, char new)
{
    for (size_t i = 0; i < START_SEQ_LEN - 1; ++i) {
        items[i] = items[i + 1];
    }
    items[START_SEQ_LEN - 1] = new;
}

ssize_t find_first_unique_four(const char *string, size_t len)
{
    char last_chars[START_SEQ_LEN];
    memcpy(last_chars, string, START_SEQ_LEN);

    for (size_t i = START_SEQ_LEN; i < len; ++i) {
        if (are_unique(last_chars))
            return i;
        if (string[i] == '\0') {
            printf("Input malformatted\n");
            exit(EXIT_FAILURE);
        }

        shift_out_left(last_chars, string[i]);
    }

    return -1;
}

int main()
{
    FILE *file = fopen("input/day6.txt", "r");
    if (file == NULL) {
        printf("Unable to open input file\n");
        return EXIT_FAILURE;
    }

    struct DynStr input;
    if (str_from_file(file, &input) != STR_SUCCESS) {
        printf("Unable to read file\n");
        return EXIT_FAILURE;
    }

    ssize_t start_seq_index =
        find_first_unique_four(str_inner(&input), input.len);
    if (start_seq_index == -1) {
        printf("Start sequence not found\n");
        str_deinit(&input);
        fclose(file);
        return EXIT_FAILURE;
    }

    printf("Start sequence index is: %zd\n", start_seq_index);

    str_deinit(&input);
    fclose(file);
    return EXIT_SUCCESS;
}
