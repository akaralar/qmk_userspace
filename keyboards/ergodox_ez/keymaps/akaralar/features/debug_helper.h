
#pragma once

#include "quantum.h"

#if CONSOLE_ENABLE
void enable_debug_user(void);
void prefixed_print(uint16_t keycode, keyrecord_t *record, const char* prefix);
void prefixed_print_keycode(uint16_t keycode, const char* prefix);
void prefixed_print_letter(uint16_t keycode, const char* prefix);
void prefixed_print_layer(layer_state_t state, const char* prefix);
void prefixed_print_all_mods(const char* prefix);
#endif
