
#pragma once

#include "quantum.h"

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// Caps Lock
//
// Caps lock is mapped to Globe (fn) key in the system level. For that reason
// we don't have a way to enable regular caps lock anymore. What we do instead
// is we keep track of caps lock state manually and when caps lock is enabled
// we enable caps word as we come across keys that needs to be capitalized,
// which are only A-Z. Furthermore when caps lock is enabled, caps word disables
// itself when it comes across any character that isn't A-Z, allowing us to use
// modifiers as we like, and later when we type A-Z we enable it again.
//------------------------------------------------------------------------------
bool process_custom_caps_lock(uint16_t keycode, keyrecord_t* record);

void caps_lock_on(void);
void caps_lock_off(void);
void caps_lock_toggle(void);
bool is_caps_lock_on(void);

#ifdef __cplusplus
}
#endif
