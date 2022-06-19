/*
   Based on http://www.keyboard-layout-editor.com/#/gists/c6c0ac051b2b118a34ef84ebadab54c7
   https://github.com/qmk/qmk_firmware/blob/master/layouts/community/ortho_4x12/crs/keymap.c
*/
#include QMK_KEYBOARD_H

#include "action_layer.h"
#ifdef AUDIO_ENABLE
  #include "audio.h"
#endif
#include "eeconfig.h"

extern keymap_config_t keymap_config;

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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Esc  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Tab  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl |LOWGUI|  GUI | TDALT| Lower|    Space    |Raise |AltGr | Win  |      | Ctrl |
 * `-----------------------------------------------------------------------------------'
 * TODO: consider moving a layer for meta stuff to the left of RCTL and use adjust as a normal functional layer
 */
[_QWERTY] = LAYOUT_planck_mit( \
  KC_ESC,  KC_Q,        KC_W,    KC_E,                  KC_R,   KC_T,    KC_Y,    KC_U,   KC_I,    KC_O,     KC_P,     KC_BSPC, \
  KC_TAB,  KC_A,        KC_S,    KC_D,                  KC_F,   KC_G,    KC_H,    KC_J,   KC_K,    KC_L,     KC_SCLN,  KC_QUOT, \
  KC_LSFT, KC_Z,        KC_X,    KC_C,                  KC_V,   KC_B,    KC_N,    KC_M,   KC_COMM, KC_DOT,   KC_SLSH,  RSFT_T(KC_ENT), \
  KC_LCTL, LOWERED_GUI, KC_LGUI, TD(TD_ALT_LOWEREDALT), LOWER,      KC_SPC,       RAISE,  KC_RALT, KC_RGUI,  _______,  KC_RCTL \
),

/* Game (Qwerty with no tapdances and stuff)
 * ,-----------------------------------------------------------------------------------.
 * | Esc  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Tab  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl |LOWGUI|  GUI | Alt  | Lower|    Space    |Raise |AltGr | Win  |      | Ctrl |
 * `-----------------------------------------------------------------------------------'
 * TODO: consider moving a layer for meta stuff to the left of RCTL and use adjust as a normal functional layer
 */
[_GAME] = LAYOUT_planck_mit( \
  KC_ESC,  KC_Q,        KC_W,    KC_E,    KC_R,   KC_T,    KC_Y,    KC_U,   KC_I,    KC_O,     KC_P,     KC_BSPC, \
  KC_TAB,  KC_A,        KC_S,    KC_D,    KC_F,   KC_G,    KC_H,    KC_J,   KC_K,    KC_L,     KC_SCLN,  KC_QUOT, \
  KC_LSFT, KC_Z,        KC_X,    KC_C,    KC_V,   KC_B,    KC_N,    KC_M,   KC_COMM, KC_DOT,   KC_SLSH,  KC_ENT, \
  KC_LCTL, LOWERED_GUI, KC_LGUI, KC_LALT, LOWER,      KC_SPC,       RAISE,  KC_RALT, KC_RGUI,  _______,  KC_RCTL \
),

/* Lower   (switched to # because KP# were weird in terminal emulators)
 * ,-----------------------------------------------------------------------------------.
 * |  `~  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |  Tab |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   4  |   5  |   6  |   .  |  *   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |   1  |   2  |   3  |   0  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      0      |      |   -  |      | KP_+ |      |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = LAYOUT_planck_mit( \
   KC_GRV,   KC_1,     KC_2,    KC_3,    KC_4,    KC_5,    KC_6,     KC_7, KC_8,    KC_9,     KC_0,    _______, \
  _______,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,     KC_4, KC_5,    KC_6,     KC_DOT,  KC_ASTR, \
  _______,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,   KC_F12,    KC_1, KC_2,    KC_3,     KC_0   , _______, \
  _______, _______, _______, _______, _______,          KC_0,     _______, KC_MINS, _______,  _______, KC_PLUS  \
),


/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |      |      |  {   |  }   |  =   |      |      |  -   |  (   |  )   |      |  Del |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Del  | PAUSE|      | PGUP | Home |      | Left | Down |  Up  | Right|  |   |   `  |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |PRTSCR|      | PGDN | End  |      |  -   |  =   |  [   |  ]   |  \|  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      | App  | App  |      |      |     Ins     |      | Home | PGDN | PGUP |  End |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT_planck_mit( \
  _______,   _______,  KC_LCBR,  KC_RCBR, KC_EQL,   _______, _______, KC_MINS, KC_LPRN, KC_RPRN,  _______, KC_DEL, \
   KC_DEL,   KC_PAUSE, _______,  KC_PGUP, KC_HOME,  _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,  KC_PIPE, KC_GRV, \
  _______,   KC_PSCR,  _______,  KC_PGDN, KC_END,   _______, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC,  KC_BSLS, _______, \
  _______,   KC_APP,   KC_APP,   _______, _______,        KC_INS,     _______,  KC_HOME, KC_PGDN,  KC_PGUP, KC_END \
),

/* Adjust (Lower + Raise)
 * ,--------------------------------------------------------------------------------------.
 * | C-A-I|Qwerty|      |      |HRESET|      |      |RGB N |RGB P    |LED + | LED -|C-A-D |
 * |------+------+------+------+------+-------------+------+---------+------+------+------|
 * | Caps |GAME  |      |Aud on|Audoff|      |      |      | PrtSc   |ScrLck| Break|      |
 * |------+------+------+------+------+------|------+------+---------+------+------+------|
 * |      |Voice-|Voice+|Mus on|Musoff| Prev | Next | Mute | VolDn   | VolUp|      |      |
 * |------+------+------+------+------+------+------+------+---------+------+------+------|
 * | Brite|      |      |      |      | Play/Pause  |      | RGB_TOG |RGB_Mod|HUE+|HUE- |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] = LAYOUT_planck_mit( \
  LALT(LCTL(KC_INS)), QWERTY,  _______, _______, TD(TD_HRESET), _______, _______, RGB_MOD, RGB_RMOD, RGB_VAI,RGB_VAD, LALT(LCTL(KC_DEL)), \
  KC_CAPS,            GAME,    _______, AU_ON,   AU_OFF,        _______, _______, _______, KC_PSCR, KC_SLCK, KC_PAUS, _______, \
  _______,            MUV_DE,  MUV_IN,  MU_ON,   MU_OFF,        KC_MPRV, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______, \
  RGB_TOG,            _______, _______, _______, _______,            KC_MPLY,     _______, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD \
)
};

#ifdef AUDIO_ENABLE
float tone_qwerty[][2]     = SONG(QWERTY_SOUND);
float music_scale[][2]     = SONG(MUSIC_SCALE_SOUND);
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
            rgb_matrix_set_color(40, RGB_BLUE); // LOWER
            break;
        case _RAISE:
            rgb_matrix_set_color(42, RGB_BLUE); // RAISE
            break;
        case _ADJUST:
            rgb_matrix_set_color(40, RGB_PURPLE); // LOWER
            rgb_matrix_set_color(42, RGB_PURPLE); // RAISE
            switch (biton32(default_layer_state)) {
                case _QWERTY:
                    rgb_matrix_set_color(1, RGB_GREEN);
                    rgb_matrix_set_color(13, 32, 32, 32);
                    break;
                case _GAME:
                    rgb_matrix_set_color(1, 32, 32, 32);
                    rgb_matrix_set_color(13, RGB_RED);
                    break;
            }
            break;
        default:
            break;
    }
}
