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

#include QMK_KEYBOARD_H
#include "version.h"

// For more info about achordion, see https://getreuer.info/posts/keyboards/achordion/index.html
#include "features/achordion.h"
// For more info about custom shift keys, see https://getreuer.info/posts/keyboards/custom-shift-keys/index.html
#include "features/custom_shift_keys.h"

enum layers {
    BASE, // default layer
    NAVI, // navigation layer
    MOUS, // mouse layer
    MDIA, // media keys layer
    NUMB, // numbers layer
    SYMB, // code symbols layer
    SNUM, // numbers from symbols layer
    FUNC, // Function keys layer
};

enum C_keycodes {
#ifdef ORYX_CONFIGURATOR
    VRSN = EZ_SAFE_RANGE,
#else
    VRSN = SAFE_RANGE,
#endif
    RGB_SLD,
};

// macOS keycodes
#define KC_CUT LGUI(KC_X)
#define KC_COPY LGUI(KC_C)
#define KC_PSTE LGUI(KC_V)
#define KC_UNDO LGUI(KC_Z)
#define KC_REDO LGUI(LSFT(KC_Z))
#define KC_FN KC_F24 // F24 is converted to fn key with karabiner

// mod-tap keys
#define MT_E MEH_T(KC_E)
#define MT_R ALL_T(KC_R)
#define MT_U ALL_T(KC_U)
#define MT_I MEH_T(KC_I)
#define MT_F MT(MOD_LSFT, KC_F)
#define MT_D MT(MOD_LGUI, KC_D)
#define MT_S MT(MOD_LALT, KC_S)
#define MT_A MT(MOD_LCTL, KC_A)
#define MT_J MT(MOD_RSFT, KC_J)
#define MT_K MT(MOD_RGUI, KC_K)
#define MT_L MT(MOD_LALT, KC_L)
#define MT_QUOT MT(MOD_RCTL, KC_QUOTE)

// Layer-tap keys
#define LT_NAVI LT(NAVI, KC_SPACE)
#define LT_MOUS LT(MOUS, KC_TAB)
#define LT_MDIA LT(MDIA, KC_ESCAPE)
#define LT_NUMB LT(NUMB, KC_BSPC)
#define LT_SNUM LT(SNUM, KC_EQUAL)
#define LT_FUNC LT(FUNC, KC_ENTER)

// Layer switching
#define LS_SYMB MO(SYMB)

// ZSA specific keys
#define ZSA_USB WEBUSB_PAIR
#define ZSA_TOG TOGGLE_LAYER_COLOR

// One-shot modifiers
#define OS_LSFT OSM(MOD_LSFT)

// Application shortcuts
#define MOOM LALT(LCTL(LSFT(KC_GRAVE))) // Moom
#define ALF_NAV LGUI(LCTL(KC_SLASH))    // Alfred file navigation
#define ALF_ACT RGUI(RCTL(KC_BSLS))     // Alfred action selection
#define ONEP_QA LALT(LGUI(KC_BSLS))     // 1password quick access
#define ONEP_AF LGUI(LSFT(KC_BSLS))     // 1password autofill
#define TH_QE MEH(KC_T)                 // Things quick entry
#define TH_QEAF HYPR(KC_T)              // Things quick entry with autofill

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
    [BASE] = LAYOUT_ergodox(
        _______, _______, _______, _______, _______, _______, _______,
        _______, KC_Q   , KC_W   , MT_E   , MT_R   , KC_T   , _______,
        _______, MT_A   , MT_S   , MT_D   , MT_F   , KC_G   ,
        _______, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , _______,
        _______, _______, _______, _______, LT_MDIA,
                                                     _______, _______,
                                                              _______,
                                            LT_NAVI, LT_MOUS, OS_LSFT,

        _______, _______, _______, _______, _______, _______, _______,
        _______, KC_Y   , MT_U   , MT_I   , KC_O   , KC_P   , _______,
                 KC_H   , MT_J   , MT_K   , MT_L   , MT_QUOT, _______,
        _______, KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, _______,
                          LS_SYMB, _______, _______, _______, _______,
        _______, _______,
        KC_FN  ,
        KC_DEL , LT_FUNC, LT_NUMB
    ),

    [NAVI] = LAYOUT_ergodox(
        _______, _______, _______, _______, _______, _______, _______,
        _______, XXXXXXX, XXXXXXX, KC_MEH , KC_HYPR, XXXXXXX, _______,
        _______, KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, XXXXXXX,
        _______, KC_UNDO, KC_CUT , KC_COPY, KC_PSTE, KC_REDO, _______,
        _______, _______, _______, _______, XXXXXXX,
                                                     _______, _______,
                                                              _______,
                                           _______ , XXXXXXX, _______,

        _______, _______, _______, _______, _______, _______, _______,
        _______, KC_PGUP, KC_HOME, KC_UP  , KC_END , KC_INS , _______,
                 KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_CAPS, _______,
        _______, KC_REDO, KC_PSTE, KC_COPY, KC_CUT , KC_UNDO, _______,
                          _______, _______, _______, _______, _______,
        _______, _______,
        _______,
        _______, KC_ENT , KC_BSPC
    ),

    [MOUS] = LAYOUT_ergodox(
        _______, _______, _______, _______, _______, _______, _______,
        _______, XXXXXXX, XXXXXXX, ALF_ACT, ALF_NAV, MOOM   , _______,
        _______, KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, ONEP_QA,
        _______, XXXXXXX, XXXXXXX, TH_QE  , TH_QEAF, ONEP_AF, _______,
        _______, _______, _______, _______, _______,
                                                     _______, _______,
                                                              _______,
                                            XXXXXXX, _______, _______,

        _______, _______, _______, _______, _______, _______, _______,
        _______, KC_WH_U, KC_WH_L, KC_MS_U, KC_WH_R, XXXXXXX, _______,
                 KC_WH_D, KC_MS_L, KC_MS_D, KC_MS_R, XXXXXXX, _______,
        _______, KC_REDO, KC_PSTE, KC_COPY, KC_CUT , KC_UNDO, _______,
                          KC_BTN2, _______, _______, _______, _______,
        _______, _______,
        _______,
        _______, KC_BTN1, KC_BTN3
    ),

    [MDIA] = LAYOUT_ergodox(
        _______, _______, _______, _______, _______, _______, _______,
        _______, QK_BOOT, ZSA_USB, XXXXXXX, DM_REC1, DM_PLY1, _______,
        _______, KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, DM_RSTP,
        _______, VRSN   , XXXXXXX, XXXXXXX, DM_REC2, DM_PLY2, _______,
        _______, _______, _______, _______, _______,
                                                     _______, _______,
                                                              _______,
                                            XXXXXXX, XXXXXXX, _______,

        _______, _______, _______, _______, _______, _______, _______,
        _______, RGB_TOG, KC_BRIU, RGB_HUI, RGB_SAI, RGB_VAI, _______,
                 ZSA_TOG, KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, _______,
        _______, RGB_MOD, KC_BRID, RGB_HUD, RGB_SAD, RGB_VAD, _______,
                          KC_MUTE, _______, _______, _______, _______,
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
                                           _______, KC_MINUS, _______,

        _______, _______, _______, _______, _______, _______, _______,
        _______, XXXXXXX, KC_HYPR, KC_MEH , KC_RALT, XXXXXXX, _______,
                 XXXXXXX, KC_RSFT, KC_RGUI, KC_LALT, KC_RCTL, _______,
        _______, XXXXXXX, XXXXXXX, _______, _______, _______, _______,
                          XXXXXXX, _______, _______, _______, _______,
        _______, _______,
        _______,
        _______, XXXXXXX, _______
    ),

    [SYMB] = LAYOUT_ergodox(
        _______, _______, _______, _______, _______, _______, _______,
        _______, KC_TILD, KC_PLUS, KC_LBRC, KC_RBRC, KC_HASH, _______,
        _______, KC_UNDS, KC_SLSH, KC_LPRN, KC_RPRN, KC_AMPR,
        _______, KC_DLR , KC_QUES, KC_LABK, KC_RABK, KC_GRV , _______,
        _______, _______, _______, _______, KC_AT  ,
                                                     _______, _______,
                                                              _______,
                                            _______, KC_DOT , _______,

        _______, _______, _______, _______, _______, _______, _______,
        _______, KC_CIRC, KC_BSLS, KC_DQUO, KC_ASTR, KC_PERC, _______,
                 KC_PIPE, KC_RCBR, KC_LCBR, KC_COLN, KC_COMM, _______,
        _______, KC_QUOT, LT_SNUM, KC_MINS, KC_EXLM, KC_SCLN, _______,
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
        _______, XXXXXXX, DT_UP  , XXXXXXX, XXXXXXX, XXXXXXX, _______,
                 DT_PRNT, KC_RSFT, KC_RGUI, KC_LALT, KC_RCTL, _______,
        _______, XXXXXXX, DT_DOWN, XXXXXXX, XXXXXXX, XXXXXXX, _______,
                          _______, _______, _______, _______, _______,
        _______, _______,
        _______,
        _______, XXXXXXX, XXXXXXX
    )
};

// clang-format on

// Custom shift keys for ? and ! in the alpha layer
const custom_shift_key_t custom_shift_keys[] = {
  {KC_COMM, KC_QUES}, // Shift + , is ?
  {KC_DOT, KC_EXLM}, // Shift + . is !
  {KC_SLSH, KC_SLSH}, // Shift + / is nothing
};

uint8_t NUM_CUSTOM_SHIFT_KEYS = sizeof(custom_shift_keys) / sizeof(custom_shift_key_t);

// Mod-tap settings
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
      case MT_A:
      case MT_S:
      case MT_L:
      case MT_QUOT:
          return g_tapping_term + 15;
      default:
          return g_tapping_term;
    }
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // Apply permissive hold to layer tap keys
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
        // Apply permissive hold to shift and cmd
        case MT_D:
        case MT_F:
        case MT_J:
        case MT_K:
            return true;
        default:
            return false;
    }
}

// Achordion
bool achordion_chord(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record, uint16_t other_keycode, keyrecord_t *other_record) {
    switch (tap_hold_keycode) {
        // Disable same hand prevention with layer switching keys
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
        case LS_SYMB:
            return true;
    }

    // Also allow same-hand holds when the other key is in the rows below the
    // alphas. The column for those are 4 and 5
    if (other_record->event.key.col >= 4) {
        return true;
    }

    // Otherwise, follow the opposite hands rule.
    return achordion_opposite_hands(tap_hold_record, other_record);
}

uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
    return g_tapping_term + 200;
}

// Custom keycode handling
bool process_custom_keycodes(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case RGB_SLD:
            if (record->event.pressed) {
#ifdef RGB_MATRIX_ENABLE
                rgblight_mode(1);
#endif
            }
            return false;
        case VRSN:
            if (record->event.pressed) {
                SEND_STRING(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
                return false;
            }

        default:
            return true;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Pass the keycode and record to achordion for tap-hold decision
    if (!process_achordion(keycode, record)) { return false; }

    // Process custom shift keys
    if (!process_custom_shift_keys(keycode, record)) { return false; }

    // Process custom keycodes defined in this file
    if (!process_custom_keycodes(keycode, record)) { return false; }

    return true;
}

// LED light behaviour for Caps Lock & Caps Word
void fix_leds_task(void) {
    led_t led_state = host_keyboard_led_state();
    if (led_state.caps_lock) {
        ergodox_right_led_3_on();
    } else {
        uint8_t layer = get_highest_layer(layer_state);
        if (layer != 3 && layer != 5 && layer != 6 && layer != 7) {
            ergodox_right_led_3_off();
        }
    }

    if (is_caps_word_on()) {
        ergodox_right_led_2_on();
    } else {
        uint8_t layer = get_highest_layer(layer_state);
        if (layer != 2 && layer != 4 && layer != 6 && layer != 7) {
            ergodox_right_led_2_off();
        }
    }
}


void matrix_scan_user() {
    achordion_task();
    fix_leds_task();
}

layer_state_t layer_state_set_user(layer_state_t state) {
    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();

    uint8_t layer = get_highest_layer(state);
    switch (layer) {
        case 1:
            ergodox_right_led_1_on();
            break;
        case 2:
            ergodox_right_led_2_on();
            break;
        case 3:
            ergodox_right_led_3_on();
            break;
        case 4:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            break;
        case 5:
            ergodox_right_led_1_on();
            ergodox_right_led_3_on();
            break;
        case 6:
            ergodox_right_led_2_on();
            ergodox_right_led_3_on();
            break;
        case 7:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            ergodox_right_led_3_on();
            break;
        default:
            break;
    }

    return state;
};

/*  This part is related to RGB matrix and fails to compile if the board has
    RGB matrix disabled */

#if RGB_MATRIX_ENABLE
void keyboard_post_init_user(void) {
    rgb_matrix_enable();
}

const uint8_t PROGMEM ledmap[][RGB_MATRIX_LED_COUNT][3] = {
    [BASE] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {8, 255, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},

    [NAVI] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {163, 218, 204}, {163, 218, 204}, {163, 218, 204}, {163, 218, 204}, {163, 218, 204}, {163, 218, 204}, {163, 218, 204}, {163, 218, 204}, {163, 218, 204}, {163, 218, 204}, {163, 218, 204}, {163, 218, 204}, {163, 218, 204}, {163, 218, 204}, {163, 218, 204}, {163, 218, 204}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {163, 218, 204}, {163, 218, 204}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {163, 218, 204}, {163, 218, 204}, {163, 218, 204}, {163, 218, 204}, {163, 218, 204}, {163, 218, 204}, {163, 218, 204}, {163, 218, 204}, {163, 218, 204}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},

    [MOUS] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {122, 255, 255}, {122, 255, 255}, {122, 255, 255}, {122, 255, 255}, {0, 0, 0}, {122, 255, 255}, {122, 255, 255}, {122, 255, 255}, {122, 255, 255}, {0, 0, 0}, {122, 255, 255}, {122, 255, 255}, {122, 255, 255}, {122, 255, 255}, {122, 255, 255}, {122, 255, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {122, 255, 255}, {122, 255, 255}, {122, 255, 255}, {0, 0, 0}, {0, 0, 0}, {122, 255, 255}, {122, 255, 255}, {122, 255, 255}, {122, 255, 255}, {122, 255, 255}, {122, 255, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},

    [MDIA] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {41, 255, 255}, {41, 255, 255}, {41, 255, 255}, {41, 255, 255}, {41, 255, 255}, {41, 255, 255}, {41, 255, 255}, {41, 255, 255}, {41, 255, 255}, {41, 255, 255}, {41, 255, 255}, {41, 255, 255}, {41, 255, 255}, {41, 255, 255}, {41, 255, 255}, {41, 255, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {41, 255, 255}, {41, 255, 255}, {0, 0, 0}, {0, 0, 0}, {41, 255, 255}, {41, 255, 255}, {41, 255, 255}, {41, 255, 255}, {41, 255, 255}, {41, 255, 255}, {41, 255, 255}, {41, 255, 255}, {0, 0, 0}, {0, 0, 0}, {41, 255, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},

    [NUMB] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 245, 245}, {0, 245, 245}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 245, 245}, {0, 245, 245}, {0, 245, 245}, {0, 245, 245}, {0, 0, 0}, {0, 0, 0}, {0, 245, 245}, {0, 245, 245}, {0, 245, 245}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 245, 245}, {0, 245, 245}, {0, 245, 245}, {0, 245, 245}, {0, 245, 245}, {0, 245, 245}, {0, 245, 245}, {0, 245, 245}, {0, 245, 245}, {0, 245, 245}, {0, 245, 245}, {0, 245, 245}, {0, 245, 245}, {0, 245, 245}, {0, 245, 245}, {0, 245, 245}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},

    [SYMB] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},

    [SNUM] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {0, 0, 0}, {0, 0, 0}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {0, 0, 0}, {0, 0, 0}, {74, 255, 255}, {74, 255, 255}, {74, 255, 255}, {0, 0, 0}, {74, 255, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},

    [FUNC] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {211, 218, 204}, {211, 218, 204}, {211, 218, 204}, {211, 218, 204}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {211, 218, 204}, {211, 218, 204}, {211, 218, 204}, {211, 218, 204}, {0, 0, 0}, {211, 218, 204}, {211, 218, 204}, {211, 218, 204}, {211, 218, 204}, {0, 0, 0}, {211, 218, 204}, {211, 218, 204}, {211, 218, 204}, {211, 218, 204}, {211, 218, 204}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
};

void set_layer_color(int layer) {
    for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        HSV hsv = {
            .h = pgm_read_byte(&ledmap[layer][i][0]),
            .s = pgm_read_byte(&ledmap[layer][i][1]),
            .v = pgm_read_byte(&ledmap[layer][i][2]),
        };
        if (!hsv.h && !hsv.s && !hsv.v) {
            rgb_matrix_set_color(i, 0, 0, 0);
        } else {
            RGB   rgb = hsv_to_rgb(hsv);
            float f   = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
            rgb_matrix_set_color(i, f * rgb.r, f * rgb.g, f * rgb.b);
        }
    }
}

bool rgb_matrix_indicators_user(void) {
    if (keyboard_config.disable_layer_led) {
        return false;
    }
    switch (biton32(layer_state)) {
        case BASE ... FUNC:
            set_layer_color(biton32(layer_state));
            break;
        default:
            if (rgb_matrix_get_flags() == LED_FLAG_NONE) {
                rgb_matrix_set_color_all(0, 0, 0);
            }
            break;
    }

    return false;
}

// End block related to RGB features
#endif
