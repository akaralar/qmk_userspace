#include "layer_rgb.h"

#if RGB_MATRIX_ENABLE

#include QMK_KEYBOARD_H // for keyboard_config

bool process_rgb_matrix_keycodes(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case RGB_TGL:
            if (record->event.pressed) {
                const uint8_t mods = get_mods();
                const uint8_t oneshot_mods = get_oneshot_mods();

                if ((mods | oneshot_mods) & MOD_MASK_SHIFT) {
                    rgblight_toggle();
                } else {
                    rgblight_toggle_noeeprom();
                }
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
        false, false, true , true , true ,    true , true , true , true , true ,
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
        true , true , true , true , false,    false, false, false, false, false,
        true , true , true , true , false,    false, true , true , true , true ,
        true , true , true , true , false,    false, false, false, false, false,
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

static void set_layer_rgb_colors(int layer) {
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

#endif // RGB_MATRIX_ENABLE
