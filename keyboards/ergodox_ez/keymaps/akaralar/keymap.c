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

#ifdef CONSOLE_ENABLE
#include "features/debug_helper.h"
#endif

//------------------------------------------------------------------------------
// Keycodes
//------------------------------------------------------------------------------
enum C_keycodes {
    VRSN = EZ_SAFE_RANGE,
#ifdef RGB_MATRIX_ENABLE
    // Custom keycode to toggle rgb lights on / off
    RGB_TGL,
    // Custom keycodes for RGB matrix brightness
    RGB_BUP,
    RGB_BDN,
#endif
    // Increase/decrease the difference from tapping term for ring/pinky fingers
    DT_R_UP,
    DT_R_DN,
    // Increase/decrease the difference from tapping term for index fingers
    DT_I_UP,
    DT_I_DN,
    // Print all the differences and the tapping term
    DT_PALL,
    // Keycode for activating casemodes
    CM_TOGL,
    // Keycode for caps lock.
    // Regular caps lock is assigned as a macOS globe (fn) key in macOS
    CPS_LCK,
    // Macro keycodes
    // Turkish letter macros
    TC_C,
    TC_G,
    TC_I,
    TC_O,
    TC_S,
    TC_U,
    // Symbol Macros
    M_UPDIR,
    M_BRACKETS,
    M_PARENS,
    M_ABRACES,
    M_CBRACES,
    M_DQUOTES,
    M_QUOTES,
    M_UNDERS,
    M_ASTRSKS,
    M_GRAVES,
    M_CBLOCK,
    M_CBLOCK_S
};

// Custom modifiers in single key
#define KC_CSG LCTL(LSFT(KC_LEFT_GUI))

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

// One-shot modifiers
#define OS_LSFT OSM(MOD_LSFT)

// macOS keycodes
#define KC_CUT LGUI(KC_X)
#define KC_COPY LGUI(KC_C)
#define KC_PSTE LGUI(KC_V)
#define KC_UNDO LGUI(KC_Z)
#define KC_REDO LGUI(LSFT(KC_Z))
#define KC_FN KC_CAPS // caps lock is converted to fn key in the OS

// Application shortcuts
#define MOOM LALT(LCTL(LSFT(KC_GRAVE))) // Moom
#define ALF_NAV LGUI(LCTL(KC_SLASH))    // Alfred file navigation
#define ALF_ACT RGUI(RCTL(KC_BSLS))     // Alfred action selection
#define ONEP_QA LALT(LGUI(KC_BSLS))     // 1password quick access
#define ONEP_AF LGUI(LSFT(KC_BSLS))     // 1password autofill
#define TH_QE MEH(KC_T)                 // Things quick entry
#define TH_QEAF HYPR(KC_T)              // Things quick entry with autofill
#define PRT_SCR LGUI(LSFT(KC_5))        // Show screenshot tool
#define EDT_SCR LGUI(LSFT(KC_4))        // Show edit screenshot tool

//------------------------------------------------------------------------------
// Layers and layer keycodes
//------------------------------------------------------------------------------
enum layers {
    QWER, // default qwerty layer
    QLET, // Only letters without modtaps for qwerty
    QTUR, // Turkish letters with diacritics
    COLE, // default colemak layer
    CLET, // Only letters without modtaps for colemak
    CTUR, // Only letters without modtaps for colemak
    NAVI, // navigation layer
    MOUS, // mouse layer
    MDIA, // media keys layer
    NUMB, // numbers layer
    SYMB, // code symbols layer
    SNUM, // numbers from symbols layer
    FUNC, // Function keys layer
};

// Layer switching keys
// Layer-taps
#define LS_NAVI LT(NAVI, KC_SPACE)
#define LS_MOUS LT(MOUS, KC_TAB)
#define LS_MDIA LT(MDIA, KC_ESCAPE)
#define LS_NUMB LT(NUMB, KC_BSPC)
#define LS_SNUM LT(SNUM, KC_EQUAL)
#define LS_FUNC LT(FUNC, KC_ENTER)
// One shots
#define LS_SYMB OSL(SYMB) // For switching to symbol layer
#define LS_QTUR OSL(QTUR) // For Turkish characters layer on Qwerty
#define LS_CTUR OSL(CTUR) // For Turkish characters layer on Colemak
// Toggling layers where mod-taps are removed from letter keys
#define LS_QLET TT(QLET)
#define LS_CLET TT(CLET)
// Toggling Colemak on / off
#define LS_COLE TG(COLE)

// Fake layer-tap keys. These keys are used in macros and since some of them
// are not basic keycodes, both tap and hold action is handled in the macros,
// which means they don't send the keycode defined here.
// See https://getreuer.info/posts/keyboards/triggers/index.html
// Tapping sends a key and holding performs the macro
#define FT_SLSH LT(SYMB, KC_0)
#define FT_LBRC LT(SYMB, KC_1)
#define FT_LPRN LT(SYMB, KC_2)
#define FT_LABK LT(SYMB, KC_3)
#define FT_LCBR LT(SYMB, KC_4)
#define FT_DQUO LT(SYMB, KC_5)
#define FT_QUOT LT(SYMB, KC_6)
#define FT_UNDS LT(SYMB, KC_7)
#define FT_ASTR LT(SYMB, KC_8)
#define FT_GRV LT(SYMB, KC_9)
#define FT_CBL LT(SYMB, KC_A)
#define FT_CBLS LT(SYMB, KC_B)

bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // Enable retro tapping for "=" / snum layer switch key
        case LS_SNUM:
            return true;
        default:
            return false;
    }
};

//------------------------------------------------------------------------------
// Custom keycode handling
//------------------------------------------------------------------------------
void execute_symbol_macro(uint16_t keycode) {
    switch (keycode) {
        case M_DQUOTES:
            tap_code16(KC_DQUO);
            tap_code16(KC_DQUO);
            tap_code(KC_LEFT);
            break;
    }
    return;
}

bool process_tap_or_long_press_key(
    keyrecord_t *record,
    uint16_t tap_keycode,
    uint16_t long_press_keycode
) {
    if (record->tap.count == 0) { // Key is being held.
        if (record->event.pressed) {
            execute_symbol_macro(long_press_keycode);
        }
        return false; // Skip default handling.
    } else {
        if (record->event.pressed) {
            register_code16(tap_keycode);
        } else {
            unregister_code16(tap_keycode);
        }
        return false; // Skip default handling.
    }
}


bool process_macro_keycodes(uint16_t keycode, keyrecord_t *record) {
    // Tap-hold macros in symbol layer
    switch (keycode) {
        case FT_DQUO:
            return process_tap_or_long_press_key(record, KC_DQUO, M_DQUOTES);
    }

    return true;
}

//------------------------------------------------------------------------------
// QMK User space functions
//------------------------------------------------------------------------------
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Process keycodes for custom macros
    if (!process_macro_keycodes(keycode, record)) { return false; }

    return true;
};

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
    [QWER] = LAYOUT_ergodox(
        _______, _______, _______, _______, _______, _______, _______,
        _______, KC_Q   , MT_W   , MT_Q_E , MT_Q_R , KC_T   , _______,
        _______, MT_A   , MT_Q_S , MT_Q_D , MT_Q_F , KC_G   ,
        _______, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , _______,
        _______, _______, _______, _______, LS_MDIA,
                                                     _______, LS_COLE,
                                                              OS_LSFT,
                                            LS_NAVI, LS_MOUS, CM_TOGL,

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
        _______, _______, _______
    ),

    [COLE] = LAYOUT_ergodox(
        _______, _______, _______, _______, _______, _______, _______,
        _______, KC_Q   , MT_W   , MT_C_F , MT_C_P , KC_B   , _______,
        _______, MT_A   , MT_C_R , MT_C_S , MT_C_T , KC_G   ,
        _______, KC_Z   , KC_X   , KC_C   , KC_D   , KC_V   , _______,
        _______, _______, _______, _______, LS_MDIA,
                                                     _______, _______,
                                                              OS_LSFT,
                                            LS_NAVI, LS_MOUS, CM_TOGL,

        _______, _______, _______, _______, _______, _______, _______,
        _______, KC_J   , MT_C_L , MT_C_U , MT_C_Y , KC_QUOT, _______,
                 KC_M   , MT_C_N , MT_C_E , MT_C_I , MT_C_O , _______,
        _______, KC_K   , KC_H   , KC_COMM, KC_DOT , KC_SLSH, _______,
                          LS_SYMB, _______, _______, _______, _______,
        LS_CLET, _______,
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
                 _______, _______, _______, TC_I   , KC_O, _______,
        _______, _______, _______, _______, _______, _______, _______,
                          _______, _______, _______, _______, _______,
        _______, _______,
        _______,
        _______, _______, _______
    ),

    [NAVI] = LAYOUT_ergodox(
        _______, _______, _______, _______, _______, _______, _______,
        _______, XXXXXXX, KC_CSG , KC_MEH , KC_HYPR, XXXXXXX, _______,
        _______, KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, XXXXXXX,
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
        _______, EDT_SCR, PRT_SCR, ALF_NAV, ALF_ACT, MOOM   , _______,
        _______, KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, ONEP_QA,
        _______, XXXXXXX, XXXXXXX, TH_QE  , TH_QEAF, XXXXXXX, _______,
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
                 KC_PIPE, KC_RCBR, FT_LCBR, KC_COLN, KC_COMM, _______,
        _______, FT_QUOT, LS_SNUM, KC_MINS, KC_EXLM, KC_SCLN, _______,
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
        _______, XXXXXXX, DT_I_UP, DT_UP  , DT_R_UP, XXXXXXX, _______,
                 DT_PALL, KC_RSFT, KC_RGUI, KC_LALT, KC_RCTL, _______,
        _______, XXXXXXX, DT_I_DN, DT_DOWN, DT_R_DN, XXXXXXX, _______,
                          _______, _______, _______, _______, _______,
        _______, _______,
        _______,
        _______, XXXXXXX, XXXXXXX
    )
};

// clang-format on

//------------------------------------------------------------------------------
// RGB Matrix
//------------------------------------------------------------------------------
#if RGB_MATRIX_ENABLE

/*  ---- LEFT HAND ----     ---- RIGHT HAND ---- */
#define LED_LAYOUT_ergodox_pretty(                \
    L01,L02,L03,L04,L05,    R01,R02,R03,R04,R05,  \
    L11,L12,L13,L14,L15,    R11,R12,R13,R14,R15,  \
    L21,L22,L23,L24,L25,    R21,R22,R23,R24,R25,  \
    L31,L32,L33,L34,L35,    R31,R32,R33,R34,R35,  \
    L41,L42,L43,L44,            R42,R43,R44,R45 ) \
                                                  \
   /* matrix positions */                         \
    { R01, R02, R03, R04, R05,                    \
      R11, R12, R13, R14, R15,                    \
      R21, R22, R23, R24, R25,                    \
      R31, R32, R33, R34, R35,                    \
           R42, R43, R44, R45,                    \
                                                  \
      L05, L04, L03, L02, L01,                    \
      L15, L14, L13, L12, L11,                    \
      L25, L24, L23, L22, L21,                    \
      L35, L34, L33, L32, L31,                    \
           L44, L43, L42, L41                     \
    }


const bool PROGMEM rgb_on[][RGB_MATRIX_LED_COUNT] = {
    [QWER] = LED_LAYOUT_ergodox_pretty(
        false, false, false, false, false,    false, false, false, false, false,
        true , true , true , true , true ,    true , true , true , true , true ,
        true , true , true , true , true ,    true , true , true , true , true ,
        true , true , true , true , true ,    true , true , true , true , true ,
        false, false, false, true ,                  true , false, false, false
    ),
    [QLET] = LED_LAYOUT_ergodox_pretty(
        false, false, false, false, false,    false, false, false, false, false,
        false, true , true , true , false,    false, true , true , true , false,
        true , true , true , true , false,    false, true , true , true , true ,
        false, false, false, false, false,    false, false, false, false, false,
        false, false, false, false ,                false , false, false, false
    ),
    [QTUR] = LED_LAYOUT_ergodox_pretty(
        false, false, false, false, false,    false, false, false, false, false,
        false, false, false, false, false,    false, true , true , true , false,
        false, true , false, false, true ,    false, false, false, false, false,
        false, false, true , false, false,    false, false, false, false, false,
        false, false, false, false ,                false , false, false, false
    ),
    [COLE] = LED_LAYOUT_ergodox_pretty(
        false, false, false, false, false,    false, false, false, false, false,
        true , true , true , true , true ,    true , true , true , true , true ,
        true , true , true , true , true ,    true , true , true , true , true ,
        true , true , true , true , true ,    true , true , true , true , true ,
        false, false, false, true ,                  true , false, false, false
    ),
    [CLET] = LED_LAYOUT_ergodox_pretty(
        false, false, false, false, false,    false, false, false, false, false,
        false, true , true , true , false,    false, true , true , true , false,
        true , true , true , true , false,    false, true , true , true , true ,
        false, false, false, false, false,    false, false, false, false, false,
        false, false, false, false ,                false , false, false, false
    ),
    [CTUR] = LED_LAYOUT_ergodox_pretty(
        false, false, false, false, false,    false, false, false, false, false,
        false, false, false, false, false,    false, false, true , false, false,
        false, false, true , false, true ,    false, false, false, true , true ,
        false, false, true , false, false,    false, false, false, false, false,
        false, false, false, false ,                false , false, false, false
    ),

    [NAVI] = LED_LAYOUT_ergodox_pretty(
        false, false, false, false, false,    false, false, false, false, false,
        false, false, true , true , false,    true , true , true , true , true ,
        true , true , true , true , false,    true , true , true , true , true ,
        true , true , true , true , true ,    true , true , true , true , true ,
        false, false, false, false,                  true , false, false, false
    ),
    [MOUS] = LED_LAYOUT_ergodox_pretty(
        false, false, false, false, false,    false, false, false, false, false,
        true , true , true , true , true ,    true , true , true , true , false,
        true , true , true , true , true ,    true , true , true , true , false,
        false, true , true , true , true ,    true , true , true , true , true ,
        false, false, false, false,                  true , false, false, false
    ),
    [MDIA] =  LED_LAYOUT_ergodox_pretty(
        false, false, false, false, false,    false, false, false, false, false,
        true , false, false, true , true ,    true , true , true , true , false,
        true , true , true , true , true ,    true , true , true , true , false,
        false, false, false, true , true ,    true , true , false, true , false,
        false, false, false, false,                  false, false, false, false
    ),
    [NUMB] =  LED_LAYOUT_ergodox_pretty(
        false, false, false, false, false,    false, false, false, false, false,
        true , true , true , true , true ,    false, true , true , false, false,
        true , true , true , true , true ,    false, true , true , true , true ,
        true , true , true , true , true ,    false, false, true , true , true ,
        false, false, false, true ,                  false, false, false, false
    ),
    [SYMB] = LED_LAYOUT_ergodox_pretty(
        false, false, false, false, false,    false, false, false, false, false,
        true , true , true , true , true ,    true , true , true , true , true ,
        true , true , true , true , true ,    true , true , true , true , true ,
        true , true , true , true , true ,    true , true , true , true , true ,
        false, false, false, true ,                  false, false, false, false
    ),
    [SNUM] = LED_LAYOUT_ergodox_pretty(
        false, false, false, false, false,    false, false, false, false, false,
        false, true , true , true , false,    false, false, false, false, false,
        false, true , true , true , false,    false, false, false, false, false,
        false, true , true , true , false,    false, false, false, false, false,
        false, false, false, true ,                  false, false, false, false
    ),
    [FUNC] = LED_LAYOUT_ergodox_pretty(
        false, false, false, false, false,    false, false, false, false, false,
        true , true , true , true , false,    false, true , true , true , false,
        true , true , true , true , false,    true , true , true , true , true ,
        true , true , true , true , false,    false, true , true , true , false,
        false, false, false, true ,                  false, false, false, false
    ),
};

const uint8_t PROGMEM rgb_colors[][3] = {
    [QWER] = {8, 255, 255},
    [QLET] = {8, 255, 255},
    [QTUR] = {8, 255, 255},
    [COLE] = {8, 255, 255},
    [CLET] = {8, 255, 255},
    [CTUR] = {8, 255, 255},
    [NAVI] = {163, 218, 255},
    [MOUS] = {122, 255, 255},
    [MDIA] = {41, 255, 255},
    [NUMB] = {0, 245, 255},
    [SYMB] = {74, 255, 255},
    [SNUM] = {74, 255, 255},
    [FUNC] = {211, 218, 255}
};

void set_layer_rgb_colors(int layer) {
    for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        if (!pgm_read_byte(&rgb_on[layer][i])) {
            rgb_matrix_set_color(i, 0, 0, 0);
        } else {
            HSV hsv = {
                .h = pgm_read_byte(&rgb_colors[layer][0]),
                .s = pgm_read_byte(&rgb_colors[layer][1]),
                .v = pgm_read_byte(&rgb_colors[layer][2]),
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
};

bool rgb_matrix_indicators_user(void) {
    if (keyboard_config.disable_layer_led) {
        return false;
    }
    switch (biton32(layer_state)) {
        case QWER ... FUNC:
            set_layer_rgb_colors(biton32(layer_state));
            break;
        default:
            if (rgb_matrix_get_flags() == LED_FLAG_NONE) {
                rgb_matrix_set_color_all(0, 0, 0);
            }
            break;
    }

    return false;
};

// End block related to RGB features
#endif
