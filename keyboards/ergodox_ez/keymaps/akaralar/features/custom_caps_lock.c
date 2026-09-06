#include "custom_caps_lock.h"

static bool caps_lock_active = false;

bool process_custom_caps_lock(uint16_t keycode, keyrecord_t* record) {
    if (!record->event.pressed) { return true; }

    if (is_caps_lock_on() && !is_caps_word_on()) {
        switch (keycode) {
            case QK_MOD_TAP ... QK_MOD_TAP_MAX:
                if (record->tap.count != 0) { // Mod-tap key is tapped.
                    keycode = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
                }
                break;
#ifndef NO_ACTION_LAYER
            case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
                if (record->tap.count != 0) {  // Layer-tap key is held.
                    keycode = QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
                }
                break;
#endif  // NO_ACTION_LAYER
        }

        uint8_t mods = get_mods() | get_oneshot_mods();

        if (mods & MOD_MASK_GUI
            || mods & MOD_MASK_ALT
            || mods & MOD_MASK_CTRL
        ) {
            return true;
        }

        switch (keycode) {
            case KC_A ... KC_Z:
                caps_word_on();
                break;
            default:
                break;
        }
    }
    return true;
}

bool is_caps_lock_on(void) { return caps_lock_active; }

void toggle_caps_lock(void) {
    caps_lock_active = !caps_lock_active;
    if (caps_lock_active) {
        caps_word_on();
    } else {
        caps_word_off();
    }
}

// QMK weak override: decide which keys keep Caps Word alive.
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
            add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_MINS:
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
            // When caps lock is enabled, we stop caps word on any key other
            // than A-Z, to be re-enabled later when another A-Z key is pressed
            return !is_caps_lock_on();

        default:
            return false;  // Deactivate Caps Word.
    }
}
