#include "debug_helper.h"

#ifdef CONSOLE_ENABLE
#include  "print.h"


static char* letter_for_keycode(uint16_t keycode) {
    enum qk_keycode_defines kc = keycode & 0x00FF;

    switch (kc) {
        case KC_NO: return "No-op";
        case KC_TRANSPARENT: return "Transparent";
        case KC_A: return "A";
        case KC_B: return "B";
        case KC_C: return "C";
        case KC_D: return "D";
        case KC_E: return "E";
        case KC_F: return "F";
        case KC_G: return "G";
        case KC_H: return "H";
        case KC_I: return "I";
        case KC_J: return "J";
        case KC_K: return "K";
        case KC_L: return "L";
        case KC_M: return "M";
        case KC_N: return "N";
        case KC_O: return "O";
        case KC_P: return "P";
        case KC_Q: return "Q";
        case KC_R: return "R";
        case KC_S: return "S";
        case KC_T: return "T";
        case KC_U: return "U";
        case KC_V: return "V";
        case KC_W: return "W";
        case KC_X: return "X";
        case KC_Y: return "Y";
        case KC_Z: return "Z";
        case KC_1: return "1";
        case KC_2: return "2";
        case KC_3: return "3";
        case KC_4: return "4";
        case KC_5: return "5";
        case KC_6: return "6";
        case KC_7: return "7";
        case KC_8: return "8";
        case KC_9: return "9";
        case KC_0: return "0";
        case KC_ENTER: return "Enter";
        case KC_ESCAPE: return "Escape";
        case KC_BACKSPACE: return "Backspace";
        case KC_TAB: return "Tab";
        case KC_SPACE: return "Space";
        case KC_QUOTE: return "Quote";
        case KC_COMMA: return "Comma";
        case KC_DOT: return "Dot";
        case KC_SLASH: return "Slash";
        default: return "Other kc";
    }
}

void enable_debug_user() {
  debug_enable=true;
//   debug_matrix=true;
//   debug_keyboard=true;
//   debug_mouse=true;
}

uint16_t counter = 0;
void prefixed_print(uint16_t keycode, keyrecord_t *record, const char* prefix) {
    uprintf("%4u %s kc: 0x%04X, col: %1u, row: %2u, pressed: %u, count: %u, int: %u, time: %5u, let: %s\n", counter, prefix, keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->tap.count, record->tap.interrupted, record->event.time, letter_for_keycode(keycode));
    counter++;
}

void prefixed_print_keycode(uint16_t keycode, const char* prefix) {
    uprintf("%4u %s kc: 0x%04X\n", counter, prefix, keycode);
    counter++;
}

void prefixed_print_letter(uint16_t keycode, const char* prefix) {
    uprintf("%4u %s letter: %s\n", counter, prefix, letter_for_keycode(keycode));
    counter++;
}
void prefixed_print_layer(layer_state_t state, const char* prefix) {
    uprintf("%4u %s layer: %2u\n", counter, prefix, state);
    counter++;
}

void print_mods(void) {
    uint8_t mods = get_mods();
    uint8_t osm = get_oneshot_mods();
    uint8_t weak = get_weak_mods();

    uprintf("Shift: %d, ", mods & MOD_MASK_SHIFT);
    uprintf("osm: %d, ", osm & MOD_MASK_SHIFT);
    uprintf("weak: %d\n", weak & MOD_MASK_SHIFT);

    // uprintf("Cmd  : %d, ", mods & MOD_MASK_GUI);
    // uprintf("osm: %d, ", osm & MOD_MASK_GUI);
    // uprintf("weak: %d\n", weak & MOD_MASK_GUI);

    // uprintf("Alt  : %d, ", mods & MOD_MASK_ALT);
    // uprintf("osm: %d, ", osm & MOD_MASK_ALT);
    // uprintf("weak: %d\n", weak & MOD_MASK_ALT);

    // uprintf("Ctrl : %d, ", mods & MOD_MASK_CTRL);
    // uprintf("osm: %d, ", osm & MOD_MASK_CTRL);
    // uprintf("weak: %d\n", weak & MOD_MASK_CTRL);
}

void prefixed_print_all_mods(const char* prefix) {
    uprintf("%4u %s layer: %2u, ", counter, prefix);
    print_mods();
}
#endif // CONSOLE_ENABLE
