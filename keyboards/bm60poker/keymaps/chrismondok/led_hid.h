#define NUM_LED_GROUPS 3

typedef struct PACKED {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint64_t led_bitmask;
} LedGroup;

typedef struct PACKED {
  RGB rgb;
  uint16_t alert_time;
} Alert;

void reset_led_groups(void);
void set_led_group(uint8_t group_number, const LedGroup *group);
void rgb_matrix_indicate_hid(void);
void do_alert(uint8_t red, uint8_t green, uint8_t blue);
