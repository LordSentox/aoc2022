#include "bitfield.h"
#include "integers.h"
#include "string_ex.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GROUP_SIZE 3
#define NUM_ITEMS  52

u8 priority(unsigned char c)
{
    // Lowercase priority
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 1;
    if (c >= 'A' && c <= 'Z')
        return c - 'A' + 27;

    printf("Not a valid character: '%c'", c);
    exit(EXIT_FAILURE);
}

unsigned char mispacked_item(const char *comp_a, const char *comp_b,
                             size_t size)
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

void init_all_fields(struct Bitfield *fields, size_t num)
{
    for (size_t i = 0; i < num; ++i) {
        bitfield_init(&fields[i], NUM_ITEMS);
    }
}

void deinit_all_fields(struct Bitfield *fields, size_t num)
{
    for (size_t i = 0; i < num; ++i) {
        bitfield_deinit(&fields[i]);
    }
}

void add_priorities_to_field(struct Bitfield *field, char *string, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        u8 prio = priority((unsigned char) string[i]);
        bitfield_set_bit(field, prio - 1, true);
    }
}

bool next_groups_item(FILE *file, u8 *prio)
{
    struct Bitfield found_items[GROUP_SIZE];
    init_all_fields(found_items, GROUP_SIZE);

    char *line = NULL;
    size_t line_len = 0;
    for (size_t i = 0; i < GROUP_SIZE; ++i) {
        ssize_t read = getline(&line, &line_len, file);
        if (read == -1) {
            deinit_all_fields(found_items, GROUP_SIZE);
            free(line);
            return false;
        }

        add_priorities_to_field(&found_items[i], line, read - 1);
    }
    free(line);

    // Find an item they all have in common
    struct Bitfield common_items =
        bitfield_bitwise_and(found_items, GROUP_SIZE);
    ssize_t first_common = bitfield_first_high_bit_pos(&common_items);

    deinit_all_fields(found_items, GROUP_SIZE);
    bitfield_deinit(&common_items);

    if (first_common != -1) {
        *prio = first_common + 1;
        return true;
    }
    else {
        return false;
    }
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

    printf("Sum of mispacked item priorities: %d\n", priority_sum);
    free(line);

    rewind(file);

    u8 prio;
    priority_sum = 0;
    while (next_groups_item(file, &prio)) {
        priority_sum += prio;
    }

    printf("The priorities of all groups' identifying items: %d\n",
           priority_sum);

    fclose(file);
    return EXIT_SUCCESS;
}
