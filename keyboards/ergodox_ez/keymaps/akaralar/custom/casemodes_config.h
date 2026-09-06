#pragma once

#include "quantum.h"
#include "common.h"

// Keymap-specific configuration on top of the vendored casemodes.c feature:
// the concrete case modes (camel / snake / kebab), the CM_TOGL keycode handling
// that picks a mode from the held modifiers, and the terminate_case_modes weak
// override that decides which keys keep a case mode alive.

//------------------------------------------------------------------------------
// Case modes
//------------------------------------------------------------------------------
enum case_mode {
    CASE_NONE = -1, // no case mode active (initial / sentinel state)
    CASE_SNAKE,
    CASE_KEBAB,
    CASE_CAMEL,
};

// The case mode selected by the most recent enable_*_case call. Used by the LED
// indicators to show which mode is active while xcase is on.
enum case_mode get_case_mode(void);

void enable_camel_case(void);
void enable_snake_case(void);
void enable_kebab_case(void);

//------------------------------------------------------------------------------
// Entry point (call from process_record_user)
//------------------------------------------------------------------------------
// Handles the CM_TOGL keycode, choosing camel / snake / kebab from the modifiers
// held at press time. Returns false (and consumes the event) when it handled
// CM_TOGL, true otherwise.
bool process_casemodes_config(uint16_t keycode, keyrecord_t *record);
