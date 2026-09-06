#pragma once

#include "quantum.h"
#include "common.h"

// Symbol-layer "fake" layer-tap keys and their tap/hold handling.
//
// Each fake LT is an LT(SYMB, ...) placeholder: the keycode it names is never
// sent. Instead this feature decides tap vs hold and either sends a symbol
// (tap) or runs a macro / layer switch (hold). It also reconstructs the
// intended action when the symbol layer "collapses" - i.e. MO(SYMB) is released
// before the key settles and QMK resolves it on the base layer.
// See https://getreuer.info/posts/keyboards/triggers/index.html

//------------------------------------------------------------------------------
// Fake layer-tap keys
//------------------------------------------------------------------------------
// Tapping sends a key, holding performs the associated macro. Because some of
// the tap symbols are not basic keycodes, both actions are handled in code
// (see fake_lt_actions), so the keycode named here is only a placeholder.
#define FT_SLSH LT(SYMB, KC_0)
#define FT_LBRC LT(SYMB, KC_1)
#define FT_LPRN LT(SYMB, KC_2)
#define FT_LABK LT(SYMB, KC_3)
#define FT_RCBR LT(SYMB, KC_4)
#define FT_DQUO LT(SYMB, KC_5)
#define FT_QUOT LT(SYMB, KC_6)
#define FT_UNDS LT(SYMB, KC_7)
#define FT_ASTR LT(SYMB, KC_8)
#define FT_GRV LT(SYMB, KC_9)
#define FT_CBL LT(SYMB, KC_A)
#define FT_CBLS LT(SYMB, KC_B)

// LS_SNUM is a layer-tap into SNUM, but its tap/hold is handled by this feature
// (tap sends "{", hold switches to the SNUM layer), so it lives here with the
// other fake LT keys rather than in the keymap's real layer-switch group.
#define LS_SNUM LT(SNUM, KC_3) // The tap is intercepted to send "{"

//------------------------------------------------------------------------------
// Entry points (call from pre_process_record_user / process_record_user)
//------------------------------------------------------------------------------
bool pre_process_symbol_layer_fake_lt_keys(uint16_t keycode, keyrecord_t *record);
bool process_symbol_layer_fake_lt_keys(uint16_t keycode, keyrecord_t *record);
