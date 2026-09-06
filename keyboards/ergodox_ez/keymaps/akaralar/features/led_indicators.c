#include "led_indicators.h"
#include QMK_KEYBOARD_H     // for the ergodox_* LED helpers
#include "common.h"         // for the layer enum
#include "casemodes.h"      // for get_xcase_state / XCASE_OFF
#include "casemodes_config.h" // for get_case_mode
#include "custom_caps_lock.h" // for is_caps_lock_on

void update_led_indicators(void) {
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
    if (is_caps_lock_on()) {
        ergodox_right_led_3_on();
    } else if (is_caps_word_on()) {
        ergodox_right_led_2_on();
    }

    // Fix LED lights behaviour for case modes
    if (get_xcase_state() != XCASE_OFF) {
        switch (get_case_mode()) {
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
}
