/* Copyright 2020 Chris Mondok
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
#include "rgb_matrix.h"
#include "lib/lib8tion/lib8tion.h"

#define RGB_DISABLE_WHEN_USB_SUSPENDED true

enum layers {
  _querty,
  _fn,
  _game,
  _lighting,
};

enum custom_codes {
  _rgb_plain = SAFE_RANGE
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_querty] = LAYOUT_60_ansi(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
        KC_TAB,      KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        LT(1,KC_ESC),   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
        KC_LSFT,            KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
        KC_LCTL,   KC_LGUI,   KC_LALT,                       KC_SPC,                              KC_RALT,   KC_RGUI,   KC_APP,   KC_RCTL
    ),
    [_fn] = LAYOUT_60_ansi(
        KC_CAPS,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,
        XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_PGUP, KC_UP, KC_PGDN, KC_PSCR, XXXXXXX, XXXXXXX, KC_INS,
        KC_TRNS,       KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, TG(_game), KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_END, XXXXXXX, XXXXXXX,
        XXXXXXX,            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX,   XXXXXXX,   XXXXXXX,                      XXXXXXX,                              XXXXXXX,   TG(_lighting),   XXXXXXX,   XXXXXXX
    ),
    [_game] = LAYOUT_60_ansi(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
        KC_TAB,      KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        KC_ESC,   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
        KC_LSFT,            KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
        KC_LCTL,   KC_LGUI,   KC_LALT,                       KC_SPC,                              KC_RALT,   TG(_game),   KC_APP,     KC_RCTL
    ),
    [_lighting] = LAYOUT_60_ansi(
        RGB_TOG,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_VAD, RGB_VAI, XXXXXXX,
        XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_HUD, RGB_HUI, XXXXXXX,
        TG(_lighting),       XXXXXXX, RGB_SAI, XXXXXXX, XXXXXXX, XXXXXXX, RGB_HUI, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_LSFT,            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_RMOD, RGB_MOD, XXXXXXX, KC_RSFT,
        XXXXXXX,   XXXXXXX,   XXXXXXX,                      _rgb_plain,                              XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch(keycode) {
    case _rgb_plain:
      rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
      return false;
  }
  return true;
}

void suspend_power_down_user(void) {
    rgb_matrix_set_suspend_state(true);
}

void suspend_wakeup_init_user(void) {
    rgb_matrix_set_suspend_state(false);
}

void rgb_matrix_get_keypos(int index, keypos_t *keypos) {
  if(index < 14) {
    keypos->row = 0;
    keypos->col = index;
  } else if(index < 28) {
    keypos->row = 1;
    keypos->col = index - 14;
  } else if(index < 41) {
    // NO_LED in column 1
    keypos->row = 2;
    keypos->col = index == 28 ? 0 : index - 29 + 2;
  } else if(index < 53) {
    keypos->row = 3;
    // NO_LED in column 0 and 12
    keypos->col = index == 52 ? 13 : index - 41 + 1;
  } else if(index < 61) {
    keypos->row = 4;
    keypos->col = index - 53;
    // three NO_LED after 55
    if(index > 55) keypos->col += 3;
    // two NO_LED after 56
    if(index > 56) keypos->col += 2;
    // NO_LED after 58
    if(index > 58) keypos->col += 1;

  }
}

void color_key(int led_index, uint16_t keycode) {
  HSV hsv = rgb_matrix_config.hsv;
  uint16_t time = scale16by8(g_rgb_timer, rgb_matrix_config.speed / 8);

  switch (keycode) {
    case XXXXXXX:
      // black
      // don't turn off keys on the lighting layer.
      if(!layer_state_cmp(layer_state, _lighting)) {
        rgb_matrix_set_color(led_index, 0x00, 0x00, 0x00);
      }
      break;
    case KC_UP:
    case KC_LEFT:
    case KC_DOWN:
    case KC_RIGHT:
    case RGB_VAI:
    case RGB_VAD:
    case RGB_MOD:
    case RGB_RMOD:
    case TG(_lighting):
      // white
      rgb_matrix_set_color(led_index, hsv.v, hsv.v, hsv.v);
      break;
    case KC_PGUP:
    case KC_PGDN:
      // cyan
      rgb_matrix_set_color(led_index, 0x00, hsv.v, hsv.v);
      break;
    case KC_PSCR:
      // magenta
      rgb_matrix_set_color(led_index, hsv.v, hsv.v, 0x00);
      break;
    case KC_HOME:
    case KC_END:
      // yellow
      rgb_matrix_set_color(led_index, hsv.v, 0x00, hsv.v);
      break;
    case KC_MPLY:
    case KC_MPRV:
    case KC_MNXT:
      // red
      rgb_matrix_set_color(led_index, hsv.v, 0x00, 0x00);
      break;
    case TG(_game):
      // green
      rgb_matrix_set_color(led_index, 0x00, hsv.v, 0x00);
      break;
    case RGB_SAI:
      // pulsing saturation
      hsv.s = abs8(sin8(time) - 128) * 2;
      RGB rgb = hsv_to_rgb(hsv);
      rgb_matrix_set_color(30, rgb.r, rgb.b, rgb.b);
      break;
    case RGB_HUI:
      // rotating hue
      hsv.h = time % UINT8_MAX;
      rgb = hsv_to_rgb(hsv);
      rgb_matrix_set_color(34, rgb.r, rgb.b, rgb.b);
      break;
  }
}

void colorize_keys_by_keycode(void) {
  uint8_t layer = get_highest_layer(layer_state);
  keypos_t key;
  for(int i = 0; i < 60; i++) {
    rgb_matrix_get_keypos(i, &key);
    uint16_t keycode = keymap_key_to_keycode(layer, key);
    color_key(i, keycode);
  }
}

void rgb_matrix_indicators_user(void) {
  // caps lock → light grave key
  if(host_keyboard_led_state().caps_lock) {
    rgb_matrix_set_color(0, 0xFF, 0xFF, 0xFF);
  }

  // compose → light right alt
  if(host_keyboard_led_state().compose) {
    rgb_matrix_set_color(57, 0xFF, 0xFF, 0xFF);
  }

  // gaming layer
  if(layer_state_cmp(layer_state, _game)) {
    // wasd keys in green
    rgb_matrix_set_color(16, 0x00, rgb_matrix_config.hsv.v, 0x00);
    rgb_matrix_set_color(29, 0x00, rgb_matrix_config.hsv.v, 0x00);
    rgb_matrix_set_color(30, 0x00, rgb_matrix_config.hsv.v, 0x00);
    rgb_matrix_set_color(31, 0x00, rgb_matrix_config.hsv.v, 0x00);

    // exit key in green
    rgb_matrix_set_color(58, 0x00, rgb_matrix_config.hsv.v, 0x00);
  } else {
    colorize_keys_by_keycode();
  }
  if(layer_state_cmp(layer_state, _lighting)) {
    //show brightness on number line
    uint8_t threshold = blend8(0, 10, rgb_matrix_config.hsv.v);
    for(int i = 0; i < 10; i++) {
      uint8_t v = i <= threshold ? 0xFF : 0x00;
      rgb_matrix_set_color(i+1, v, v, v);
    }
  }
}
