#pragma once

#include <stdint.h>

uint32_t virtual_address_to_file_offset(const void* file_data, const uintptr_t virtual_address);

uintptr_t file_offset_to_virtual_address(const void* file_data, const uint32_t file_offset);