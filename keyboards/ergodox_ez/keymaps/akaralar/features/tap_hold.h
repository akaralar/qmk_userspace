#pragma once

#include "quantum.h"
#include "common.h"

// Mod-tap and layer-switch key definitions, plus the tap/hold tuning callbacks
// (get_tapping_term, get_permissive_hold, get_chordal_hold, flow tap and
// speculative hold), which are defined in tap_hold.c as QMK weak overrides -
// the keymap never calls them directly, so no prototypes are needed here.
//
// The key aliases live in this header, rather than in the keymap, because they
// are the keys whose tap/hold and layer-switch behavior this module governs.
// The keymap includes this header to place them in the layout.

//------------------------------------------------------------------------------
// Mod-tap keys
//------------------------------------------------------------------------------
// mod-tap keys same for qwerty and colemak
#define MT_A MT(MOD_LCTL, KC_A)
#define MT_W MT(MOD_LSFT | MOD_LCTL | MOD_LGUI, KC_W)

// mod-tap keys for qwerty
#define MT_Q_E MEH_T(KC_E)
#define MT_Q_R ALL_T(KC_R)
#define MT_Q_U ALL_T(KC_U)
#define MT_Q_I MEH_T(KC_I)
#define MT_Q_O MT(MOD_RSFT | MOD_RCTL | MOD_RGUI, KC_O)
#define MT_Q_F MT(MOD_LSFT, KC_F)
#define MT_Q_D MT(MOD_LGUI, KC_D)
#define MT_Q_S MT(MOD_LALT, KC_S)
#define MT_Q_J MT(MOD_RSFT, KC_J)
#define MT_Q_K MT(MOD_RGUI, KC_K)
#define MT_Q_L MT(MOD_LALT, KC_L)
#define MT_Q_QT MT(MOD_RCTL, KC_QUOTE)

// mod-tap keys for colemak-dh
#define MT_C_F MEH_T(KC_F)
#define MT_C_P ALL_T(KC_P)
#define MT_C_L ALL_T(KC_L)
#define MT_C_U MEH_T(KC_U)
#define MT_C_Y MT(MOD_RSFT | MOD_RCTL | MOD_RGUI, KC_Y)
#define MT_C_T MT(MOD_LSFT, KC_T)
#define MT_C_S MT(MOD_LGUI, KC_S)
#define MT_C_R MT(MOD_LALT, KC_R)
#define MT_C_N MT(MOD_RSFT, KC_N)
#define MT_C_E MT(MOD_RGUI, KC_E)
#define MT_C_I MT(MOD_LALT, KC_I)
#define MT_C_O MT(MOD_RCTL, KC_O)

//------------------------------------------------------------------------------
// Layer switching keys
//------------------------------------------------------------------------------
// Layer-taps
#define LS_NAVI LT(NAVI, KC_SPACE)
#define LS_MOUS LT(MOUS, KC_TAB)
#define LS_MDIA LT(MDIA, KC_ESCAPE)
#define LS_NUMB LT(NUMB, KC_BSPC)
// LS_SNUM is defined in features/symbol_layer.h (its tap/hold is handled there).
#define LS_FUNC LT(FUNC, KC_ENTER)
// Momentary
#define LS_SYMB MO(SYMB)
// One shots
#define LS_QTUR OSL(QTUR) // For Turkish characters layer on Qwerty
#define LS_CTUR OSL(CTUR) // For Turkish characters layer on Colemak
// Toggling layers where mod-taps are removed from letter keys
#define LS_QLET TT(QLET)
#define LS_CLET TT(CLET)
// Toggling Colemak on / off
#define LS_QWER TG(QWER)
