/* Copyright 2021-2022 TweetyDaBird, Marius Renner
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "eeconfig.h"

#define _QWERTY 0
#define _NUMQWERTY 1
#define _GAME 2
#define _LOWER 4
#define _LOWERNUM 5
#define _RAISE 6
#define _ADJUST 7

// Simple custom keycodes
enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  NUMQWERTY,
  GAME,
  LOWER,
  RAISE,
  LOWERNUM,
  BACKLIT,
  EXT_PLV,
  LOWERED_GUI // GUI + LOWER, especially useful for GUI+Number shortcuts
};

// Tap Dances START

// Tap Dance Keycodes
enum tap_dances {
    TD_ALT_LOWEREDALT,
    TD_GUI_LOWEREDGUI,
    TD_HRESET, // Hold reset
};

// Tap Dance states
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_SINGLE_TAP,
    TD_DOUBLE_HOLD
} td_state_t;

typedef struct {
    bool       did_enable_layer;
    td_state_t state;
} td_tap_t;

// Create a global instance of the tapdance state type
static td_tap_t td_lalt_state = {
    .did_enable_layer = false,
    .state = TD_NONE
};

static td_tap_t td_lgui_state = {
    .did_enable_layer = false,
    .state = TD_NONE
};

// Determine the tapdance state to return
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (!state->pressed) return TD_DOUBLE_SINGLE_TAP;
        else return TD_DOUBLE_HOLD;
    }

    else return TD_UNKNOWN; // Any number higher than the maximum state value you return above
}

void alt_loweredalt_finished(tap_dance_state_t *state, void *user_data) {
    td_lalt_state.state = cur_dance(state);
    switch (td_lalt_state.state) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_HOLD:
            register_code16(KC_LALT);
            break;
        case TD_DOUBLE_SINGLE_TAP:
            tap_code16(KC_LALT);
            register_code16(KC_LALT);
            break;
        case TD_DOUBLE_HOLD:
            register_code16(KC_LALT);
            if (!layer_state_is(_LOWER)) {
                layer_on(_LOWER);
                td_lalt_state.did_enable_layer = true;
            }
            update_tri_layer(_LOWER, _RAISE, _ADJUST);
            break;
        default:
            break;
    }
}

void alt_loweredalt_reset(tap_dance_state_t *state, void *user_data) {
    td_lalt_state.state = cur_dance(state);
    switch (td_lalt_state.state) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_HOLD:
        case TD_DOUBLE_SINGLE_TAP:
            unregister_code16(KC_LALT);
            break;
        case TD_DOUBLE_HOLD:
            unregister_code16(KC_LALT);
            break;
        default:
            break;
    }

    if (td_lalt_state.did_enable_layer) {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
        td_lalt_state.did_enable_layer = false;
    }
}

void gui_loweredgui_finished(tap_dance_state_t *state, void *user_data) {
    td_lgui_state.state = cur_dance(state);
    switch (td_lgui_state.state) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_HOLD:
            register_code16(KC_LGUI);
            break;
        case TD_DOUBLE_SINGLE_TAP:
            tap_code16(KC_LGUI);
            register_code16(KC_LGUI);
            break;
        case TD_DOUBLE_HOLD:
            register_code16(KC_LGUI);
            if (!layer_state_is(_LOWER)) {
                layer_on(_LOWER);
                td_lgui_state.did_enable_layer = true;
            }
            update_tri_layer(_LOWER, _RAISE, _ADJUST);
            break;
        default:
            break;
    }
}

void gui_loweredgui_reset(tap_dance_state_t *state, void *user_data) {
    td_lgui_state.state = cur_dance(state);
    switch (td_lgui_state.state) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_HOLD:
        case TD_DOUBLE_SINGLE_TAP:
            unregister_code16(KC_LGUI);
            break;
        case TD_DOUBLE_HOLD:
            unregister_code16(KC_LGUI);
            break;
        default:
            break;
    }

    if (td_lgui_state.did_enable_layer) {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
        td_lgui_state.did_enable_layer = false;
    }
}

void hreset_finished(tap_dance_state_t *state, void *user_data) {
    td_state_t tap_state = cur_dance(state);
    switch (tap_state) {
        case TD_SINGLE_TAP:
            break;
        case TD_SINGLE_HOLD:
            reset_keyboard();
            break;
        case TD_DOUBLE_SINGLE_TAP:
        case TD_DOUBLE_HOLD:
        default:
            break;
    }
}

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    [TD_ALT_LOWEREDALT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, alt_loweredalt_finished, alt_loweredalt_reset),
    [TD_GUI_LOWEREDGUI] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, gui_loweredgui_finished, gui_loweredgui_reset),
    [TD_HRESET] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, hreset_finished, NULL),
};

// Tap Dances END

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // Delayed reset so I stop pressing it by accident
        case TD(TD_HRESET):
            return 2000;
        default:
            return TAPPING_TERM;
    }
}

/* Keymap for reference
 * ,-----------------------------------------.                    ,------------------------------------------.
 * |      |      |      |      |      |      |   ___        ___   |      |      |      |      |      |       |
 * |------+------+------+------+------+------|  /   \      /   \  |------+------+------+------+------+-------|
 * | Esc  |   Q  |   W  |   E  |   R  |   T  | ( ENC )    ( ENC ) |   Y  |   U  |   I  |   O  |   P  | Bksp  |
 * |------+------+------+------+------+------|  \___/      \___/  |------+------+------+------+------+-------|
 * | Tab  |   A  |   S  |   D  |   F  |   G  |                    |   H  |   J  |   K  |   L  |   ;  |   '   |
 * |------+------+------+------+------+------|-------.    ,-------|------+------+------+------+------+-------|
 * | Ctrl |   Z  |   X  |   C  |   V  |   B  |GUI/Win|    |       |   N  |   M  |   ,  |   .  |   /  | Ctrl  |
 * `-----------------------------------------|-------/    \-------|------------------------------------------'
 *                    | Alt  | Shift | Space / LOWER /     \ Raise \ | Shift|AltGr | Win  |
 *                    | TDALT|       /     |       /        \       \|      |Enter | Gui  |
 *                    `-------------------'-------'          '-------'--------------------'
 */

#define TDLGUI TD(TD_GUI_LOWEREDGUI)
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_QWERTY] = LAYOUT(

   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,
   KC_ESC,  KC_Q,    KC_W,      KC_E,                     KC_R,    KC_T,  KC_Y,    KC_U,             KC_I,    KC_O,     KC_P,     KC_BSPC,
   KC_TAB,  KC_A,    KC_S,      KC_D,                     KC_F,    KC_G,  KC_H,    KC_J,             KC_K,    KC_L,     KC_SCLN,  KC_QUOT,
   KC_LCTL, KC_Z,    KC_X,      KC_C,                     KC_V,    KC_B, TDLGUI,   TDLGUI, KC_N,    KC_M,             KC_COMM, KC_DOT,   KC_SLSH,  KC_RCTL,
                         TD(TD_ALT_LOWEREDALT),         KC_LSFT, KC_SPC, LOWER,    RAISE,  KC_RSFT, RALT_T(KC_ENT),   TDLGUI
),

[_NUMQWERTY] = LAYOUT(
   KC_GRV,  KC_1,    KC_2,      KC_3, KC_4,    KC_5,     _______,      _______,  KC_6,    KC_7,      KC_8,    KC_9,   KC_0,    KC_MINS,
  _______, _______, _______, _______, _______, _______,                         _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______,   _______,                       _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______,  _______, _______,      _______, _______, _______, _______, _______, _______, _______,
                             _______, _______, _______,  LOWERNUM,     _______, _______, _______, _______
),

[_GAME] = LAYOUT(
   KC_GRV,  KC_1,    KC_2,      KC_3,              KC_4,    KC_5,  _______,         _______,  KC_6,    KC_7,      KC_8,    KC_9,   KC_0,    KC_MINS,
   KC_ESC,  KC_Q,    KC_W,      KC_E,              KC_R,    KC_T,                             KC_Y,    KC_U,      KC_I,    KC_O,     KC_P,     KC_BSPC,
   KC_TAB,  KC_A,    KC_S,      KC_D,              KC_F,    KC_G,                             KC_H,    KC_J,      KC_K,    KC_L,     KC_SCLN,  KC_QUOT,
   KC_LCTL, KC_Z,    KC_X,      KC_C,              KC_V,    KC_B, KC_LGUI,          KC_RGUI,  KC_N,    KC_M,      KC_COMM, KC_DOT,   KC_SLSH,  KC_RCTL,
                         KC_LALT, KC_LSFT, KC_SPC, LOWERNUM,                       RAISE   , KC_RSFT, KC_ENT,   KC_RGUI
),

[_LOWER] = LAYOUT(
  _______, _______, _______, _______, _______, _______,  _______,  _______,    _______, _______, _______, _______, _______, _______,
   KC_GRV, KC_1   , KC_2   , KC_3   , KC_4,    KC_5   ,                         KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , _______,
  _______, KC_F1  , KC_F2  , KC_F3  , KC_F4,   KC_F5,                           KC_F6  , KC_4   , KC_5   , KC_6   , KC_DOT , KC_ASTR,
  _______, KC_F7  , KC_F8  , KC_F9  , KC_F10,  KC_F11, _______,        _______, KC_F12 , KC_1   , KC_2   , KC_3   , KC_0   , KC_PLUS,
                             _______, _______, KC_0,  _______,       _______, _______, _______, _______
),

[_LOWERNUM] = LAYOUT(
  _______, KC_6   , KC_7   , KC_8   , KC_9   , KC_0   ,  _______,  _______,     _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______,                         _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______,   _______,                       _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______,  _______, _______,        _______,    _______, _______, _______, _______, _______, _______,
                             _______, _______, _______,  _______,       _______, _______, _______, _______
),


[_RAISE] = LAYOUT(
  _______, _______, _______,  _______, _______, _______,  _______,  _______,     _______, _______, _______, _______, _______, _______,
  _______, _______, KC_LCBR,  KC_RCBR, KC_EQL, _______,                          KC_LCBR, KC_RCBR, KC_LPRN, KC_RPRN, _______, KC_DEL,
  KC_DEL, KC_PAUSE, _______,  KC_PGUP, KC_HOME, _______,                         KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, KC_PIPE, KC_GRV,
  _______, KC_PSCR, _______,  KC_PGDN,  KC_END,  _______, _______,      _______, KC_MINS,  KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS, _______,
                               _______, _______, KC_INS, _______,       _______, _______, _______, _______
),

[_ADJUST] = LAYOUT(
  XXXXXXX, NUMQWERTY,  XXXXXXX ,  XXXXXXX , XXXXXXX, XXXXXXX, _______,  _______, RGB_TOG, RGB_MOD, RGB_VAI, RGB_VAD, RGB_HUI, RGB_HUD,
  XXXXXXX ,QWERTY,   XXXXXXX, XXXXXXX,TD(TD_HRESET),XXXXXXX,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  KC_CAPS, GAME,     XXXXXXX, XXXXXXX,    KC_MPLY,  KC_MPRV,                 KC_MNXT, KC_MPLY, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_MPRV,  KC_MNXT, KC_APP,     KC_APP, KC_MPRV, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                            _______, _______, KC_MPLY, _______,            _______, KC_MPLY, _______, _______          \
  )
};

#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    oled_scroll_set_speed(6);
    // I only have an OLED on the left side
    return OLED_ROTATION_270;
}

bool oled_task_user(void) {
    // 'lotus', 32x32px
    static const char PROGMEM lotus_picture[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xe0, 0x70,
        0x70, 0xe0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc8, 0xff, 0xe3, 0x03, 0x03, 0x06, 0xfe, 0xff, 0x07, 0x01, 0x00,
        0x00, 0x01, 0x07, 0xff, 0xfe, 0x06, 0x03, 0x03, 0xe3, 0xff, 0xc8, 0xc0, 0xc0, 0xc0, 0xc0, 0x80,
        0x80, 0xc3, 0xef, 0x7c, 0x78, 0x70, 0xe1, 0xc7, 0xcf, 0x9c, 0xb8, 0xf3, 0xff, 0xfc, 0xf0, 0xc0,
        0xc0, 0xf0, 0xfc, 0xff, 0xf3, 0xb8, 0x9c, 0xcf, 0xc7, 0xe1, 0x70, 0x78, 0x7c, 0xef, 0xc3, 0x80,
        0x01, 0x03, 0x03, 0x06, 0x06, 0x0c, 0x0c, 0x0c, 0x0c, 0x0d, 0x0d, 0x0d, 0x07, 0x07, 0x03, 0x03,
        0x03, 0x03, 0x07, 0x07, 0x0d, 0x0d, 0x0d, 0x0c, 0x0c, 0x0c, 0x0c, 0x06, 0x06, 0x03, 0x03, 0x01
    };

    oled_write_raw_P(lotus_picture, sizeof(lotus_picture));
    #ifdef __AVR__
        oled_set_cursor(0, 7);
        oled_write_ln_P(PSTR("Lotus "), false);
        oled_write_ln_P(PSTR(" -58- "), false);
        oled_write_ln_P(PSTR(" Glow "), false);
        oled_set_cursor(0, 14);
    #else
        oled_set_cursor(0, 5);
        oled_write_ln_P(PSTR("Lotus\n"), false);
        oled_write_ln_P(PSTR(" -58- \n"), false);
        oled_write_ln_P(PSTR("Glow "), false);
        oled_set_cursor(0, 13);
    #endif
    switch (get_highest_layer(default_layer_state)) {
        case _QWERTY:
            oled_write_ln_P(PSTR("NORM"), false);
            break;
        case _NUMQWERTY:
            oled_write_ln_P(PSTR("NUM"), false);
            break;
        case _GAME:
            oled_write_ln_P(PSTR("GAME"), false);
            break;
    }
    return false;
}

#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_QWERTY);

        #ifdef OLED_ENABLE
            oled_clear();
        #endif
      }
      return false;
      break;

    case NUMQWERTY:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_NUMQWERTY);

        #ifdef OLED_ENABLE
            oled_clear();
        #endif
      }
      return false;
      break;

    case GAME:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_GAME);

        #ifdef OLED_ENABLE
            oled_clear();
        #endif
      }
      return false;
      break;

    case LOWER:
        if (record->event.pressed) {
          layer_on(_LOWER);
          update_tri_layer(_LOWER, _RAISE, _ADJUST);
        } else {
          layer_off(_LOWER);
          update_tri_layer(_LOWER, _RAISE, _ADJUST);
        }
      return false;
      break;

    case LOWERNUM:
        if (record->event.pressed) {
          layer_on(_LOWER);
          layer_on(_LOWERNUM);
          update_tri_layer(_LOWER, _RAISE, _ADJUST);
        } else {
          layer_off(_LOWER);
          layer_off(_LOWERNUM);
          update_tri_layer(_LOWER, _RAISE, _ADJUST);
        }
      return false;
      break;

    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;

    case BACKLIT:
      if (record->event.pressed) {
        register_code(KC_RSFT);
        #ifdef BACKLIGHT_ENABLE
          backlight_step();
        #endif
      } else {
        unregister_code(KC_RSFT);
      }
      return false;
      break;

    case LOWERED_GUI:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
        register_code(KC_LGUI);
      } else {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
        unregister_code(KC_LGUI);
      }
      return false;
      break;
    }
  return true;
}

bool rgb_matrix_indicators_user(void) {
// bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t val = rgb_matrix_get_val();

    HSV blue_scaled_hsv = { HSV_BLUE };
    blue_scaled_hsv.v = val;
    RGB blue_scaled_rgb;

    HSV purple_scaled_hsv = { HSV_PURPLE };
    purple_scaled_hsv.v = val;
    RGB purple_scaled_rgb;

    switch(get_highest_layer(layer_state | default_layer_state)) {
        case _LOWER:
        case _LOWERNUM: // modifier on the left half
            blue_scaled_rgb = hsv_to_rgb(blue_scaled_hsv);
            rgb_matrix_set_color(25, blue_scaled_rgb.r, blue_scaled_rgb.g, blue_scaled_rgb.b); // LOWER

            // DEBUG
            rgb_matrix_set_color(29, RGB_RED); // 6 for DEBUG (right half -> red)
            rgb_matrix_set_color(4, RGB_GREEN); // 1 for DEBUG (left half -> green)
            rgb_matrix_set_color(8, RGB_GREEN); // W for DEBUG (left half -> green)
            break;
        case _RAISE: // modifier on the right half
            blue_scaled_rgb = hsv_to_rgb(blue_scaled_hsv);
            rgb_matrix_set_color(54, blue_scaled_rgb.r, blue_scaled_rgb.g, blue_scaled_rgb.b); // RAISE

            // DEBUG
            rgb_matrix_set_color(0, RGB_GREEN); // 6 for DEBUG (left half -> green)
            rgb_matrix_set_color(33, RGB_RED); // 0 for DEBUG (right half -> red)
            rgb_matrix_set_color(37, RGB_RED); // O for DEBUG (right half -> red)
            break;
        case _ADJUST:
            purple_scaled_rgb = hsv_to_rgb(purple_scaled_hsv);
            rgb_matrix_set_color(25, purple_scaled_rgb.r, purple_scaled_rgb.g, purple_scaled_rgb.b); // LOWER
            rgb_matrix_set_color(54, purple_scaled_rgb.r, purple_scaled_rgb.g, purple_scaled_rgb.b); // RAISE
            switch (get_highest_layer(default_layer_state)) {
                case _QWERTY:
                    rgb_matrix_set_color(4, 32, 32, 32);
                    rgb_matrix_set_color(7, RGB_GREEN);
                    rgb_matrix_set_color(16, 32, 32, 32);
                    break;
                case _NUMQWERTY:
                    rgb_matrix_set_color(4, RGB_YELLOW);
                    rgb_matrix_set_color(7, 32, 32, 32);
                    rgb_matrix_set_color(16, 32, 32, 32);
                    break;
                case _GAME:
                    rgb_matrix_set_color(4, 32, 32, 32);
                    rgb_matrix_set_color(7, 32, 32, 32);
                    rgb_matrix_set_color(16, RGB_RED);
                    break;
            }
            break;
        default:
            break;
    }

    return false;
}

// void keyboard_post_init_user(void) {
//   // Customise these values to desired behaviour
//   debug_enable=true;
//   debug_matrix=true;
//   debug_keyboard=true;
//   debug_mouse=true;
// }

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    switch(get_highest_layer(layer_state)) {
        case _LOWER:
        case _LOWERNUM:
        case _RAISE:
        case _ADJUST:
            if (clockwise) {
                tap_code(KC_AUDIO_VOL_UP);
            } else {
                tap_code(KC_AUDIO_VOL_DOWN);
            }
            break;
        default:
            if (clockwise) {
                tap_code(KC_MS_WH_DOWN);
            } else {
                tap_code(KC_MS_WH_UP);
            }
            break;
    }
    return false;
}

#endif
