#pragma once

#include <stdint.h>
#include <stdio.h>

struct deob_context
{
    FILE* log_file;

    uint8_t* deob_begin;
    uint8_t* deob_end;
};