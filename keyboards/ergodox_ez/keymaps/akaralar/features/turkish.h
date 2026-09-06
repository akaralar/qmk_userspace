#pragma once

#include "quantum.h"
#include "common.h"

// Turkish diacritic letters (ç, ğ, ı, ö, ş, ü) typed via the macOS US-International
// dead-key sequences. Each TC_* keycode (defined in common.h) taps the relevant
// Option dead key, then the base letter, honoring shift / caps lock / caps word
// so the result matches the current case.

//------------------------------------------------------------------------------
// Entry point (call from process_record_user)
//------------------------------------------------------------------------------
// Handles the TC_C..TC_U keycodes. Returns false (and consumes the event) when
// it handled a Turkish letter keycode, true otherwise.
bool process_turkish_diacritics(uint16_t keycode, keyrecord_t *record);
