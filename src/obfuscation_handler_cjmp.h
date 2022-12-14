#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "deob_context.h"

extern const uint8_t obfuscation_pattern_cjmp[101];

bool obfuscation_handler_cjmp(const struct deob_context* const context, uint8_t** segment_addresses);