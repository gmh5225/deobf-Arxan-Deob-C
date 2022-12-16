#include <string.h>

#include "obfuscation_handler_ret.h"

#include "pattern.h"
#include "x64_util.h"

const uint8_t obfuscation_pattern_ret[21] =
{
    0x02,
    0x05, 0x04,

    0x48, 0x8D, 0x64, 0x24, 0x08,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,

    0xFF, 0x64, 0x24, 0xF8,
    0xFF, 0xFF, 0xFF, 0xFF,
};

bool obfuscation_handler_ret(const struct deob_context* const context, uint8_t** segment_addresses)
{
    (void)context;

    const size_t section_count = pattern_get_section_count(obfuscation_pattern_ret);
    for (size_t i = 0; i < section_count; i++)
    {
        const size_t section_size = pattern_get_section_size(obfuscation_pattern_ret, i);
        memset(segment_addresses[i], 0x90, section_size);
    }

    segment_addresses[0][0] = 0xC3;

    return true;
}