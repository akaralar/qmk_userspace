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

void caps_lock_on() {
    caps_lock_active = true;
}

void caps_lock_off() {
    caps_lock_active = false;
}

void caps_lock_toggle(void) {
    if (caps_lock_active) {
        caps_lock_off();
    } else {
        caps_lock_on();
    }
}

bool is_caps_lock_on(void) { return caps_lock_active; }
