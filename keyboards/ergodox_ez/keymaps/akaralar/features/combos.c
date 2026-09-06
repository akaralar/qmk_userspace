#include "quantum.h"

#ifdef COMBO_ENABLE

#include "casemodes_config.h" // for enable_*_case
#include "custom_caps_lock.h" // for toggle_caps_lock

// Combos that trigger the caps word / caps lock and case-mode actions. QMK
// picks up key_combos[] and process_combo_event automatically when
// COMBO_ENABLE is set; there is no entry point for the keymap to call.

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

#endif // COMBO_ENABLE
