#include "custom_caps_lock.h"

static bool caps_lock_active = false;

bool process_custom_caps_lock(uint16_t keycode, keyrecord_t* record) {
    if (!record->event.pressed) { return true; }

    if (is_caps_lock_on() && !is_caps_word_on()) {
        // Resolve mod-tap / layer-tap keys to their tap keycode when tapped, so
        // the A-Z check below sees the underlying letter. On a hold, tap.count
        // is 0 and the key keeps its MT/LT keycode (which is not A-Z), so Caps
        // Word is left off.
        if (record->tap.count != 0) {
            keycode = get_tap_keycode(keycode);
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
