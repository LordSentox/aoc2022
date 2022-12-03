#ifndef INTEGERS_H
#define INTEGERS_H

#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

static inline u16 u16_max(u16 a, u16 b)
{
    return a > b ? a : b;
}
static inline u32 u32_max(u32 a, u32 b)
{
    return a > b ? a : b;
}

#endif /* INTEGERS_H */
