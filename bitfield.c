#include "bitfield.h"

static inline size_t data_size(size_t bitlen)
{
    size_t data_size = bitlen / 8;
    if (bitlen % 8 != 0)
	data_size++;

    return data_size;
}

void bitfield_init(struct Bitfield *field, size_t len)
{
    field->data = calloc(1, data_size(len));
    field->len = len;
}

void bitfield_deinit(struct Bitfield *field)
{
    free(field->data);
    field->data = NULL;
    field->len = 0;
}

bool bitfield_is_bit_high(const struct Bitfield *field, size_t bit)
{
    if (bit >= field->len)
	return false;
    
    // Select the byte the bits status is stored in
    const u8 *byte = field->data + (bit / 8);

    // Return state of the bit at the correct place.
    return *byte & (1 << (bit % 8));
}

void bitfield_set_bit(struct Bitfield *field, size_t bit, bool value)
{
    if (bit >= field->len)
	return;

    // Select the byte the bit should be stored in
    u8 *byte = field->data + (bit / 8);

    u8 bit_flag = 1 << bit % 8;
    if (value) {
	*byte |= bit_flag;
    } else {
	*byte &= ~bit_flag;
    }
}

struct Bitfield bitfield_bitwise_and(const struct Bitfield *fields, size_t num_fields)
{
    if (num_fields == 0) {
	struct Bitfield empty;
	empty.data = NULL;
	empty.len = 0;
	return empty;
    }
    
    // Maximum size will be chosen
    size_t max_bitlen = 0;
    size_t min_bitlen = SIZE_MAX;
    for (size_t i = 0; i < num_fields; ++i) {
	if (fields[i].len > max_bitlen)
	    max_bitlen = fields[i].len;
	if (fields[i].len < min_bitlen)
	    min_bitlen = fields[i].len;
    }

    struct Bitfield result;
    bitfield_init(&result, max_bitlen);

    /* Compare the bits required, the bits after the minimum sized field has ended
       can be set to 0 (which they are by default) */
    for (size_t i = 0; i < data_size(min_bitlen); ++i) {
	result.data[i] = fields[0].data[i];
	for (size_t k = 0; k < num_fields; k++) {
	    result.data[i] &= fields[k].data[i];
	}
    }
    
    return result;
}

ssize_t bitfield_first_high_bit_pos(const struct Bitfield *field)
{
    size_t size = data_size(field->len);
    ssize_t byte_containing_high = -1;
    for (size_t i = 0; i < size; ++i) {
	if (field->data[i] != 0) {
	    byte_containing_high = i;
	    break;
	}
    }

    if (byte_containing_high == -1)
	return -1;

    for (u8 i = 0; i < 8; ++i) {
	if (field->data[byte_containing_high] & 1 << i)
	    return byte_containing_high * 8 + i;
    }

    return -1;
}
