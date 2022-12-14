#include <string.h>

#include "obfuscation_handler_push_pop.h"
#include "array_util.h"
#include "pattern.h"

#define X64_REX_W  (0x48)
#define X64_REX_WR (0x4C)

static const uint8_t push_pop_registers_1[] = { 0x44, 0x4C, 0x54, 0x5C, 0x64, 0x6C, 0x74, 0x7C };
static const uint8_t push_pop_registers_2[] = { 0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C, 0x34, 0x3C };

const uint8_t obfuscation_pattern_push_1[23] =
{
    0x02,
    0x05, 0x05,

    0x00, 0x89, 0x00, 0x24, 0xF8,
    0x00, 0xFF, 0x00, 0xFF, 0xFF,

    0x48, 0x8D, 0x64, 0x24, 0xF8,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

const uint8_t obfuscation_pattern_push_2[21] =
{
    0x02,
    0x05, 0x04,

    0x48, 0x8D, 0x64, 0x24, 0xF8,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,

    0x00, 0x89, 0x00, 0x24,
    0x00, 0xFF, 0x00, 0xFF,
};

const uint8_t obfuscation_pattern_pop_1[] =
{
    0x02,
    0x05, 0x05,

    0x48, 0x8D, 0x64, 0x24, 0x08,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,

    0x00, 0x8B, 0x00, 0x24, 0xF8,
    0x00, 0xFF, 0x00, 0xFF, 0xFF,
};

const uint8_t obfuscation_pattern_pop_2[] =
{
    0x02,
    0x04, 0x05,

    0x00, 0x8B, 0x00, 0x24,
    0x00, 0xFF, 0x00, 0xFF,

    0x48, 0x8D, 0x64, 0x24, 0x08,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

static bool handle_push_pop(uint8_t** segment_addresses, const uint8_t* pattern, const size_t rex_segment, const uint8_t* const register_list, const size_t register_segment, const size_t register_index, const uint8_t opcode_base)
{
    const uint8_t rex = segment_addresses[rex_segment][0];

    if ((rex != X64_REX_W) && (rex != X64_REX_WR))
    {
        return false;
    }

    const size_t position = find_in_array_u8(register_list, 8, segment_addresses[register_segment][register_index]); // 8 = register list size. (both push_pop_registers 1&2 are 8 elements)

    if (position == ARRAY_NPOS)
    {
        return false;
    }

    for (size_t i = 0; i < pattern_get_section_count(pattern); i++)
    {
        memset(segment_addresses[i], 0x90, pattern_get_section_size(pattern, i));
    }

    const uint8_t opcode = opcode_base + (uint8_t)position;

    segment_addresses[0][0] = opcode;

    if (rex == X64_REX_WR)
    {
        segment_addresses[0][0] = 0x41; // REX.B (opcode reg field)
        segment_addresses[0][1] = opcode;
    }

    return true;
}

bool obfuscation_handler_push_1(const struct deob_context* const context, uint8_t** segment_addresses)
{
    (void)context;
    return handle_push_pop(segment_addresses, obfuscation_pattern_push_1, 0, push_pop_registers_1, 0, 2, 0x50);
}

bool obfuscation_handler_push_2(const struct deob_context* const context, uint8_t** segment_addresses)
{
    (void)context;
    return handle_push_pop(segment_addresses, obfuscation_pattern_push_2, 1, push_pop_registers_2, 1, 2, 0x50);
}

bool obfuscation_handler_pop_1(const struct deob_context* const context, uint8_t** segment_addresses)
{
    (void)context;
    return handle_push_pop(segment_addresses, obfuscation_pattern_pop_1, 1, push_pop_registers_1, 1, 2, 0x58);
}

bool obfuscation_handler_pop_2(const struct deob_context* const context, uint8_t** segment_addresses)
{
    (void)context;
    return handle_push_pop(segment_addresses, obfuscation_pattern_pop_2, 0, push_pop_registers_2, 0, 2, 0x58);
}