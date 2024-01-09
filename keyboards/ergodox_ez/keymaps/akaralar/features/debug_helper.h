
#pragma once

#include "quantum.h"

void enable_debug_user(void);
void prefixed_print(uint16_t keycode, keyrecord_t *record, const char* prefix);
void prefixed_print_layer(layer_state_t state, const char* prefix);
void prefixed_print_all_mods(const char* prefix);
