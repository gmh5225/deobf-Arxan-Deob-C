#pragma once

#include <stddef.h>
#include <stdint.h>

#define MAX_PATTERN_SEGMENTS (15)

static inline size_t pattern_get_section_count(const uint8_t* const pattern)
{
    return pattern[0];
}

static inline size_t pattern_get_section_size(const uint8_t* const pattern, const size_t section)
{
    return pattern[1 + section];
}

static inline size_t pattern_get_section(const uint8_t* const pattern, const size_t section, const uint8_t** const data, const uint8_t** const mask)
{
    const size_t section_size = pattern_get_section_size(pattern, section);
    size_t offset = 1 + pattern_get_section_count(pattern);

    for (size_t i = 0; i < section; i++)
    {
        offset += pattern_get_section_size(pattern, i) * 2;
    }

    *data = (pattern + offset);
    *mask = (pattern + offset + section_size);

    return section_size;
}