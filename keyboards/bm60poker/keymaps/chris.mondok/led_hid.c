#include "led_hid.h"

#define NUM_LED_GROUPS 3

static LedGroup led_groups[NUM_LED_GROUPS];

void reset_led_groups() {
  memset(&(led_groups[0]), 0, sizeof(LedGroup));
  memset(&(led_groups[1]), 0, sizeof(LedGroup));
  memset(&(led_groups[2]), 0, sizeof(LedGroup));
}

void set_led_group(uint8_t group_number, LedGroup *group) {
  memcpy(led_groups + group_number, group, 30);
}

void rgb_matrix_indicate_groups() {
  for(uint8_t group_index = 0; group_index < NUM_LED_GROUPS; group_index++) {
    LedGroup group = led_groups[group_index];
    for(uint8_t led_index = 0; led_index < group.count; led_index++) {
      rgb_matrix_set_color(group.leds[led_index], group.red, group.green, group.blue);
    }
  }
}
