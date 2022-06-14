/* Copyright 2021 TweetyDaBird
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
#define _GAME 1
#define _LOWER 2
#define _RAISE 3
#define _ADJUST 16

// Simple custom keycodes
enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  GAME,
  LOWER,
  RAISE,
  BACKLIT,
  EXT_PLV,
  LOWERED_GUI // GUI + LOWER, especially useful for GUI+Number shortcuts
};

// Tap Dances START

// Tap Dance Keycodes
enum tap_dances {
    TD_ALT_LOWEREDALT,
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

// Determine the tapdance state to return
td_state_t cur_dance(qk_tap_dance_state_t *state) {
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

void alt_loweredalt_finished(qk_tap_dance_state_t *state, void *user_data) {
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

void alt_loweredalt_reset(qk_tap_dance_state_t *state, void *user_data) {
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

void hreset_finished(qk_tap_dance_state_t *state, void *user_data) {
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
qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_ALT_LOWEREDALT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, alt_loweredalt_finished, alt_loweredalt_reset),
    [TD_HRESET] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, hreset_finished, NULL, 2000), // Delayed reset so I stop pressing it by accident
};

// Tap Dances END

/* Keymap for reference
 * The outermost key on the last row is the encoder press!
 * ,-----------------------------------------.                    ,------------------------------------------.
 * |      |      |      |      |      |      |   ___        ___   |      |      |      |      |      |       |
 * |------+------+------+------+------+------|  /   \      /   \  |------+------+------+------+------+-------|
 * | Esc  |   Q  |   W  |   E  |   R  |   T  | ( ENC )    ( ENC ) |   Y  |   U  |   I  |   O  |   P  | Bksp  |
 * |------+------+------+------+------+------|  \___/      \___/  |------+------+------+------+------+-------|
 * | Tab  |   A  |   S  |   D  |   F  |   G  |                    |   H  |   J  |   K  |   L  |   ;  |   '   |
 * |------+------+------+------+------+------|-------.    ,-------|------+------+------+------+------+-------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |       |    |       |   N  |   M  |   ,  |   .  |   /  | Enter |
 * `-----------------------------------------|-------/    \-------|------------------------------------------'
 *                    | Win  | Alt  | Lower / Space /      \ Ctrl  \ | Raise|AltGr |      |
 *                    | Gui  |Lowalt|      |       /        \       \|      |      |      |
 *                    `-------------------'-------'          '-------'--------------------'
 */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_QWERTY] = LAYOUT(

   XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX,           XXXXXXX, XXXXXXX,                             XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,
   KC_ESC,  KC_Q,    KC_W,      KC_E,              KC_R,    KC_T,                             KC_Y,    KC_U,      KC_I,    KC_O,     KC_P,     KC_BSPC,
   KC_TAB,  KC_A,    KC_S,      KC_D,              KC_F,    KC_G,                             KC_H,    KC_J,      KC_K,    KC_L,     KC_SCLN,  KC_QUOT,
   KC_LSFT, KC_Z,    KC_X,      KC_C,              KC_V,    KC_B, XXXXXXX,          XXXXXXX,  KC_N,    KC_M,      KC_COMM, KC_DOT,   KC_SLSH,  RSFT_T(KC_ENT),
     XXXXXXX, KC_LGUI,   TD(TD_ALT_LOWEREDALT),    KC_SPC,  LOWER,      KC_RCTRL, RAISE, KC_RALT,  XXXXXXX,   XXXXXXX
),

[_GAME] = LAYOUT(
   KC_GRV,  KC_1,    KC_2,      KC_3,              KC_4,    KC_5,                             KC_6,    KC_7,      KC_8,    KC_9,   KC_0,    KC_MINS,
   KC_ESC,  KC_Q,    KC_W,      KC_E,              KC_R,    KC_T,                             KC_Y,    KC_U,      KC_I,    KC_O,     KC_P,     KC_BSPC,
   KC_TAB,  KC_A,    KC_S,      KC_D,              KC_F,    KC_G,                             KC_H,    KC_J,      KC_K,    KC_L,     KC_SCLN,  KC_QUOT,
   KC_LSFT, KC_Z,    KC_X,      KC_C,              KC_V,    KC_B, XXXXXXX,          XXXXXXX,  KC_N,    KC_M,      KC_COMM, KC_DOT,   KC_SLSH,  KC_ENT,
                       XXXXXXX, KC_LGUI, KC_LALT, KC_SPC, LOWER,                    KC_RCTRL, RAISE, KC_RALT,  XXXXXXX,   XXXXXXX
),

[_LOWER] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                         _______, _______, _______, _______, _______, _______,
   KC_GRV, KC_1   , KC_2   , KC_3   , KC_4,    KC_5   ,                         KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , _______,
  _______, KC_F1  , KC_F2  , KC_F3  , KC_F4,   KC_F5,                           KC_F6  , KC_4   , KC_5   , KC_6   , KC_DOT , KC_ASTR,
  _______, KC_F7  , KC_F8  , KC_F9  , KC_F10,  KC_F11, _______,        KC_0,    KC_F12 , KC_1   , KC_2   , KC_3   , KC_SLSH, KC_PLUS,
                    _______, _______, _______, KC_0  , _______,       _______, _______, _______, _______, _______
),

[_RAISE] = LAYOUT(
  _______, _______, _______,  _______, _______, _______,                         _______, _______, _______, _______, _______, _______,
  _______, _______, KC_LCBR,  KC_RCBR, KC_EQL, _______,                          _______, KC_MINS, KC_LPRN, KC_RPRN, _______, KC_DEL,
  KC_DEL, KC_PAUSE, _______,  KC_PGUP, KC_HOME, KC_RBRC,                         KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, KC_PIPE, KC_GRV,
  _______, KC_PSCR, KC_PAST,  KC_PGDN,  KC_END,  KC_EQL, _______,       _______, KC_MINS,  KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS, _______,
                     KC_APP,  _______, _______, KC_INS,  _______,       _______, _______, _______, _______, _______
),

[_ADJUST] = LAYOUT(
  XXXXXXX, XXXXXXX,  XXXXXXX ,  XXXXXXX , XXXXXXX, XXXXXXX,                     RGB_TOG, RGB_MOD, RGB_VAI, RGB_VAD, RGB_HUI, RGB_HUD,
  KC_CAPS  , QWERTY,   XXXXXXX,XXXXXXX,TD(TD_HRESET),XXXXXXX,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, GAME,XXXXXXX, XXXXXXX,    XXXXXXX,  XXXXXXX,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_MPRV,  KC_MNXT, XXXXXXX,     KC_MPRV, KC_MNXT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                   _______, _______, _______, KC_MPLY, _______,     KC_MPLY, _______, _______, _______, _______ \
  )
};

#ifdef OLED_ENABLE

static void render_logo(void) {
    static const char PROGMEM qmk_logo[] = {
        0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
        0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
        0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0
    };

    oled_write_P(qmk_logo, false);
}

static void print_status_narrow(void) {
    // Print current mode
    oled_write_P(PSTR("\n"), false);
  oled_write_P(PSTR(""), false);
    oled_write_P(PSTR("Lotus -58-"), false);
  oled_write_P(PSTR("\n"), false);

    switch (get_highest_layer(default_layer_state)) {
        case _QWERTY:
            oled_write_P(PSTR("Qwrty"), false);
            break;
        default:
            oled_write_P(PSTR("Undef"), false);
    }
    oled_write_P(PSTR("\n"), false);
    // Print current layer
    oled_write_P(PSTR("Layer"), false);
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write_P(PSTR("-Base\n"), false);
            break;
        case _FUNC:
            oled_write_P(PSTR("-Func\n"), false);
            break;
        case _NUM:
            oled_write_P(PSTR("-Num \n"), false);
            break;
        case _SYSTEM:
            oled_write_P(PSTR("-Sys \n"), false);
            break;
        default:
            oled_write_P(PSTR("Undef"), false);
    }
    oled_write_P(PSTR("\n"), false);
    led_t led_usb_state = host_keyboard_led_state();
    oled_write_ln_P(PSTR("CPSLK"), led_usb_state.caps_lock);
    oled_write_P(PSTR("\n"), false);
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_270;
    }
    return rotation;
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        print_status_narrow();
    } else {
        render_logo();
    }

    return false;
}

#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_QWERTY);
      }
      return false;
      break;

    case GAME:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_GAME);
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

void rgb_matrix_indicators_kb(void) {
    switch(get_highest_layer(layer_state | default_layer_state)) {
        case _LOWER:
            rgb_matrix_set_color(25, RGB_BLUE); // LOWER
            break;
        case _RAISE:
            rgb_matrix_set_color(55, RGB_BLUE); // RAISE
            break;
        case _ADJUST:
            rgb_matrix_set_color(25, RGB_PURPLE); // LOWER
            rgb_matrix_set_color(55, RGB_PURPLE); // RAISE
            switch (biton32(default_layer_state)) {
                case _QWERTY:
                    rgb_matrix_set_color(5, RGB_GREEN);
                    rgb_matrix_set_color(6, 32, 32, 32);
                    break;
                case _GAME:
                    rgb_matrix_set_color(5, 32, 32, 32);
                    rgb_matrix_set_color(6, RGB_RED);
                    break;
            }
            break;
        default:
            break;
    }
}

#ifdef ENCODER_ENABLE
static fast_timer_t last_encoding_time = 0;
static const fast_timer_t ENCODER_DEBOUNCE = 10;
static const fast_timer_t ENCODER_DIRECTION_CHANGE_DEBOUNCE = 200;
static bool last_encoder_clockwise = true;

bool encoder_update_user(uint8_t index, bool clockwise) {
    // Adapted and changed from https://www.reddit.com/r/olkb/comments/tz4tek/encoder_debouncing_is_easy/
    fast_timer_t now = timer_read_fast();
    if (TIMER_DIFF_FAST(now, last_encoding_time) < ENCODER_DEBOUNCE) {
        return false;
    }
    if (clockwise != last_encoder_clockwise && TIMER_DIFF_FAST(now, last_encoding_time) < ENCODER_DIRECTION_CHANGE_DEBOUNCE) {
        return false;
    }
    last_encoding_time = timer_read_fast();
    last_encoder_clockwise = clockwise;

    if (clockwise) {
        tap_code(KC_MS_WH_DOWN);
    } else {
        tap_code(KC_MS_WH_UP);
    }
    return true;
}

#endif
