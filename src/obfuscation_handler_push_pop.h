#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "deob_context.h"

extern const uint8_t obfuscation_pattern_push_1[23];
extern const uint8_t obfuscation_pattern_push_2[21];

extern const uint8_t obfuscation_pattern_pop_1[23];
extern const uint8_t obfuscation_pattern_pop_2[21];

bool obfuscation_handler_push_1(const struct deob_context* const context, uint8_t** segment_addresses);
bool obfuscation_handler_push_2(const struct deob_context* const context, uint8_t** segment_addresses);
bool obfuscation_handler_pop_1(const struct deob_context* const context, uint8_t** segment_addresses);
bool obfuscation_handler_pop_2(const struct deob_context* const context, uint8_t** segment_addresses);