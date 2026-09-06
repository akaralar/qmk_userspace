#include "turkish.h"
#include "custom_caps_lock.h" // for is_caps_lock_on

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

bool process_turkish_diacritics(uint16_t keycode, keyrecord_t *record) {
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
