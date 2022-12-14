#pragma once

#include <stdint.h>
#include <stdbool.h>

struct memory_range
{
    uint8_t* begin;
    uint8_t* end;
};


bool read_binary_file(struct memory_range* out, const char* const file_path);