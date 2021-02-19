#include "led_hid.h"

static LedGroup led_groups[NUM_LED_GROUPS];

void reset_led_groups() {
  for(int i = 0; i < NUM_LED_GROUPS; i++) {
    memset(&(led_groups[i]), 0, sizeof(LedGroup));
  }
}

void set_led_group(uint8_t group_number, const LedGroup *group) {
  memcpy(led_groups + group_number, group, sizeof(LedGroup));
}

static Alert alert;

static const int ALERT_DURATION = 500;

void do_alert(uint8_t red, uint8_t green, uint8_t blue) {
  alert.rgb.r = red;
  alert.rgb.g = green;
  alert.rgb.b = blue;
  alert.alert_time = timer_read();
}

void rgb_matrix_indicate_hid() {
  if(alert.alert_time) {
    uint16_t elapsed = timer_elapsed(alert.alert_time);
    if(elapsed > ALERT_DURATION) {
      alert.alert_time = 0;
    } else {
      rgb_matrix_set_color_all(alert.rgb.r, alert.rgb.g, alert.rgb.b);
    }
  } else {
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

}
