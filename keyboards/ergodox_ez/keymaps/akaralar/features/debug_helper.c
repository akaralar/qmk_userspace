#include "debug_helper.h"
#include  "print.h"

uint16_t counter = 0;
void debug_print(uint16_t keycode, keyrecord_t *record, const char* prefix) {
#ifdef CONSOLE_ENABLE
    uprintf("%2u %s kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u, type: %u\n", counter, prefix, keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count, record->event.type);
    counter++;
#endif
}

void enable_debug_user() {
#ifdef CONSOLE_ENABLE
  debug_enable=true;
//   debug_matrix=true;
//   debug_keyboard=true;
//   debug_mouse=true;
#endif
}
