#pragma once

#include "quantum.h"
#include "common.h"

// RGB matrix layer lighting: per-layer LED on/off maps and colors, applied via
// the rgb_matrix_indicators_user callback, plus the RGB_* keycode handling.
//
// NOTE: this whole feature is compiled only when RGB_MATRIX_ENABLE is set. The
// ergodox_ez/glow keymap currently builds with RGB_MATRIX_ENABLE = no, so none
// of this is active in the current firmware - it is kept here for when RGB
// matrix is turned back on.

#if RGB_MATRIX_ENABLE
// Handle the RGB_TGL / RGB_BUP / RGB_BDN custom keycodes. Returns false (and
// consumes the event) when it handled the keycode, true otherwise.
bool process_rgb_matrix_keycodes(uint16_t keycode, keyrecord_t *record);
#endif
