#include "symbol_layer.h"

static void execute_symbol_macro(uint16_t keycode) {
    switch (keycode) {
        // Holding down ampersand sends markdown code block
        case M_CBLOCK:
            SEND_STRING("```");
            tap_code(KC_ENTER);
            tap_code(KC_ENTER);
            SEND_STRING("```");
            tap_code(KC_UP);
            return;
        // Holding down hash sends markdown code block for Swift code
        case M_CBLOCK_S:
            SEND_STRING("```swift");
            tap_code(KC_ENTER);
            tap_code(KC_ENTER);
            SEND_STRING("```");
            tap_code(KC_UP);
            return;
        // Holding down slash sends string and returns
        case M_UPDIR:
            SEND_STRING("../");
            return;

        // Others sends keycodes, breaks out of the switch and sends left
        // arrow after the switch
        // For smart punctuation, we send the keycodes not the string
        case M_DQUOTES:
            tap_code16(KC_DQUO);
            tap_code16(KC_DQUO);
            break;
        case M_QUOTES:
            tap_code(KC_QUOT);
            tap_code(KC_QUOT);
            break;
        case M_BRACKETS:        SEND_STRING("[]");   break;
        case M_PARENS:          SEND_STRING("()");   break;
        case M_ABRACES:         SEND_STRING("<>");   break;
        case M_CBRACES:         SEND_STRING("{}");   break;
        case M_UNDERS:          SEND_STRING("__");   break;
        case M_ASTRSKS:         SEND_STRING("**");   break;
        case M_GRAVES:          SEND_STRING("``");   break;
        // Allow system to process other keycodes
        default: return;
    }
    tap_code(KC_LEFT);
    return;
}

// Maps a symbol-layer fake LT keycode to its tap symbol and hold macro.
// Returns false if `keycode` is not one of these keys. This is the single
// source of truth for "is this a symbol-layer fake LT" and what it does.
static bool fake_lt_actions(
    uint16_t keycode,
    uint16_t *tap_keycode,
    uint16_t *hold_macro
) {
    switch (keycode) {
        case FT_SLSH: *tap_keycode = KC_SLASH; *hold_macro = M_UPDIR;    return true;
        case FT_LBRC: *tap_keycode = KC_LBRC;  *hold_macro = M_BRACKETS; return true;
        case FT_LPRN: *tap_keycode = KC_LPRN;  *hold_macro = M_PARENS;   return true;
        case FT_LABK: *tap_keycode = KC_LABK;  *hold_macro = M_ABRACES;  return true;
        case FT_RCBR: *tap_keycode = KC_RCBR;  *hold_macro = M_CBRACES;  return true;
        case FT_DQUO: *tap_keycode = KC_DQUO;  *hold_macro = M_DQUOTES;  return true;
        case FT_QUOT: *tap_keycode = KC_QUOT;  *hold_macro = M_QUOTES;   return true;
        case FT_UNDS: *tap_keycode = KC_UNDS;  *hold_macro = M_UNDERS;   return true;
        case FT_ASTR: *tap_keycode = KC_ASTR;  *hold_macro = M_ASTRSKS;  return true;
        case FT_GRV:  *tap_keycode = KC_GRV;   *hold_macro = M_GRAVES;   return true;
        case FT_CBL:  *tap_keycode = KC_AMPR;  *hold_macro = M_CBLOCK;   return true;
        case FT_CBLS: *tap_keycode = KC_HASH;  *hold_macro = M_CBLOCK_S; return true;
        // LS_SNUM's tap is "{"; its hold is a layer switch (handled specially,
        // no macro), so `hold_macro` is unused for it.
        case LS_SNUM: *tap_keycode = KC_LCBR;  *hold_macro = KC_NO;      return true;
        default:      return false;
    }
}

// Performs the intended symbol-layer action for a fake LT key. `keycode` is the
// fake LT keycode (resolved directly in the stable case, or reconstructed from
// the pending record in the collapse case), and `record->tap.count` decides
// tap vs hold (reliable in both cases). Always consumes the event so the
// underlying keycode - the LT placeholder, or the collapsed base key - is never
// emitted.
static bool handle_fake_lt(uint16_t keycode, keyrecord_t *record) {
    uint16_t tap_keycode;
    uint16_t hold_macro;
    if (!fake_lt_actions(keycode, &tap_keycode, &hold_macro)) {
        return true; // not a fake LT: let normal processing continue
    }

    if (record->tap.count > 0) {
        // Tap: emit the symbol once, on the key-down.
        if (record->event.pressed) {
            tap_code16(tap_keycode);
        }
    } else if (keycode == LS_SNUM) {
        // LS_SNUM's hold switches to the SNUM layer. We drive it manually
        // instead of letting QMK's LT do it, so the stable and collapse paths
        // are identical: in a collapse QMK hands us the base keycode and would
        // never switch layers on its own.
        if (record->event.pressed) {
            layer_on(SNUM);
        } else {
            layer_off(SNUM);
        }
    } else if (record->event.pressed) {
        // FT_* hold runs a one-shot macro on the key-down.
        execute_symbol_macro(hold_macro);
    }

    return false; // symbol-layer action handled; suppress default processing
}

// Fake symbol-layer LT keys are deferred by the tapping machinery. When
// MO(SYMB) is released before such a key settles, the symbol layer "collapses"
// and QMK resolves the key on the base layer - handing us the base keycode
// (e.g. "n") instead of the fake LT. To honor the intent fixed at key-down (the
// layer the key was pressed on), `pre_process` records every fake LT pressed on
// the symbol layer, and if a base keycode later resolves at that position we
// reconstruct the symbol-layer action from the recorded keycode.
#define FAKE_LT_PENDING_SLOTS 4
typedef struct {
    bool     active;
    uint8_t  row;
    uint8_t  col;
    uint16_t keycode;        // the fake LT that was pressed at this position
    bool     reconstructing; // whether the key-down committed a collapse reconstruct
} fake_lt_pending_t;

static fake_lt_pending_t fake_lt_pending[FAKE_LT_PENDING_SLOTS];

static fake_lt_pending_t *find_pending_fake_lt(keyrecord_t *record) {
    for (int i = 0; i < FAKE_LT_PENDING_SLOTS; i++) {
        if (fake_lt_pending[i].active
            && fake_lt_pending[i].row == record->event.key.row
            && fake_lt_pending[i].col == record->event.key.col
        ) {
            return &fake_lt_pending[i];
        }
    }
    return NULL;
}

static void remember_pending_fake_lt(uint16_t keycode, keyrecord_t *record) {
    fake_lt_pending_t *slot = find_pending_fake_lt(record);
    if (slot == NULL) {
        for (int i = 0; i < FAKE_LT_PENDING_SLOTS; i++) {
            if (!fake_lt_pending[i].active) { slot = &fake_lt_pending[i]; break; }
        }
    }
    if (slot == NULL) { return; } // all slots busy (5+ pending is not realistic)

    slot->active         = true;
    slot->row            = record->event.key.row;
    slot->col            = record->event.key.col;
    slot->keycode        = keycode;
    slot->reconstructing = false;
}

bool pre_process_symbol_layer_fake_lt_keys(
    uint16_t keycode,
    keyrecord_t *record
) {
    // Record intent at key-down, while the correct layer is still live:
    // pre_process runs before the tapping machinery defers the key, so
    // `keycode` is the real fake LT even if the layer later collapses. We only
    // record - nothing is emitted here - so modal apps (e.g. Vim) never see a
    // premature keystroke.
    uint16_t tap_keycode;
    uint16_t hold_macro;
    if (record->event.pressed
        && fake_lt_actions(keycode, &tap_keycode, &hold_macro)
    ) {
        remember_pending_fake_lt(keycode, record);
    }

    return true;
}

bool process_symbol_layer_fake_lt_keys(
    uint16_t keycode,
    keyrecord_t *record
) {
    uint16_t tap_keycode;
    uint16_t hold_macro;

    // Stable case: the key resolved on the symbol layer, so QMK hands us the
    // fake LT keycode directly. Handle it and drop any pending record.
    if (fake_lt_actions(keycode, &tap_keycode, &hold_macro)) {
        fake_lt_pending_t *slot = find_pending_fake_lt(record);
        if (slot != NULL && !record->event.pressed) { slot->active = false; }
        return handle_fake_lt(keycode, record);
    }

    // A base/other-layer keycode resolved here. If a fake LT was pressed at this
    // position, decide whether this is a genuine collapse: it is only a collapse
    // if the layer fell all the way back to a base alpha layer. If a functional
    // layer (e.g. SNUM) is on top, the key resolved there on purpose - a number
    // on SNUM at a position that is a fake LT on SYMB - and must pass through
    // untouched. We decide on the key-down and mirror it on the release, so the
    // release does not depend on how the key happens to re-resolve.
    fake_lt_pending_t *slot = find_pending_fake_lt(record);
    if (slot != NULL) {
        if (record->event.pressed) {
            slot->reconstructing = get_highest_layer(layer_state) < NAVI;
            if (slot->reconstructing) {
                return handle_fake_lt(slot->keycode, record);
            }
            return true; // resolved on a functional layer: pass through
        }

        bool     reconstructing = slot->reconstructing;
        uint16_t recorded       = slot->keycode;
        slot->active = false; // consume the pending record on release
        if (reconstructing) {
            return handle_fake_lt(recorded, record);
        }
        return true;
    }

    return true; // not a fake LT and nothing pending: normal processing
}
