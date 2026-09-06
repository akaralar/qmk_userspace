#pragma once

#include "quantum.h"

// Onboard ergodox LED indicators (the three right-hand LEDs). Reflects the
// highest active layer, with overrides layered on top for caps lock / caps word
// and the active case mode. Driven from matrix_scan_user (continuously) and
// layer_state_set_user (on layer changes).
void update_led_indicators(void);
