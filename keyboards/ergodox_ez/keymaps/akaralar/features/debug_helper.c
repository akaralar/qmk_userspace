#include "debug_helper.h"


#ifdef CONSOLE_ENABLE
#include  "print.h"

void enable_debug_user() {
  debug_enable=true;
//   debug_matrix=true;
//   debug_keyboard=true;
//   debug_mouse=true;
}

uint16_t counter = 0;
void prefixed_print(uint16_t keycode, keyrecord_t *record, const char* prefix) {
    uprintf("%4u %s kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u, type: %u\n", counter, prefix, keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count, record->event.type);
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
