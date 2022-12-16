#include <string.h>
#include "obfuscation_handler_cjmp.h"
#include "x64_util.h"
#include "win32_util.h"
#include "pattern.h"

const uint8_t obfuscation_pattern_cjmp[101] =
{
    0x0C,
    0x01, 0x0A, 0x04, 0x01, 0x01, 0x05, 0x0A, 0x04, 0x05, 0x01, 0x01, 0x01,

    0x55,
    0xFF,

    0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x48, 0x87, 0x2C, 0x00,
    0xFF, 0xFF, 0xFF, 0x00,

    0x50,
    0xF0,

    0x50,
    0xF0,

    0x48, 0x8B, 0x00, 0x24, 0x10,
    0xFF, 0xFF, 0x00, 0xFF, 0xFF,

    0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x00, 0x0F, 0x00, 0x00,
    0x00, 0xFF, 0x00, 0x00,

    0x48, 0x89, 0x00, 0x24, 0x10,
    0xFF, 0xFF, 0x00, 0xFF, 0xFF,

    0x50,
    0xF0,

    0x50,
    0xF0,

    0xC3,
    0xFF,
};

static uint32_t ptr_distance(const uintptr_t lhs, const uintptr_t rhs)
{
    if (lhs > rhs)
    {
        return (uint32_t)(lhs - rhs);
    }
    return (uint32_t)(rhs - lhs);
}

bool obfuscation_handler_cjmp(const struct deob_context* const context, uint8_t** segment_addresses)
{
    const uint8_t cmov = segment_addresses[7][2];
    
    if ((cmov & 0xF0) != 0x40)
    {
        return false;
    }

    const uintptr_t jmp_to  = *(uintptr_t*)(segment_addresses[1] + 2);
    const uintptr_t cjmp_to = *(uintptr_t*)(segment_addresses[6] + 2);

    const size_t section_count = pattern_get_section_count(obfuscation_pattern_cjmp);
    for (size_t i = 0; i < section_count; i++)
    {
        const size_t section_size = pattern_get_section_size(obfuscation_pattern_cjmp, i);
        memset(segment_addresses[i], 0x90, section_size);
    }

    uintptr_t va_source = file_offset_to_virtual_address(context->deob_begin, (uint32_t)(segment_addresses[0] - context->deob_begin));
    uintptr_t va_target = file_offset_to_virtual_address(context->deob_begin, (uint32_t)(segment_addresses[1] - context->deob_begin));

    if (ptr_distance(va_source, va_target) >= 5)
    {
        x64_make_rel32_jmp(segment_addresses[0], va_source, va_target);
    }

    va_source = file_offset_to_virtual_address(context->deob_begin, (uint32_t)(segment_addresses[1] - context->deob_begin));
    va_target = cjmp_to;

    x64_make_rel32_cjmp(segment_addresses[1], va_source, va_target, (0x80 | (cmov & 0x0F)));

    va_source = file_offset_to_virtual_address(context->deob_begin, (uint32_t)((segment_addresses[1] + 6) - context->deob_begin));
    va_target = jmp_to;

    x64_make_rel32_jmp(segment_addresses[1] + 6, va_source, va_target);

    return true;
}