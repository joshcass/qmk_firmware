// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

/* Preonic port of the bastardkb/scylla joshcass keymap.
 *
 * The Scylla's four 6-column halves map 1:1 onto the Preonic's top four rows.
 * The bottom row stands in for the Scylla's two 5-key thumb clusters:
 *
 *   Scylla left cluster        Scylla right cluster
 *   [LT2/<-][Spc][Bspc]        [GUI][Ent][LT1/->]
 *           [Dn/Del][C-S-C]    [C-S-V][Up/Prt]
 *
 * The Scylla keeps all four arrows on thumbs (<- and -> as the layer-key taps).
 * Here <- and -> get dedicated keys at cols 1 and 10, so the layer keys are
 * plain MO() holds rather than LT() -- no tap keycode to misfire.
 *
 * Because the arrows now live on the base layer, the bottom row is KC_TRNS on
 * layers 1 and 2 rather than the Scylla's KC_NO, so arrows stay live while a
 * layer is held. Ctrl+End still overrides col 11 on layer 2.
 */

enum {
    TD_DOWN_DEL,
    TD_UP_PSCR,
};

void dance_up_scrot(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            tap_code(KC_UP);
            break;
        case 2:
            register_code(KC_LGUI);
            register_code(KC_LSFT);
            register_code(KC_PSCR);
            clear_keyboard();
            break;
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_DOWN_DEL] = ACTION_TAP_DANCE_DOUBLE(KC_DOWN, KC_DEL),
    [TD_UP_PSCR]  = ACTION_TAP_DANCE_FN(dance_up_scrot),
};

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
        case RCTL_T(KC_ESC):
        case RALT_T(KC_BSLS):
            return true;
        default:
            return false;
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Base
 * ,-----------------------------------------------------------------------------------------------------.
 * |  F1   |  F2  |  F3   |  F4  |  F5  |  F6  |  F7  |  F8  |  F9    |  F10   |  F11  |  F12  |
 * | Tab   |  Q   |  W    |  D   |  F   |  K   |  J   |  U   |  R     |  L     |  ;    | \/RAlt|
 * |Esc/Ctl|  A   |  S    |  E   |  T   |  G   |  Y   |  N   |  I     |  O     |  H    | '/Ctl |
 * | Shift |  Z   |  X    |  C   |  V   |  B   |  P   |  M   |  ,     |  .     |  /    | Shift |
 * | C-S-C | Left | Dn/Del| MO2  | Spc  | Bspc | GUI  | Ent  | MO1    | Up/Prt | Right | C-S-V |
 * `-----------------------------------------------------------------------------------------------------'
 */
[0] = LAYOUT_ortho_5x12(
    KC_F1,          KC_F2,    KC_F3,            KC_F4,          KC_F5,   KC_F6,   KC_F7,   KC_F8,  KC_F9,           KC_F10,          KC_F11,  KC_F12,
    KC_TAB,         KC_Q,     KC_W,             KC_D,           KC_F,    KC_K,    KC_J,    KC_U,   KC_R,            KC_L,            KC_SCLN, RALT_T(KC_BSLS),
    RCTL_T(KC_ESC), KC_A,     KC_S,             KC_E,           KC_T,    KC_G,    KC_Y,    KC_N,   KC_I,            KC_O,            KC_H,    LCTL_T(KC_QUOT),
    KC_LSFT,        KC_Z,     KC_X,             KC_C,           KC_V,    KC_B,    KC_P,    KC_M,   KC_COMM,         KC_DOT,          KC_SLSH, KC_RSFT,
    RCS(KC_C),      KC_LEFT,  TD(TD_DOWN_DEL),  MO(2),          KC_SPC,  KC_BSPC, KC_LGUI, KC_ENT, MO(1),           TD(TD_UP_PSCR),  KC_RGHT, RCS(KC_V)
),

/* Layer 1 -- held on right thumb (col 8), symbols under the left hand */
[1] = LAYOUT_ortho_5x12(
    KC_NO,   KC_NO,   KC_EQL,  KC_PLUS, KC_TILD, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
    KC_TRNS, CW_TOGG, KC_EXLM, KC_AT,   KC_HASH, KC_LBRC, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
    KC_TRNS, KC_MINS, KC_DLR,  KC_PERC, KC_CIRC, KC_LPRN, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_TRNS,
    KC_NO,   KC_UNDS, KC_AMPR, KC_ASTR, KC_GRV,  KC_LCBR, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
),

/* Layer 2 -- held on left thumb (col 3), numpad under the right hand */
[2] = LAYOUT_ortho_5x12(
    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   UG_TOGG, UG_NEXT, UG_HUEU, UG_SATU, UG_VALU, UG_VALD,
    KC_TRNS, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_RBRC, KC_7,    KC_8,    KC_9,    KC_NO,   KC_NO,
    KC_TRNS, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_RPRN, KC_4,    KC_5,    KC_6,    KC_0,    KC_TRNS,
    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_RCBR, KC_1,    KC_2,    KC_3,    KC_NO,   KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, LCTL(KC_END)
)

};
