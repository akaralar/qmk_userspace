/* Copyright 2022 Ahmet Karalar (@akaralar)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "action.h"
#include "action_layer.h"
#include "modifiers.h"
#include "oryx.h"
#include "quantum.h"
#include QMK_KEYBOARD_H
#include "version.h"

// For more info about custom shift keys, see https://getreuer.info/posts/keyboards/custom-shift-keys/index.html
#include "features/custom_shift_keys.h"

// For more info about casemodes, see https://github.com/andrewjrae/kyria-keymap/
#include "features/casemodes.h"

#include "features/custom_caps_lock.h"

// Shared layer / custom-keycode definitions and the symbol-layer fake LT feature
#include "features/common.h"
#include "features/symbol_layer.h"
#include "features/layer_rgb.h"
#include "features/tap_hold.h"

#ifdef CONSOLE_ENABLE
#include "features/debug_helper.h"
#endif

//------------------------------------------------------------------------------
// Keycodes
//------------------------------------------------------------------------------
// The custom-keycode enum (C_keycodes) lives in features/common.h so feature
// modules can share it.

// Custom modifiers in single key
#define KC_CSG LCTL(LSFT(KC_LEFT_GUI))

// The mod-tap keys (MT_*) are defined in features/tap_hold.h, alongside the
// tap/hold tuning that governs their behavior.

// One-shot modifiers
#define OS_LSFT OSM(MOD_LSFT)
#define OS_LCMD OSM(MOD_LGUI)
#define OS_LOPT OSM(MOD_LALT)
#define OS_LCTL OSM(MOD_LCTL)
#define OS_RSFT OSM(MOD_RSFT)
#define OS_RCMD OSM(MOD_RGUI)
#define OS_ROPT OSM(MOD_RALT)
#define OS_RCTL OSM(MOD_RCTL)

// macOS keycodes
#define KC_CUT LGUI(KC_X)
#define KC_COPY LGUI(KC_C)
#define KC_PSTE LGUI(KC_V)
#define KC_UNDO LGUI(KC_Z)
#define KC_REDO LGUI(LSFT(KC_Z))
#define KC_FN KC_CAPS // caps lock is converted to fn key in the OS

// Application shortcuts
#define MOOM LALT(LCTL(LSFT(KC_GRAVE))) // Moom
#define ALF_SEA LGUI(LALT(KC_SPC))      // Alfred search
#define ALF_NAV LGUI(LCTL(KC_SLASH))    // Alfred file navigation
#define ALF_ACT RGUI(RCTL(KC_BSLS))     // Alfred action selection
#define ONEP_QA LALT(LGUI(KC_BSLS))     // 1password quick access
#define ONEP_AF LGUI(LSFT(KC_BSLS))     // 1password autofill
#define TH_QE MEH(KC_T)                 // Things quick entry
#define TH_QEAF HYPR(KC_T)              // Things quick entry with autofill
#define EDT_SCR LGUI(LSFT(KC_4))        // Show edit screenshot tool
#define PRT_SCR LGUI(LSFT(KC_5))        // Show screenshot tool
#define REC_OPT LGUI(LSFT(KC_6))        // Show recording options tool


//------------------------------------------------------------------------------
// Layers and layer keycodes
//------------------------------------------------------------------------------
// The layer enum lives in features/common.h so feature modules can share it.

// The layer-switch keys (LS_*) are defined in features/tap_hold.h, and the
// IS_LAYER_TAP helper that enumerates them is private to that module.

// The fake layer-tap keys (FT_*) are defined in features/symbol_layer.h,
// alongside the code that handles their tap/hold behavior.

//------------------------------------------------------------------------------
// Custom shift keys
//------------------------------------------------------------------------------
const custom_shift_key_t custom_shift_keys[] = {
  {LS_NUMB , KC_DEL}, // Shift + LT Backspace is delete
  {KC_BSPC , KC_DEL}, // Shift + Normal backspace is delete
};

uint8_t NUM_CUSTOM_SHIFT_KEYS =
    sizeof(custom_shift_keys) / sizeof(custom_shift_key_t);

//------------------------------------------------------------------------------
// Caps Word
//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
// Casemodes
//------------------------------------------------------------------------------
enum case_mode {
    CASE_SNAKE,
    CASE_KEBAB,
    CASE_CAMEL,
};

static enum case_mode case_mode = -1;

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

static void enable_camel_case(void) {
    enable_xcase_with(OS_LSFT);
    case_mode = CASE_CAMEL;
}

static void enable_snake_case(void) {
    enable_xcase_with(KC_UNDS);
    case_mode = CASE_SNAKE;
}

static void enable_kebab_case(void) {
    enable_xcase_with(KC_MINS);
    case_mode = CASE_KEBAB;
}


//------------------------------------------------------------------------------
// Caps lock
//------------------------------------------------------------------------------
static void toggle_caps_lock(void) {
    caps_lock_toggle();
    if (is_caps_lock_on()) {
        caps_word_on();
    } else {
        caps_word_off();
    }
}

//------------------------------------------------------------------------------
// Combos
//------------------------------------------------------------------------------
#ifdef COMBO_ENABLE
enum combo_events {
  CAPS_WORD,
  CAPS_LOCK,
  CAMEL_CASE,
  SNAKE_CASE,
  KEBAB_CASE
};

const uint16_t PROGMEM caps_word_combo[] = {KC_D, KC_H, COMBO_END};
const uint16_t PROGMEM caps_lock_combo[] = {KC_G, KC_M, COMBO_END};
const uint16_t PROGMEM camel_case_combo[] = {KC_C, KC_COMM, COMBO_END};
const uint16_t PROGMEM snake_case_combo[] = {KC_X, KC_DOT, COMBO_END};
const uint16_t PROGMEM kebab_case_combo[] = {KC_Z, KC_SLSH, COMBO_END};

combo_t key_combos[] = {
  [CAPS_WORD] = COMBO_ACTION(caps_word_combo),
  [CAPS_LOCK] = COMBO_ACTION(caps_lock_combo),
  [CAMEL_CASE] = COMBO_ACTION(camel_case_combo),
  [SNAKE_CASE] = COMBO_ACTION(snake_case_combo),
  [KEBAB_CASE] = COMBO_ACTION(kebab_case_combo),
};

void process_combo_event(uint16_t combo_index, bool pressed) {
    switch (combo_index) {
        case CAPS_WORD:
            if (pressed) {
                caps_word_on();
            }
            break;
        case CAPS_LOCK:
            if (pressed) {
                toggle_caps_lock();
            }
            break;
        case CAMEL_CASE:
            if (pressed) {
                enable_camel_case();
            }
            break;
        case SNAKE_CASE:
            if (pressed) {
                enable_snake_case();
            }
            break;
        case KEBAB_CASE:
            if (pressed) {
                enable_kebab_case();
            }
            break;
    }
}
#endif

//------------------------------------------------------------------------------
// Custom keycode handling
//------------------------------------------------------------------------------
static void send_string_if_enabled(const char *string) {
#ifdef SEND_STRING_ENABLE
    send_string(string);
#endif
}

static bool process_casemodes_keycode(uint16_t keycode, keyrecord_t *record) {
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

typedef struct {
    uint16_t diacritic_dead_key;
    uint16_t key_to_add_diacritic;
} turkish_diacritic_key;

static const turkish_diacritic_key turkish_diacritic_keys[] = {
    {KC_C, KC_C},
    {KC_B, KC_G},
    {KC_W, KC_I},
    {KC_U, KC_O},
    {KC_C, KC_S},
    {KC_U, KC_U},
};

static bool process_tr_letter_keycodes(uint16_t keycode, keyrecord_t *record) {
    if (keycode < TC_C || keycode > TC_U) {
        return true;
    }

    if (record->event.pressed) {
        uint8_t mods = get_mods();
        uint8_t oneshot_mods = get_oneshot_mods();
        uint8_t weak_mods = get_weak_mods();

        clear_mods();
        clear_oneshot_mods();
        clear_weak_mods();

        turkish_diacritic_key keys = turkish_diacritic_keys[keycode - TC_C];
        tap_code16(LALT(keys.diacritic_dead_key));

        if (((mods | oneshot_mods | weak_mods) & MOD_MASK_SHIFT)
            || is_caps_lock_on()
            || is_caps_word_on()
        ) {
            tap_code16(LSFT(keys.key_to_add_diacritic));
        } else {
            tap_code16(keys.key_to_add_diacritic);
        }
    }

    return false;
}

static bool should_swallow_esc_release = false;
static bool process_swallowed_esc(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LS_MDIA:
            if (record->tap.count != 0) { // key is being tapped
                // Don't send escape key when trying to exit caps word, caps
                // lock or case modes
                if (record->event.pressed
                    && (
                        (is_caps_word_on() && !is_caps_lock_on())
                        || (get_xcase_state() != XCASE_OFF)
                    )
                ) {
                    caps_word_off();
                    disable_xcase();
                    should_swallow_esc_release = true;
                    return false; // skip default handling
                }
            }

            // We should also swallow key release record
            if (should_swallow_esc_release && !record->event.pressed) {
                should_swallow_esc_release = false;
                return false; // skip default handling
            }
    }

    return true; // otherwise continue with default handling
}

static bool process_other_keycodes(uint16_t keycode, keyrecord_t *record) {
    // Handle if keycode is "casemodes" keycode
    if (!process_casemodes_keycode(keycode, record)) { return false; }

    // Handle Esc when it's being used to exit Caps Word or Case Modes
    if (!process_swallowed_esc(keycode, record)) { return false; }

#ifdef RGB_MATRIX_ENABLE
    // Process RGB Matrix keycodes
    if (!process_rgb_matrix_keycodes(keycode, record)) { return false; }
#endif

    switch (keycode) {
        case VRSN:
            if (record->event.pressed) {
                const char* str = QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION;
                send_string_if_enabled(str);
            }
            return false;
        case CPS_LCK:
            if (record->event.pressed) {
                toggle_caps_lock();
            }
            return false;
        default:
            break;
    }

    return true;
};

//------------------------------------------------------------------------------
// LED lights
//------------------------------------------------------------------------------
static void led_state_set(layer_state_t state) {
    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();

    uint8_t layer = get_highest_layer(layer_state);
    switch (layer) {
        case NAVI: {
            ergodox_right_led_1_on();
            break;
        }
        case MOUS: {
            ergodox_right_led_2_on();
            break;
        }
        case MDIA: {
            ergodox_right_led_3_on();
            break;
        }
        case NUMB: {
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            break;
        }
        case SYMB: {
            ergodox_right_led_1_on();
            ergodox_right_led_3_on();
            break;
        }
        case SNUM: {
            ergodox_right_led_2_on();
            ergodox_right_led_3_on();
            break;
        }
        case CLET:
        case QLET:
        case CTUR:
        case QTUR:
        case FUNC: {
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            ergodox_right_led_3_on();
            break;
        }
        default:
            break;
    }

    // Fix LED lights behaviour for Caps Lock and Caps Word
    // led_t led_state = host_keyboard_led_state();
    if (is_caps_lock_on()) {
        ergodox_right_led_3_on();
    } else if (is_caps_word_on()) {
        ergodox_right_led_2_on();
    }

    // Fix LED lights behaviour for case modes
    if (get_xcase_state() != XCASE_OFF) {
        switch (case_mode) {
            case CASE_CAMEL:
                ergodox_right_led_1_on();
                break;
            case CASE_SNAKE:
                ergodox_right_led_2_on();
                break;
            case CASE_KEBAB:
                ergodox_right_led_3_on();
                break;
            default:
                break;
        }
    }
};

// Fix LED lights behaviour for when other things affect LEDs (like Caps Lock &
// Caps Word and case modes)
void fix_leds_task(void) {
    led_state_set(layer_state);
};

//------------------------------------------------------------------------------
// QMK User space functions
//------------------------------------------------------------------------------
void keyboard_post_init_user(void) {
#if CONSOLE_ENABLE
    enable_debug_user();
#endif
};

void matrix_scan_user() {
    fix_leds_task();
};

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!pre_process_symbol_layer_fake_lt_keys(keycode, record)) {
        return false;
    }

    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // First process the symbol layer fake lt keys as they might be ignored.
    if (!process_symbol_layer_fake_lt_keys(keycode, record)) { return false; }

#ifdef CONSOLE_ENABLE
    prefixed_print(keycode, record, "process_record_user");
#endif

    // Process case modes after other key codes because we use Esc to quit
    // case modes but we don't want to send the escape key. If case modes
    // handles the key first, it will send the Esc key itself.
    if (!process_case_modes(keycode, record)) { return false; }

    // Pass the keycode and record to custom caps lock
    if (!process_custom_caps_lock(keycode, record)) { return false; }

    // Pass the keycode and record to custom shift keys
    if (!process_custom_shift_keys(keycode, record)) { return false; }

    // // Process keycodes for Turkish diacritics letters
    if (!process_tr_letter_keycodes(keycode, record)) { return false; }

    // Process custom keycodes defined in this file
    if (!process_other_keycodes(keycode, record)) { return false; }

    return true;
};


layer_state_t layer_state_set_user(layer_state_t state) {
    led_state_set(state);
    return state;
};

//------------------------------------------------------------------------------
// Add empty functions for Magic Keycodes to save some space
// see https://docs.qmk.fm/#/squeezing_avr?id=magic-functions
//------------------------------------------------------------------------------
uint16_t keycode_config(uint16_t keycode) {
    return keycode;
}

uint8_t mod_config(uint8_t mod) {
    return mod;
}

//------------------------------------------------------------------------------
// Keymap
//------------------------------------------------------------------------------

// clang-format off

/* Matrix positions in Column:Row format
 * ,--------------------------------------------------.
 * |   0:0  | 0:1  | 0:2  | 0:3  | 0:4  | 0:5  | 0:6  |
 * |--------+------+------+------+------+-------------|
 * |   1:0  | 1:1  | 1:2  | 1:3  | 1:4  | 1:5  | 1:6  |
 * |--------+------+------+------+------+------|      |
 * |   2:0  | 2:1  | 2:2  | 2:3  | 2:4  | 2:5  |------|
 * |--------+------+------+------+------+------| 3:6  |
 * |   3:0  | 3:1  | 3:2  | 3:3  | 3:4  | 3:5  |      |
 * `--------+------+------+------+------+-------------'
 *   | 4:0  | 4:1  | 4:2  | 4:3  | 4:4  |
 *   `----------------------------------'
 *                                      ,-------------.
 *                                      | 5:5  | 5:6  |
 *                               ,------|------|------|
 *                               |      |      | 5:4  |
 *                               | 5:3  | 5:2  |------|
 *                               |      |      | 5:1  |
 *                               `--------------------'
 * ,--------------------------------------------------.
 * |  0:7 |  0:8 |  0:9 | 0:10 | 0:11 | 0:12 | 0:13   |
 * |------+------+------+------+------+------+--------|
 * |  1:7 |  1:8 |  1:9 | 1:10 | 1:11 | 1:12 | 1:13   |
 * |      |------+------+------+------+------+--------|
 * |------|  2:8 |  2:9 | 2:10 | 2:11 | 2:12 | 2:13   |
 * |  3:7 |------+------+------+------+------+--------|
 * |      |  3:8 |  3:9 | 3:10 | 3:11 | 3:12 | 3:13   |
 * `-------------+------+------+------+------+--------'
 *               |  4:9 | 4:10 | 4:11 | 4:12 | 4:13 |
 *               `----------------------------------'
 * ,-------------.
 * |  5:7 | 5:8  |
 * |------+------+------.
 * |  5:9 |      |      |
 * |------| 5:11 | 5:10 |
 * | 5:12 |      |      |
 * `--------------------'
 */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [COLE] = LAYOUT_ergodox(
        _______, _______, _______, _______, _______, _______, _______,
        _______, KC_Q   , MT_W   , MT_C_F , MT_C_P , KC_B   , _______,
        _______, MT_A   , MT_C_R , MT_C_S , MT_C_T , KC_G   ,
        _______, KC_Z   , KC_X   , KC_C   , KC_D   , KC_V   , _______,
        _______, _______, _______, _______, LS_MDIA,
                                                     CW_TOGG, LS_QWER,
                                                              CM_TOGL,
                                            LS_NAVI, LS_MOUS, OS_LSFT,

        _______, _______, _______, _______, _______, _______, _______,
        _______, KC_J   , MT_C_L , MT_C_U , MT_C_Y , KC_QUOT, _______,
                 KC_M   , MT_C_N , MT_C_E , MT_C_I , MT_C_O , _______,
        _______, KC_K   , KC_H   , KC_COMM, KC_DOT , KC_SLSH, _______,
                          LS_SYMB, _______, _______, _______, _______,
        LS_CLET, CW_TOGG,
        KC_FN  ,
        LS_CTUR, LS_FUNC, LS_NUMB
    ),

    [CLET] = LAYOUT_ergodox(
        _______, _______, _______, _______, _______, _______, _______,
        _______, _______, KC_W   , KC_F   , KC_P   , _______, _______,
        _______, KC_A   , KC_R   , KC_S   , KC_T   , _______,
        _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,
                                                     _______, _______,
                                                              _______,
                                            _______, _______, _______,

        _______, _______, _______, _______, _______, _______, _______,
        _______, _______, KC_L   , KC_U   , KC_Y   , _______, _______,
                 _______, KC_N   , KC_E   , KC_I   , KC_O   , _______,
        _______, _______, _______, _______, _______, _______, _______,
                          _______, _______, _______, _______, _______,
        _______, _______,
        _______,
        _______, _______, _______
    ),

    [CTUR] = LAYOUT_ergodox(
        _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, TC_S   , _______, TC_G   ,
        _______, _______, _______, TC_C   , _______, _______, _______,
        _______, _______, _______, _______, _______,
                                                     _______, _______,
                                                              _______,
                                            _______, _______, _______,

        _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, TC_U   , _______, _______, _______,
                 _______, _______, _______, TC_I   , TC_O   , _______,
        _______, _______, _______, _______, _______, _______, _______,
                          _______, _______, _______, _______, _______,
        _______, _______,
        _______,
        XXXXXXX, _______, _______
    ),

    [QWER] = LAYOUT_ergodox(
        _______, _______, _______, _______, _______, _______, _______,
        _______, KC_Q   , MT_W   , MT_Q_E , MT_Q_R , KC_T   , _______,
        _______, MT_A   , MT_Q_S , MT_Q_D , MT_Q_F , KC_G   ,
        _______, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , _______,
        _______, _______, _______, _______, LS_MDIA,
                                                     _______, _______,
                                                              CM_TOGL,
                                            LS_NAVI, LS_MOUS, OS_LSFT,

        _______, _______, _______, _______, _______, _______, _______,
        _______, KC_Y   , MT_Q_U , MT_Q_I , MT_Q_O , KC_P   , _______,
                 KC_H   , MT_Q_J , MT_Q_K , MT_Q_L , MT_Q_QT, _______,
        _______, KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, _______,
                          LS_SYMB, _______, _______, _______, _______,
        LS_QLET, _______,
        KC_FN  ,
        LS_QTUR, LS_FUNC, LS_NUMB
    ),

    [QLET] = LAYOUT_ergodox(
        _______, _______, _______, _______, _______, _______, _______,
        _______, _______, KC_W   , KC_E   , KC_R   , _______, _______,
        _______, KC_A   , KC_S   , KC_D   , KC_F   , _______,
        _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,
                                                     _______, _______,
                                                              _______,
                                            _______, _______, _______,

        _______, _______, _______, _______, _______, _______, _______,
        _______, _______, KC_U   , KC_I   , KC_O   , _______, _______,
                 _______, KC_J   , KC_K   , KC_L   , KC_QUOT, _______,
        _______, _______, _______, _______, _______, _______, _______,
                          _______, _______, _______, _______, _______,
        _______, _______,
        _______,
        _______, _______, _______
    ),

    [QTUR] = LAYOUT_ergodox(
        _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______,
        _______, _______, TC_S   , _______, _______, TC_G   ,
        _______, _______, _______, TC_C   , _______, _______, _______,
        _______, _______, _______, _______, _______,
                                                     _______, _______,
                                                              _______,
                                            _______, _______, _______,

        _______, _______, _______, _______, _______, _______, _______,
        _______, _______, TC_U   , TC_I   , TC_O   , _______, _______,
                 _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______,
                          _______, _______, _______, _______, _______,
        _______, _______,
        _______,
        XXXXXXX, _______, _______
    ),

    [NAVI] = LAYOUT_ergodox(
        _______, _______, _______, _______, _______, _______, _______,
        _______, XXXXXXX, KC_CSG , KC_MEH , KC_HYPR, XXXXXXX, _______,
        _______, KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, ALF_SEA,
        _______, KC_UNDO, KC_CUT , KC_COPY, KC_PSTE, KC_REDO, _______,
        _______, _______, _______, _______, XXXXXXX,
                                                     _______, _______,
                                                              _______,
                                           _______ , XXXXXXX, _______,

        _______, _______, _______, _______, _______, _______, _______,
        _______, KC_PGUP, KC_HOME, KC_UP  , KC_END , KC_INS , _______,
                 KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, CPS_LCK, _______,
        _______, KC_REDO, KC_PSTE, KC_COPY, KC_CUT , KC_UNDO, _______,
                          _______, _______, _______, _______, _______,
        _______, _______,
        _______,
        _______, KC_ENT , KC_BSPC
    ),

    [MOUS] = LAYOUT_ergodox(
        _______, _______, _______, _______, _______, _______, _______,
        _______, TH_QEAF, TH_QE  , ALF_NAV, ALF_ACT, MOOM   , _______,
        _______, OS_LCTL, OS_LOPT, OS_LCMD, OS_LSFT, ONEP_QA,
        _______, XXXXXXX, XXXXXXX, REC_OPT, EDT_SCR, PRT_SCR, _______,
        _______, _______, _______, _______, _______,
                                                     _______, _______,
                                                              _______,
                                            XXXXXXX, _______, _______,

        _______, _______, _______, _______, _______, _______, _______,
        _______, MS_WHLU, MS_WHLL,   MS_UP, MS_WHLR, XXXXXXX, _______,
                 MS_WHLD, MS_LEFT, MS_DOWN, MS_RGHT, XXXXXXX, _______,
        _______, KC_REDO, KC_PSTE, KC_COPY, KC_CUT , KC_UNDO, _______,
                          MS_BTN2, _______, _______, _______, _______,
        _______, _______,
        _______,
        _______, MS_BTN1, MS_BTN3
    ),

    [MDIA] = LAYOUT_ergodox(
        _______, _______, _______, _______, _______, _______, _______,
        _______, QK_BOOT, XXXXXXX, XXXXXXX, DM_REC1, DM_PLY1, _______,
        _______, KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, DM_RSTP,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, DM_REC2, DM_PLY2, _______,
        _______, _______, _______, _______, _______,
                                                     _______, _______,
                                                              _______,
                                            XXXXXXX, XXXXXXX, _______,

        _______, _______, _______, _______, _______, _______, _______,
        _______, KC_MNXT, KC_VOLU, KC_BRIU, RGB_BUP, XXXXXXX, _______,
                 KC_MPRV, KC_VOLD, KC_BRID, RGB_BDN, XXXXXXX, _______,
        _______, VRSN   , KC_MUTE, XXXXXXX, RGB_TGL, XXXXXXX, _______,
                          XXXXXXX, _______, _______, _______, _______,
        _______, _______,
        _______,
        _______, KC_MSTP, KC_MPLY
    ),

    [NUMB] = LAYOUT_ergodox(
        _______, _______, _______, _______, _______, _______, _______,
        _______, KC_LBRC, KC_7   , KC_8   , KC_9   , KC_RBRC, _______,
        _______, KC_SCLN, KC_4   , KC_5   , KC_6   , KC_EQL ,
        _______, KC_GRV , KC_1   , KC_2   , KC_3   , KC_BSLS, _______,
        _______, _______, _______, _______, KC_0   ,
                                                     _______, _______,
                                                              _______,
                                            _______, KC_MINS, _______,

        _______, _______, _______, _______, _______, _______, _______,
        _______, XXXXXXX, KC_HYPR, KC_MEH , KC_CSG , XXXXXXX, _______,
                 XXXXXXX, KC_RSFT, KC_RGUI, KC_LALT, KC_RCTL, _______,
        _______, XXXXXXX, XXXXXXX, _______, _______, _______, _______,
                          XXXXXXX, _______, _______, _______, _______,
        _______, _______,
        _______,
        _______, XXXXXXX, _______
    ),

    [SYMB] = LAYOUT_ergodox(
        _______, _______, _______, _______, _______, _______, _______,
        _______, KC_TILD, KC_PLUS, FT_LBRC, KC_RBRC, FT_CBLS, _______,
        _______, FT_UNDS, FT_SLSH, FT_LPRN, KC_RPRN, FT_CBL ,
        _______, KC_DLR , KC_QUES, FT_LABK, KC_RABK, FT_GRV , _______,
        _______, _______, _______, _______, KC_AT  ,
                                                     _______, _______,
                                                              _______,
                                            _______, KC_DOT , _______,

        _______, _______, _______, _______, _______, _______, _______,
        _______, KC_CIRC, KC_BSLS, FT_DQUO, FT_ASTR, KC_PERC, _______,
                 KC_PIPE, LS_SNUM, FT_RCBR, KC_COLN, KC_COMM, _______,
        _______, FT_QUOT, KC_EQL , KC_MINS, KC_EXLM, KC_SCLN, _______,
                          XXXXXXX, _______, _______, _______, _______,
        _______, _______,
        _______,
        _______, _______, XXXXXXX
    ),

    [SNUM] = LAYOUT_ergodox(
        _______, _______, _______, _______, _______, _______, _______,
        _______, XXXXXXX, KC_7   , KC_8   , KC_9   , XXXXXXX, _______,
        _______, XXXXXXX, KC_4   , KC_5   , KC_6   , XXXXXXX,
        _______, XXXXXXX, KC_1   , KC_2   , KC_3   , XXXXXXX, _______,
        _______, _______, _______, _______, KC_0   ,
                                                     _______, _______,
                                                              _______,
                                            _______, KC_DOT , _______,

        _______, _______, _______, _______, _______, _______, _______,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
                 XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, _______,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
                          _______, _______, _______, _______, _______,
        _______, _______,
        _______,
        _______, _______, _______
    ),

    [FUNC] = LAYOUT_ergodox(
        _______, _______, _______, _______, _______, _______, _______,
        _______, KC_F12 , KC_F7  , KC_F8  , KC_F9  , XXXXXXX, _______,
        _______, KC_F11 , KC_F4  , KC_F5  , KC_F6  , XXXXXXX,
        _______, KC_F10 , KC_F1  , KC_F2  , KC_F3  , XXXXXXX, _______,
        _______, _______, _______, _______, KC_ESC ,
                                                     _______, _______,
                                                              _______,
                                            KC_SPC , KC_TAB , _______,

        _______, _______, _______, _______, _______, _______, _______,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
                 XXXXXXX, OS_RSFT, OS_RCMD, OS_ROPT, OS_RCTL, _______,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
                          _______, _______, _______, _______, _______,
        _______, _______,
        _______,
        _______, XXXXXXX, XXXXXXX
    )
};

// clang-format on
