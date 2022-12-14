#include <string.h>

#include "obfuscation_handler_jmp.h"

#include "x64_util.h"
#include "win32_util.h"

const uint8_t obfuscation_pattern_jmp[31] =
{
    0x04,
    0x01, 0x07, 0x04, 0x01,

    0x55,
    0xFF,

    0x48, 0x8D, 0x2D, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,

    0x48, 0x87, 0x2C, 0x24,
    0xFF, 0xFF, 0xFF, 0xFF,

    0xC3,
    0xFF,
};

bool obfuscation_handler_jmp(const struct deob_context* const context, uint8_t** segment_addresses)
{
    const uintptr_t va_lea = file_offset_to_virtual_address(context->deob_begin, (uint32_t)(segment_addresses[1] - context->deob_begin));
    const uintptr_t va_source = file_offset_to_virtual_address(context->deob_begin, (uint32_t)(segment_addresses[0] - context->deob_begin));
    const uintptr_t va_target = x64_calc_rel32(va_lea + 3, *(int32_t*)(segment_addresses[1] + 3), 0);

    x64_make_rel32_jmp(segment_addresses[0], va_source, va_target);

    return true;
}