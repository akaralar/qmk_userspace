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

// For more info about casemodes, see https://github.com/andrewjrae/kyria-keymap/
#include "features/casemodes.h"

#include "features/custom_caps_lock.h"

#ifdef CONSOLE_ENABLE
#include "features/debug_helper.h"
#endif

//------------------------------------------------------------------------------
// Keycodes
//------------------------------------------------------------------------------
enum C_keycodes {
    VRSN = EZ_SAFE_RANGE,
    // Custom keycode to toggle rgb lights on / off
    RGB_TGL,
    // Custom keycodes for RGB matrix brightness
    RGB_BUP,
    RGB_BDN,
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
#define ALF_SEA LGUI(LALT(KC_SPC))      // Alfred search
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
#define LS_SNUM LT(SNUM, KC_3) // The tap is intercepted later to send "}"
#define LS_FUNC LT(FUNC, KC_ENTER)
#define LS_SYMB LT(SYMB, KC_C) // The tap is intercepted later to send OSL(SYMB)
// One shots
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
                            || (code) == LS_COLE \
                            || (code) == LS_CLET \
                            || (code) == LS_CTUR)

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

    // Otherwise, only consider alpha keys block
    if (record->event.key.col > 3) {
        return g_tapping_term;
    }

    switch (record->event.key.row) {
        // Increase tapping term for ring and pinky fingers
        case 0 ... 2:
        case 11 ... 13:
            return g_tapping_term + ring_pinky_tap_term_diff;
        // Decrease tapping term for index fingers
        case 4 ... 9:
            return g_tapping_term - index_tap_term_diff;
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
// Achordion
//------------------------------------------------------------------------------
bool achordion_chord(uint16_t tap_hold_keycode,
                     keyrecord_t *tap_hold_record,
                     uint16_t other_keycode,
                     keyrecord_t *other_record) {
    // Allow same hand holds with layer switching keys
    if (IS_LAYER_TAP(tap_hold_keycode)) {
        return true;
    }

    // Allow same-hand holds for thumb keys
    if (other_record->event.key.col >= 4) {
        return true;
    }

    // Otherwise, follow the opposite hands rule.
    return achordion_opposite_hands(tap_hold_record, other_record);
}

uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
    // Disable Achordion for symbol and number layer switch keys, mainly to get
    // around streak timeout.
    switch (tap_hold_keycode) {
        case LS_SYMB:
        case LS_NUMB:
        case LS_SNUM:
            return 0;
    }
    return g_tapping_term + 200;
}

bool achordion_eager_mod(uint8_t mod) {
  switch (mod) {
    // Eagerly apply Shift, Cmd and Alt mods.
    case MOD_LSFT:
    case MOD_RSFT:
    case MOD_LGUI:
    case MOD_RGUI:
    case MOD_LALT:
    case MOD_RALT:
      return true;
    default:
      return false;
  }
};

uint16_t achordion_streak_timeout(uint16_t tap_hold_keycode) {
    // A short streak detection timeout for Space layer-tap key
    if (tap_hold_keycode == LS_NAVI) {
        return 50;
    }

    // Disable streak detection for Shift mod-tap keys or other layer-tap keys.
    if (tap_hold_keycode == MT_Q_F
        || tap_hold_keycode == MT_Q_J
        || tap_hold_keycode == MT_C_T
        || tap_hold_keycode == MT_C_N
        || IS_LAYER_TAP(tap_hold_keycode)
    ) {
        return 0;
    }

    // A longer timeout otherwise.
    return 100;
}

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

//------------------------------------------------------------------------------
// Custom keycode handling
//------------------------------------------------------------------------------
static const char* to_string(uint16_t diff) {
    const char *diff_str = get_u16_str(diff, ' ');
    // Skip padding spaces
    while (*diff_str == ' ') {
        diff_str++;
    }
    return diff_str;
}

static void send_string_if_enabled(const char *string) {
#ifdef SEND_STRING_ENABLE
    send_string(string);
#endif
}

static bool process_tapping_term_keycodes(
    uint16_t keycode,
    keyrecord_t *record
) {
    switch (keycode) {
        case DT_I_UP:
            if (record->event.pressed) {
                index_tap_term_diff += DYNAMIC_TAPPING_TERM_INCREMENT;
            }
            return false;
        case DT_I_DN:
            if (record->event.pressed) {
                index_tap_term_diff -= DYNAMIC_TAPPING_TERM_INCREMENT;
            }
            return false;
        case DT_R_UP:
            if (record->event.pressed) {
                ring_pinky_tap_term_diff += DYNAMIC_TAPPING_TERM_INCREMENT;
            }
            return false;

        case DT_R_DN:
            if (record->event.pressed) {
                ring_pinky_tap_term_diff -= DYNAMIC_TAPPING_TERM_INCREMENT;
            }
            return false;
        case DT_PALL:
            if (record->event.pressed) {
                send_string_if_enabled(to_string(g_tapping_term));
                send_string_if_enabled(", i: ");
                send_string_if_enabled(to_string(index_tap_term_diff));
                send_string_if_enabled(", rp: ");
                send_string_if_enabled(to_string(ring_pinky_tap_term_diff));
            }
            return false;
        default:
            return true;
    }
}

static bool process_casemodes_keycode(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case CM_TOGL:
            if (record->event.pressed) {
                const uint8_t mods = get_mods();
                const uint8_t oneshot_mods = get_oneshot_mods();

                if ((mods | oneshot_mods) & MOD_MASK_SHIFT) {

                    // Shift held, activate camel case
                    unregister_mods(MOD_MASK_SHIFT);
                    del_oneshot_mods(MOD_MASK_SHIFT);
                    enable_xcase_with(OS_LSFT);
                    register_mods(mods);

                    case_mode = CASE_CAMEL;
                } else if ((mods | oneshot_mods) & MOD_MASK_GUI) {

                    // CMD held, activate snake case
                    unregister_mods(MOD_MASK_GUI);
                    del_oneshot_mods(MOD_MASK_GUI);
                    enable_xcase_with(KC_UNDS);
                    register_mods(mods);

                    case_mode = CASE_SNAKE;
                } else if ((mods | oneshot_mods) & MOD_MASK_ALT) {

                    // ALT held, activate kebab case
                    unregister_mods(MOD_MASK_ALT);
                    del_oneshot_mods(MOD_MASK_ALT);
                    enable_xcase_with(KC_MINS);
                    register_mods(mods);

                    case_mode = CASE_KEBAB;
                }
            }
            return false;
        default:
            return true;
    }
}

static void execute_symbol_macro(uint16_t keycode) {
    switch (keycode) {
        // Holding down ampersand sends markdown code block
        case M_CBLOCK:
            SEND_STRING("```");
            tap_code(KC_ENTER);
            tap_code(KC_ENTER);
            SEND_STRING("```");
            tap_code(KC_UP);
            return;
        // Holding down hash sends markdown code block for Swift code
        case M_CBLOCK_S:
            SEND_STRING("```swift");
            tap_code(KC_ENTER);
            tap_code(KC_ENTER);
            SEND_STRING("```");
            tap_code(KC_UP);
            return;
        // Holding down slash sends string and returns
        case M_UPDIR:
            SEND_STRING("../");
            return;

        // Others sends keycodes, breaks out of the switch and sends left
        // arrow after the switch
        // For smart punctuation, we send the keycodes not the string
        case M_DQUOTES:
            tap_code16(KC_DQUO);
            tap_code16(KC_DQUO);
            break;
        case M_QUOTES:
            tap_code(KC_QUOT);
            tap_code(KC_QUOT);
            break;
        case M_BRACKETS:        SEND_STRING("[]");   break;
        case M_PARENS:          SEND_STRING("()");   break;
        case M_ABRACES:         SEND_STRING("<>");   break;
        case M_CBRACES:         SEND_STRING("{}");   break;
        case M_UNDERS:          SEND_STRING("__");   break;
        case M_ASTRSKS:         SEND_STRING("**");   break;
        case M_GRAVES:          SEND_STRING("``");   break;
        // Allow system to process other keycodes
        default: return;
    }
    tap_code(KC_LEFT);
    return;
}

static bool process_tap_or_long_press_key(
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

static bool process_tr_letter_macro(uint16_t keycode, keyrecord_t *record) {
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

static bool process_macro_keycodes(uint16_t keycode, keyrecord_t *record) {
    // Tap-hold macros in symbol layer
    switch (keycode) {
        case FT_SLSH:
            return process_tap_or_long_press_key(record, KC_SLASH, M_UPDIR);
        case FT_LBRC:
            return process_tap_or_long_press_key(record, KC_LBRC, M_BRACKETS);
        case FT_LPRN:
            return process_tap_or_long_press_key(record, KC_LPRN, M_PARENS);
        case FT_LABK:
            return process_tap_or_long_press_key(record, KC_LABK, M_ABRACES);
        case FT_LCBR:
            return process_tap_or_long_press_key(record, KC_LCBR, M_CBRACES);
        case FT_DQUO:
            return process_tap_or_long_press_key(record, KC_DQUO, M_DQUOTES);
        case FT_QUOT:
            return process_tap_or_long_press_key(record, KC_QUOT, M_QUOTES);
        case FT_UNDS:
            return process_tap_or_long_press_key(record, KC_UNDS, M_UNDERS);
        case FT_ASTR:
            return process_tap_or_long_press_key(record, KC_ASTR, M_ASTRSKS);
        case FT_GRV:
            return process_tap_or_long_press_key(record, KC_GRV, M_GRAVES);
        case FT_CBL:
            return process_tap_or_long_press_key(record, KC_AMPR, M_CBLOCK);
        case FT_CBLS:
            return process_tap_or_long_press_key(record, KC_HASH, M_CBLOCK_S);
        case LS_SNUM:
            if (record->event.pressed && record->tap.count != 0) {
                tap_code16(KC_RCBR);
                return false;
            }
            return true;
    }

    return process_tr_letter_macro(keycode, record);
}

static bool should_swallow_esc_release = false;
static bool process_swallowed_esc(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LS_MDIA:
            if (record->tap.count != 0) { // key is being tapped
                // Don't send escape key when trying to exit caps word or case
                // modes
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

static bool process_symbol_layer_keycode(
    uint16_t keycode,
    keyrecord_t* record
) {
    if (keycode == LS_SYMB && record->tap.count == 1)  {
        if (record->event.pressed) {
            set_oneshot_layer(SYMB, ONESHOT_START);
        }

        return false;
    }
    return true;
}

#ifdef RGB_MATRIX_ENABLE
static bool process_rgb_matrix_keycodes(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case RGB_TGL:
            if (record->event.pressed) {
                rgblight_toggle_noeeprom();
            }
            return false;
        case RGB_BUP:
            if (record->event.pressed) {
                rgb_matrix_increase_val_noeeprom();
            }
            return false;
        case RGB_BDN:
            if (record->event.pressed) {
                rgb_matrix_decrease_val_noeeprom();
            }
            return false;
        default:
            return true;
    }
}
#endif

static bool process_other_keycodes(uint16_t keycode, keyrecord_t *record) {
    // Handle if keycode is "dynamic tapping term per key" adjustment keycode
    if (!process_tapping_term_keycodes(keycode, record)) { return false; }

    // Handle if keycode is "casemodes" keycode
    if (!process_casemodes_keycode(keycode, record)) { return false; }

    // Handle Esc when it's being used to exit Caps Word or Case Modes
    if (!process_swallowed_esc(keycode, record)) { return false; }

    // Symbol layer switch key is a fake layer tap key and we intercept the tap
    // to toggle a oneshot symbol layer.
    if (!process_symbol_layer_keycode(keycode, record)) { return false; }

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
                caps_lock_toggle();
                if (is_caps_lock_on()) {
                    caps_word_on();
                } else {
                    caps_word_off();
                }

            }
            return false;
        default:
            return true;
    }

    return true;
};

static bool is_fake_lt_keycode_in_symbol_layer(
    uint16_t keycode,
    keyrecord_t* record
) {
    return get_highest_layer(layer_state) == SYMB && IS_QK_LAYER_TAP(keycode);
}

static void check_and_disable_oneshot_symbol_layer(keyrecord_t* record) {
    if (get_oneshot_layer() == SYMB && record->event.pressed) {
        layer_off(get_oneshot_layer());
        reset_oneshot_layer();
    }
}

//------------------------------------------------------------------------------
// Combos
//------------------------------------------------------------------------------
#ifdef COMBO_ENABLE
enum combo_events {
  M_CODE_BLOCK,
  M_CODE_BLOCK_SWIFT
};

const uint16_t PROGMEM code_block_combo[] = {KC_RABK, FT_GRV, COMBO_END};
const uint16_t PROGMEM swift_code_block_combo[] = {KC_RPRN, FT_CBL, COMBO_END};
combo_t key_combos[] = {
  [M_CODE_BLOCK] = COMBO_ACTION(code_block_combo),
  [M_CODE_BLOCK_SWIFT] = COMBO_ACTION(swift_code_block_combo),
};

void process_combo_event(uint16_t combo_index, bool pressed) {
    switch (combo_index) {
        case M_CODE_BLOCK:
            if (pressed) {
                execute_symbol_macro(M_CBLOCK);
            }
            break;
        case M_CODE_BLOCK_SWIFT:
            if (pressed) {
                execute_symbol_macro(M_CBLOCK_S);
            }
            break;
    }
}
#endif
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
    achordion_task();
    fix_leds_task();
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Pass the keycode and record to achordion for tap-hold decision
    if (!process_achordion(keycode, record)) { return false; }

#ifdef CONSOLE_ENABLE
    prefixed_print(keycode, record, "process_record_user");
#endif

    // For fake layer tap keys in symbol layer that are intercepted, further
    // processing is disabled by returning `false`. When oneshot symbol layer is
    // on, we can't reset the oneshot layer status in `post_process_record_user`
    // as further processing is stopped. For those keycodes we disable the
    // oneshot status here.
    if (is_fake_lt_keycode_in_symbol_layer(keycode, record)) {
        check_and_disable_oneshot_symbol_layer(record);
    }

    // Process case modes after other key codes because we use Esc to quit
    // case modes but we don't want to send the escape key. If case modes
    // handles the key first, it will send the Esc key itself.
    if (!process_case_modes(keycode, record)) { return false; }

    // Pass the keycode and record to custom caps lock
    if (!process_custom_caps_lock(keycode, record)) { return false; }

    // Pass the keycode and record to custom shift keys
    if (!process_custom_shift_keys(keycode, record)) { return false; }

    // Process keycodes for custom macros
    if (!process_macro_keycodes(keycode, record)) { return false; }

    // Process custom keycodes defined in this file
    if (!process_other_keycodes(keycode, record)) { return false; }

    return true;
};

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    // When oneshot symbol layer is on, turn off oneshot status for regular keys
    check_and_disable_oneshot_symbol_layer(record);
}

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
        XXXXXXX, _______, _______
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
                 KC_PIPE, LS_SNUM, FT_LCBR, KC_COLN, KC_COMM, _______,
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
        true , true , true , true , true ,    true , true , true , true , true ,
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
