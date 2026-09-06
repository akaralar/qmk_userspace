#pragma once

#include "quantum.h"

// Definitions shared across the keymap and its feature modules. Keeping the
// layer list and the custom-keycode registry here lets feature modules (e.g.
// symbol_layer) reference them without depending on keymap.c.

//------------------------------------------------------------------------------
// Layers
//------------------------------------------------------------------------------
enum layers {
    COLE, // default colemak layer
    CLET, // Only letters without modtaps for colemak
    CTUR, // Only letters without modtaps for colemak
    QWER, // default qwerty layer
    QLET, // Only letters without modtaps for qwerty
    QTUR, // Turkish letters with diacritics
    NAVI, // navigation layer
    MOUS, // mouse layer
    MDIA, // media keys layer
    NUMB, // numbers layer
    SYMB, // code symbols layer
    SNUM, // numbers from symbols layer
    FUNC, // Function keys layer
};

//------------------------------------------------------------------------------
// Custom keycodes
//------------------------------------------------------------------------------
enum C_keycodes {
    VRSN = QK_USER_0,
    // Custom keycode to toggle rgb lights on / off
    RGB_TGL,
    // Custom keycodes for RGB matrix brightness
    RGB_BUP,
    RGB_BDN,
    // Keycode for activating casemodes
    CM_TOGL,
    // Keycode for caps lock.
    // Regular caps lock is assigned as a macOS globe (fn) key in macOS
    CPS_LCK,
    // Macro keycodes
    // Turkish letter macros
    TC_C,
    TC_G,
    TC_I,
    TC_O,
    TC_S,
    TC_U,
    // Symbol Macros (handled by the symbol_layer feature)
    M_UPDIR,
    M_BRACKETS,
    M_PARENS,
    M_ABRACES,
    M_CBRACES,
    M_DQUOTES,
    M_QUOTES,
    M_UNDERS,
    M_ASTRSKS,
    M_GRAVES,
    M_CBLOCK,
    M_CBLOCK_S
};
