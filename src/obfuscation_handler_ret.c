#include <string.h>

#include "obfuscation_handler_ret.h"

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

    memset(segment_addresses[0], 0x90, 5);
    memset(segment_addresses[1], 0x90, 4);

    segment_addresses[0][0] = 0xC3;

    return true;
}