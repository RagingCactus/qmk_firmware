#include QMK_KEYBOARD_H
#include "eeconfig.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[0] = LAYOUT(

   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,
   KC_ESC,  KC_Q,    KC_W,      KC_E,                     KC_R,    KC_T,  KC_Y,    KC_U,             KC_I,    KC_O,     KC_P,     KC_BSPC,
   KC_TAB,  KC_A,    KC_S,      KC_D,                     KC_F,    KC_G,  KC_H,    KC_J,             KC_K,    KC_L,     KC_SCLN,  KC_QUOT,
   KC_LCTL, KC_Z,    KC_X,      KC_C,                     KC_V,    KC_B, XXXXXXX,   XXXXXXX, KC_N,    KC_M,             KC_COMM, KC_DOT,   KC_SLSH,  KC_RCTL,
                         XXXXXXX,         KC_LSFT, KC_SPC, XXXXXXX,    XXXXXXX,  KC_RSFT, RALT_T(KC_ENT),   XXXXXXX
)
};

bool rgb_matrix_indicators_user(void) {
    rgb_matrix_set_color(0, RGB_GREEN); // 5 for DEBUG (left half -> green)
    rgb_matrix_set_color(3, RGB_GREEN); // 2 for DEBUG (left half -> green)
    rgb_matrix_set_color(9, RGB_GREEN); // E for DEBUG (left half -> green)
    rgb_matrix_set_color(29, RGB_RED); // 6 for DEBUG (right half -> red)
    rgb_matrix_set_color(33, RGB_RED); // 0 for DEBUG (right half -> red)
    rgb_matrix_set_color(37, RGB_RED); // O for DEBUG (right half -> red)
    return false;
}
