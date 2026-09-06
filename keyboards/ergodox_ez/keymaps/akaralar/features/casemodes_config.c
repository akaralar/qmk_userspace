#include "casemodes_config.h"
#include "casemodes.h" // for enable_xcase_with
#include "tap_hold.h"  // for LS_MDIA

static enum case_mode case_mode = CASE_NONE;

enum case_mode get_case_mode(void) {
    return case_mode;
}

void enable_camel_case(void) {
    enable_xcase_with(OSM(MOD_LSFT));
    case_mode = CASE_CAMEL;
}

void enable_snake_case(void) {
    enable_xcase_with(KC_UNDS);
    case_mode = CASE_SNAKE;
}

void enable_kebab_case(void) {
    enable_xcase_with(KC_MINS);
    case_mode = CASE_KEBAB;
}

// Weak override from casemodes.c: decide which keys keep a case mode alive.
bool terminate_case_modes(uint16_t keycode, const keyrecord_t *record) {
    switch (keycode) {
        // Keycodes to ignore (don't disable case modes)
        case KC_A ... KC_Z:
        case KC_1 ... KC_0:
        case KC_MINS:
        case KC_UNDS:
        case KC_BSPC:
        case KC_DEL:
        case CM_TOGL:
        // We use Esc key to exit Case Modes but we don't want to send Esc to
        // the system. If we do not add the Esc key to the list of keys to be
        // ignored by Case Modes, when case modes processes Esc key, it exits
        // but sends the key as well.
        // So we ignore the key here but later when we handle custom keycodes in
        // `process_other_keycodes` we disable Case Modes manually.
        case LS_MDIA:
        case KC_ESC:
            // If mod chording disable the mods
            if (record->event.pressed && (get_mods() != 0)) {
                return true;
            }
            break;
        default:
            if (record->event.pressed) {
                return true;
            }
            break;
    }
    return false;
}

bool process_casemodes_config(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case CM_TOGL:
            if (record->event.pressed) {
                const uint8_t mods = get_mods();
                const uint8_t oneshot_mods = get_oneshot_mods();

                if ((mods | oneshot_mods) & MOD_MASK_SHIFT) {
                    // Shift held, activate snake case
                    unregister_mods(MOD_MASK_GUI);
                    del_oneshot_mods(MOD_MASK_GUI);
                    enable_snake_case();
                    register_mods(mods);
                } else if ((mods | oneshot_mods) & MOD_MASK_GUI) {
                    // CMD held, activate kebab case
                    unregister_mods(MOD_MASK_ALT);
                    del_oneshot_mods(MOD_MASK_ALT);
                    enable_kebab_case();
                    register_mods(mods);
                } else {
                    // No mod or other mods held, activate camel case
                    unregister_mods(MOD_MASK_SHIFT);
                    del_oneshot_mods(MOD_MASK_SHIFT);
                    enable_camel_case();
                    register_mods(mods);
                }
            }
            return false;
        default:
            return true;
    }
}
