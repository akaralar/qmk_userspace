#include "tap_hold.h"
#include "symbol_layer.h" // for LS_SNUM, referenced by IS_LAYER_TAP

// Helper for "real" layer switching keys. Since a bunch of fake layer switching
// keys are used for macros, we can't use QK_LAYER_TAP_MAX and we want to be
// able to test against real layer switching keys when checking if a keycode is
// a layer tap.
#define IS_LAYER_TAP(code) ((code) == LS_NAVI \
                            || (code) == LS_MOUS \
                            || (code) == LS_MDIA \
                            || (code) == LS_NUMB \
                            || (code) == LS_SYMB \
                            || (code) == LS_SNUM \
                            || (code) == LS_FUNC \
                            || (code) == LS_QLET \
                            || (code) == LS_QTUR \
                            || (code) == LS_QWER \
                            || (code) == LS_CLET \
                            || (code) == LS_CTUR)

//------------------------------------------------------------------------------
// Mod-tap settings
//------------------------------------------------------------------------------
#ifndef DYNAMIC_TAPPING_TERM_ENABLE
#define DYNAMIC_TAPPING_TERM_INCREMENT 0
static uint16_t g_tapping_term = TAPPING_TERM;
#endif
static uint16_t index_tap_term_diff = 25;
static uint16_t ring_pinky_tap_term_diff = 15;

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    // Give a little bit of time to the thumb space key
    if (keycode == LS_NAVI) {
        return g_tapping_term + 25;
    }

    // Make tapping term much shorter for shift mod tap keys
    switch (keycode) {
        case MT_Q_F:
        case MT_Q_J:
        case MT_C_T:
        case MT_C_N:
            return g_tapping_term - index_tap_term_diff;
    }

    // Otherwise, only consider alpha keys block
    if (record->event.key.col > 3) {
        return g_tapping_term;
    }

    switch (record->event.key.row) {
        // Increase tapping term for ring and pinky fingers
        case 0 ... 2:
        case 11 ... 13:
            return g_tapping_term + ring_pinky_tap_term_diff;
        default:
            return g_tapping_term;
    }
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    // Apply permissive hold to layer switching keys
    if (IS_LAYER_TAP(keycode)) { return true; }

    switch (keycode) {
        // Apply permissive hold to shift and cmd
        // Qwerty shift and cmd mod-taps
        case MT_Q_D:
        case MT_Q_F:
        case MT_Q_J:
        case MT_Q_K:
        // Colemak shift and cmd mod-taps
        case MT_C_S:
        case MT_C_T:
        case MT_C_N:
        case MT_C_E:
            return true;
        default:
            return false;
    }
};

//------------------------------------------------------------------------------
// Chordal hold
//------------------------------------------------------------------------------
bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record,
                      uint16_t other_keycode, keyrecord_t* other_record) {
    // Disable chordal hold when we are in the symbol layer.
    if (get_highest_layer(layer_state) == SYMB) {
        return true;
    }

    // Disable chordal hold for layer switch keys, mainly to get
    // around streak timeout during fast typing.
    if (IS_LAYER_TAP(tap_hold_keycode)) {
        return true;
    }

    // Allow same-hand holds for thumb keys
    if (other_record->event.key.col >= 4) {
        return true;
    }

    // Otherwise defer to the opposite hands rule.
    return get_chordal_hold_default(tap_hold_record, other_record);
}

//------------------------------------------------------------------------------
// Flow tap
//------------------------------------------------------------------------------
bool is_flow_tap_key(uint16_t keycode) {
    // Note: Flow Tap is intentionally left active while a hotkey mod
    // (Ctrl/Cmd/Alt) is held. Disabling it there was tried and reverted - it
    // works better in practice - so there is deliberately no mod guard here.

    // Disable Flow Tap if not on home row layer
    if (get_highest_layer(layer_state) != COLE && get_highest_layer(layer_state) != QWER) {
        return false;
    }

    // Disable streak detection for layer-tap keys.
    if (IS_LAYER_TAP(keycode)) {
        return false;
    }

    // Disable streak detection for shift mod tap keys and right cmd for copy paste
    if (IS_QK_MOD_TAP(keycode)) {
        uint8_t mods = QK_MOD_TAP_GET_MODS(keycode);
        if (mods == MOD_LSFT || mods == MOD_RSFT || mods == MOD_RGUI) {
            return false;
        }
    }

    switch (get_tap_keycode(keycode)) {
        case KC_SPC:
        case KC_A ... KC_Z:
        case KC_DOT:
        case KC_COMM:
        case KC_QUOT:
        case KC_SLSH:
            return true;
    }

    return false;
}

uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t* record,
                           uint16_t prev_keycode) {
    // Disable check for Cmd + C and Cmd + V
    if ((prev_keycode == MT_Q_K || prev_keycode == MT_C_E)
        && (keycode == KC_V || keycode == KC_C)
    ) {
        return 0;
    }

    // A short streak detection timeout for Space layer-tap key
    if (keycode == LS_NAVI) {
        return 0;
    }

    // The term is decided by the current key alone. The previous key in the
    // streak is intentionally ignored here - guarding on is_flow_tap_key(
    // prev_keycode) as well was tried and dropped on purpose.
    if (is_flow_tap_key(keycode)) {
        return FLOW_TAP_TERM;
    }

    return 0;
}

//------------------------------------------------------------------------------
// Speculative hold
//------------------------------------------------------------------------------
bool get_speculative_hold(uint16_t keycode, keyrecord_t* record) {
    if (IS_QK_MOD_TAP(keycode)) {
        return true;
    }

    return false; // Disable otherwise.
}
