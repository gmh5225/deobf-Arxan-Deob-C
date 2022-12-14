#pragma once

#include <stdint.h>
#include <stddef.h>

static inline uintptr_t x64_calc_rel32(const uintptr_t address, const int32_t rel, const size_t offset)
{
    return (address + rel + sizeof(int32_t) + offset);
}

static inline void x64_make_rel32_jmp(void* addr, const uintptr_t jmp_from, const uintptr_t jmp_to)
{
    uint8_t* a = addr;

    a[0] = 0xE9;
    *((uint32_t*)(a + 1)) = (uint32_t)(jmp_to - (jmp_from + 5));
    return;
}

static inline void x64_make_rel32_cjmp(void* addr, const uintptr_t jmp_from, const uintptr_t jmp_to, const uint8_t type)
{
    uint8_t* a = addr;

    a[0] = 0x0F;
    a[1] = type;
    *((uint32_t*)(a + 2)) = (uint32_t)(jmp_to - (jmp_from + 6));
    return;
}