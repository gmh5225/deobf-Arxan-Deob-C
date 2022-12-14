#pragma once

#include <stdint.h>
#include <stddef.h>

#define ARRAY_NPOS (SIZE_MAX)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

#define FIND_U8(array, find) find_in_array_u8(array, ARRAY_SIZE(array), find)

static inline size_t find_in_array_u8(const uint8_t* array, const size_t array_size, const uint8_t find)
{
    for (size_t i = 0; i < array_size; i++)
    {
        if (array[i] == find)
        {
            return i;
        }
    }
    return SIZE_MAX;
}