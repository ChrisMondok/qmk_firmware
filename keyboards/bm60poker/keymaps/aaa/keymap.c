/* Copyright 2020 ipetepete
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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_60_ansi(
        KC_ESC,  KC_A,    KC_A,    KC_A,    KC_A,    KC_A,    KC_A,    KC_A,    KC_A,    KC_A,    KC_A,    KC_A, KC_A,  KC_BSPC,
        KC_TAB,      KC_A,    KC_A,    KC_A,    KC_A,    KC_A,    KC_A,    KC_A,    KC_A,    KC_A,    KC_A,    KC_A, KC_A, KC_A,
        KC_CAPS,       KC_A,    KC_A,    KC_A,    KC_A,    KC_A,    KC_A,    KC_A,    KC_A,    KC_A,    KC_A, KC_A, KC_ENT,
        KC_LSFT,            KC_A,    KC_A,    KC_A,    KC_A,    KC_A,    KC_A,    KC_A,    KC_A, KC_A,  KC_A, KC_RSFT,
        KC_LCTL,   KC_LGUI,   KC_LALT,                       KC_SPC,                              KC_RALT,   KC_RGUI,   MO(1),     KC_RCTL
    ),
    [1] = LAYOUT_60_ansi(
        RESET,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,
        _______,     RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, _______, _______, _______, _______, _______,
        _______,       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______,            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______,   _______,   _______,                      _______,                              _______,   _______,   _______,   _______
    ),

};
