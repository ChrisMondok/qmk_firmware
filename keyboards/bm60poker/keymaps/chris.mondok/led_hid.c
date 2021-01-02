#include "led_hid.h"

#define NUM_LED_GROUPS 3

LedGroup ledGroups[NUM_LED_GROUPS];

void reset_led_groups() {
  memset(&(ledGroups[0]), 0, sizeof(LedGroup));
  memset(&(ledGroups[1]), 0, sizeof(LedGroup));
  memset(&(ledGroups[2]), 0, sizeof(LedGroup));
}

void raw_hid_receive_led_groups(uint8_t *data, uint8_t length) {
  if(*data == _reset) {
    memset(&(ledGroups[0]), 0, sizeof(LedGroup));
    memset(&(ledGroups[1]), 0, sizeof(LedGroup));
    memset(&(ledGroups[2]), 0, sizeof(LedGroup));
  } else if(*data == _set_led_group) {
    //format: [cmdNumber, groupNumber, …group]
    memcpy(ledGroups + data[1], data+2, 30);
  }

  raw_hid_send(data, length);
}

void rgb_matrix_indicate_groups() {
  for(uint8_t group_index = 0; group_index < NUM_LED_GROUPS; group_index++) {
    LedGroup group = ledGroups[group_index];
    for(uint8_t led_index = 0; led_index < group.count; led_index++) {
      rgb_matrix_set_color(group.leds[led_index], group.red, group.green, group.blue);
    }
  }
}
