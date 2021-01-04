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
#include "keymap.h"

#define RGB_DISABLE_WHEN_USB_SUSPENDED true
#define KC_COMPOSE KC_RALT

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
        KC_LCTL,   KC_LGUI,   KC_LALT,                       KC_SPC,                              KC_RGUI,  KC_COMPOSE, KC_APP,  KC_RCTL
    ),
    [_fn] = LAYOUT_60_ansi(
        KC_CAPS,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,
        XXXXXXX,      KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_F17, XXXXXXX, KC_PGUP, KC_UP, KC_PGDN, KC_PSCR, XXXXXXX, XXXXXXX, KC_INS,
        KC_TRNS,       KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, TG(_game), KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_TRNS,            KC_SLEP, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  KC_END, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,                      XXXXXXX,                   XXXXXXX,   TG(_lighting),   KC_TRNS,   KC_TRNS
    ),
    [_game] = LAYOUT_60_ansi(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
        KC_TAB,      KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        KC_ESC,   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
        KC_LSFT,            KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_UP,
        KC_LCTL,   KC_LGUI,   KC_LALT,                       KC_SPC,                              TG(_game), KC_LEFT,  KC_DOWN, KC_RIGHT
    ),
    [_lighting] = LAYOUT_60_ansi(
        RGB_TOG,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        TG(_lighting),       XXXXXXX, RGB_SAI, XXXXXXX, XXXXXXX, XXXXXXX, RGB_HUI, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_LSFT,            XXXXXXX, XXXXXXX, XXXXXXX, RGB_VAI, XXXXXXX, XXXXXXX, XXXXXXX, RGB_RMOD, RGB_MOD, XXXXXXX, KC_RSFT,
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

void keyboard_post_init_user(void) {
  rgb_matrix_enable_noeeprom();

  // disable underglow, the case is opaque.
  rgb_matrix_set_flags(LED_FLAG_ALL ^ LED_FLAG_UNDERGLOW);
}

void suspend_power_down_user(void) {
  rgb_matrix_set_color_all(0x00, 0x00, 0x00);
  RGB rgb = hsv_to_rgb(rgb_matrix_config.hsv);
  rgb_matrix_set_color(56, rgb.r, rgb.g, rgb.b);
}

void rgb_matrix_indicators_user(void) {
  if(layer_state_cmp(layer_state, _game)) {
    // wasd keys in green
    rgb_matrix_set_color(16, 0x00, rgb_matrix_config.hsv.v, 0x00);
    rgb_matrix_set_color(29, 0x00, rgb_matrix_config.hsv.v, 0x00);
    rgb_matrix_set_color(30, 0x00, rgb_matrix_config.hsv.v, 0x00);
    rgb_matrix_set_color(31, 0x00, rgb_matrix_config.hsv.v, 0x00);

    // arrow keys in green
    rgb_matrix_set_color(52, 0x00, rgb_matrix_config.hsv.v, 0x00);
    rgb_matrix_set_color(58, 0x00, rgb_matrix_config.hsv.v, 0x00);
    rgb_matrix_set_color(59, 0x00, rgb_matrix_config.hsv.v, 0x00);
    rgb_matrix_set_color(60, 0x00, rgb_matrix_config.hsv.v, 0x00);

    // exit key in cyan
    rgb_matrix_set_color(57, 0x00, rgb_matrix_config.hsv.v, rgb_matrix_config.hsv.v);
  } else {
    colorize_keys_by_keycode();
  }

  if(layer_state_cmp(layer_state, _lighting)) {
    show_brightness_on_number_line();
  }
}

void rgb_matrix_indicate_white(int led_index) {
  uint8_t v = rgb_matrix_config.hsv.v;
  if(v < 0x80) v = 0x80;
  rgb_matrix_set_color(led_index, v, v, v);
}

void rgb_matrix_indicate_hue(int led_index, uint8_t hue) {
  HSV hsv = rgb_matrix_config.hsv;
  hsv.h = hue;
  hsv.s = 0xFF;
  if(hsv.v < 0x80) hsv.v = 0x80;
  RGB rgb = hsv_to_rgb(hsv);
  rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);
}

void color_key(int led_index, uint16_t keycode) {
  switch (keycode) {
    case XXXXXXX:
      // black
      // don't turn off keys on the lighting layer.
      if(!layer_state_cmp(layer_state, _lighting)) {
        rgb_matrix_set_color(led_index, 0x00, 0x00, 0x00);
      }
      return;
    case KC_UP:
    case KC_LEFT:
    case KC_DOWN:
    case KC_RIGHT:
    case RGB_MOD:
    case RGB_RMOD:
    case RGB_HUI:
    case RGB_HUD:
    case RGB_SAI:
    case RGB_SAD:
    case RGB_VAI:
    case RGB_VAD:
      // white
      rgb_matrix_indicate_white(led_index);
      return;
    case KC_PGUP:
    case KC_PGDN:
    case KC_HOME:
    case KC_END:
      // blue
      rgb_matrix_indicate_hue(led_index, 0xAA);
      return;
    case KC_PSCR:
    case KC_SLEP:
      // yellow
      rgb_matrix_indicate_hue(led_index, 0x30);
      return;
    case KC_MPLY:
    case KC_MPRV:
    case KC_MNXT:
      // red
      rgb_matrix_indicate_hue(led_index, 0x00);
      return;
    case TG(_game):
      // green
      rgb_matrix_indicate_hue(led_index, 0x55);
      return;
    case KC_COMPOSE:
      if(host_keyboard_led_state().compose) {
        rgb_matrix_indicate_white(led_index);
      }
      return;
    case KC_CAPS:
    case KC_GRV:
      if(host_keyboard_led_state().caps_lock) {
        rgb_matrix_indicate_white(led_index);
      }
      return;
    default:
      // f keys are non-contiguous!
      if((keycode >= KC_F1 && keycode <= KC_F12) || (keycode >= KC_F13 && keycode <= KC_F24)) {
        rgb_matrix_indicate_hue(led_index, 0xD5);
      } else if(keycode >= QK_TOGGLE_LAYER && keycode < QK_TOGGLE_LAYER_MAX) {
        rgb_matrix_indicate_hue(led_index, 0x80);
      }
      return;
  }
}

void colorize_keys_by_keycode(void) {
  uint8_t layer = get_highest_layer(layer_state);
  keypos_t key;
  uint8_t led_i;
  for(key.row = 0; key.row < MATRIX_ROWS; key.row++) {
    for(key.col = 0; key.col < MATRIX_COLS; key.col++) {
      uint16_t keycode = keymap_key_to_keycode(layer, key);
      if(keycode == KC_NO) continue;
      // BUG: this will misbehave if there is ever more than one LED for the given row / column.
      // This *appears* safe for the bm60poker.
      int count = rgb_matrix_map_row_column_to_led(key.row, key.col, &led_i);
      if(!count) continue;
      color_key(led_i, keycode);
    }
  }
}

void show_brightness_on_number_line() {
  uint8_t threshold;
  if(rgb_matrix_config.hsv.v > 0) {
    threshold = blend8(0, 10, rgb_matrix_config.hsv.v);
  } else {
    // out of range so that no keys light up
    threshold = 255;
  }
  for(int i = 0; i < 10; i++) {
    uint8_t v = i == threshold ? 0xFF : 0x00;
    rgb_matrix_set_color(i+1, v, v, v);
  }
}
