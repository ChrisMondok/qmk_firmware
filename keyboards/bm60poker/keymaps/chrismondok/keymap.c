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
#include <string.h>
#include "rgb_matrix.h"
#include "lib/lib8tion/lib8tion.h"
#include "keymap.h"
#include "raw_hid.h"
#include "led_hid.c"
#include "shift_lock.c"

#define RGB_DISABLE_WHEN_USB_SUSPENDED true
#define KC_COMPOSE KC_RALT

enum layers {
  LAYER_QWERTY,
  LAYER_FN,
  LAYER_GAME,
  LAYER_LIGHTING,
  LAYER_MAX,
};

enum custom_codes {
  KC_CUSTOM_RGB_PLAIN = SAFE_RANGE
};

enum hid_commands {
  HID_CMD_RESET_LED_GROUPS = 0x00,
  HID_CMD_SET_LED_GROUP,
  HID_CMD_SET_LAYER,
};

const uint16_t PROGMEM keymaps[LAYER_MAX][MATRIX_ROWS][MATRIX_COLS] = {
    [LAYER_QWERTY] = LAYOUT_60_ansi(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
        KC_TAB,      KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        LT(1,KC_ESC),   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
        KC_LSFT,            KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
        KC_LCTL,   KC_LGUI,   KC_LALT,                       KC_SPC,                              KC_RGUI,  KC_COMPOSE, KC_APP,  KC_RCTL
    ),
    [LAYER_FN] = LAYOUT_60_ansi(
        XXXXXXX,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,
        XXXXXXX,      KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_F17, XXXXXXX, KC_PGUP, KC_UP, KC_PGDN, KC_PSCR, XXXXXXX, XXXXXXX, KC_INS,
        KC_TRNS,       KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, TG(LAYER_GAME), KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, XXXXXXX, TG(LAYER_LIGHTING),
        KC_TRNS,            KC_SLEP, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  KC_END, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_TRNS,
        KC_TRNS,   KC_TRNS,   KC_TRNS,                      XXXXXXX,                   XXXXXXX,   KC_TRNS,   KC_TRNS,   KC_TRNS
    ),
    [LAYER_GAME] = LAYOUT_60_ansi(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
        KC_TAB,      KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        KC_ESC,   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
        KC_LSFT,            KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_UP,
        KC_LCTL,   KC_LGUI,   KC_LALT,                       KC_SPC,                              TG(LAYER_GAME), KC_LEFT,  KC_DOWN, KC_RIGHT
    ),
    [LAYER_LIGHTING] = LAYOUT_60_ansi(
        RGB_TOG,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        TG(LAYER_LIGHTING),       XXXXXXX, RGB_SAI, XXXXXXX, XXXXXXX, XXXXXXX, RGB_HUI, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_LSFT,            XXXXXXX, XXXXXXX, XXXXXXX, RGB_VAI, XXXXXXX, XXXXXXX, XXXXXXX, RGB_RMOD, RGB_MOD, XXXXXXX, KC_RSFT,
        XXXXXXX,   XXXXXXX,   XXXXXXX,                      KC_CUSTOM_RGB_PLAIN,                              XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if(!process_record_shift_lock(keycode, record)) return false;
  switch(keycode) {
    case KC_CUSTOM_RGB_PLAIN:
      rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
      rgb_matrix_sethsv(0, 0, 0);
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

static layer_state_t layer_state_old;

const uint64_t wasd = (1LL << 16) | (7LL << 29);

static const LedGroup gaming_led_group = {
  0x00,
  0xFF,
  0x00,
  wasd,
};

layer_state_t layer_state_set_user(layer_state_t state) {
  if(!layer_state_cmp(state, LAYER_GAME) && layer_state_cmp(layer_state_old, LAYER_GAME)) {
    // leaving game layer
    reset_led_groups();
  } else if(layer_state_cmp(state, LAYER_GAME) && !layer_state_cmp(layer_state_old, LAYER_GAME)) {
    //entering game layer
    set_led_group(1, &gaming_led_group);
  }
  layer_state_old = state;
  return state;
}

void raw_hid_receive(uint8_t *data, uint8_t length) {
  switch(*data) {
    case HID_CMD_RESET_LED_GROUPS:
      reset_led_groups();
      break;
    case HID_CMD_SET_LED_GROUP:
      //format: [cmdNumber, groupNumber, …group]
      set_led_group(data[1], (LedGroup *) (data+2));
      break;
    case HID_CMD_SET_LAYER:
      if(data[1] < LAYER_MAX) {
        layer_move(data[1]);
        break;
      }
    default:
      raw_hid_send(data, length);
      break;
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

void rgb_matrix_indicators_user(void) {
  if(!layer_state_cmp(layer_state, LAYER_GAME)) {
    colorize_keys_by_keycode();
  }

  rgb_matrix_indicate_groups();

  if(layer_state_cmp(layer_state, LAYER_LIGHTING)) {
    show_brightness_on_number_line();
  }
}

void color_key(int led_index, uint16_t keycode) {
  switch (keycode) {
    case XXXXXXX:
      // black
      // don't turn off keys on the lighting layer.
      if(!layer_state_cmp(layer_state, LAYER_LIGHTING)) {
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
    case KC_LSFT:
    case KC_RSFT:
    case KC_SPC:
    case KC_CAPS:
      if(host_keyboard_led_state().caps_lock) rgb_matrix_indicate_white(led_index);
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
    case TG(LAYER_GAME):
      // green
      rgb_matrix_indicate_hue(led_index, 0x55);
      return;
    case KC_COMPOSE:
      if(host_keyboard_led_state().compose) {
        rgb_matrix_indicate_white(led_index);
      }
      return;
    default:
      if(keycode >= QK_TOGGLE_LAYER && keycode < QK_TOGGLE_LAYER_MAX) {
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
