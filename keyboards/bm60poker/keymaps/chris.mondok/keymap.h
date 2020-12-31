#pragma once

#include QMK_KEYBOARD_H

void rgb_matrix_get_keypos(int index, keypos_t *keypos);
void color_key(int led_index, uint16_t keycode);
void colorize_keys_by_keycode(void);
void show_brightness_on_number_line(void);
