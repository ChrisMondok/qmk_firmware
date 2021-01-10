#include "led_hid.h"

#define NUM_LED_GROUPS 3

static LedGroup led_groups[NUM_LED_GROUPS];

void reset_led_groups() {
  memset(&(led_groups[0]), 0, sizeof(LedGroup));
  memset(&(led_groups[1]), 0, sizeof(LedGroup));
  memset(&(led_groups[2]), 0, sizeof(LedGroup));
}

void set_led_group(uint8_t group_number, const LedGroup *group) {
  memcpy(led_groups + group_number, group, sizeof(LedGroup));
}

void rgb_matrix_indicate_groups() {
  for(uint8_t led_index = 0; led_index < 61; led_index++) {
    uint64_t bitmask = 1LL << (led_index);
    for(uint8_t group_index = 0; group_index < NUM_LED_GROUPS; group_index++) {
      LedGroup group = led_groups[group_index];
      if(group.led_bitmask & bitmask) {
        rgb_matrix_set_color(led_index, group.red, group.green, group.blue);
      }
    }
  }
}
