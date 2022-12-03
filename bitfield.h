#ifndef BITFIELD_H
#define BITFIELD_H

#include "integers.h"
#include <stdlib.h>
#include <stdbool.h>

struct Bitfield {
    u8 *data;
    size_t len;
};

void bitfield_init(struct Bitfield *field, size_t len);
void bitfield_deinit(struct Bitfield *field);

bool bitfield_is_bit_high(const struct Bitfield *field, size_t bit);
void bitfield_set_bit(struct Bitfield *field, size_t bit, bool value);

struct Bitfield bitfield_bitwise_and(const struct Bitfield *fields, size_t num_fields);

ssize_t bitfield_first_high_bit_pos(const struct Bitfield *field);

#endif /* BITFIELD_H */
