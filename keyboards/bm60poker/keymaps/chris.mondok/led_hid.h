// a SET_LEDS packet looks like this:
// 1 N R G B COUNT […leds]
// │ │ │ │ │ │     │
// │ │ │ │ │ │     └ LEDs in this group
// │ │ │ │ │ └ num LEDs
// │ │ │ │ └ blue
// │ │ │ └ green
// │ │ └ red
// │ └ group number (0 - 2)
// └ command number
//
// this leaves a max of 26 LEDs in a 32 byte message.
#define MAX_LEDS_IN_GROUP 26

typedef struct PACKED {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t count; // 1 byte
  uint8_t leds[MAX_LEDS_IN_GROUP];
} LedGroup;

void reset_led_groups(void);
void set_led_group(uint8_t group_number, LedGroup *group);
void rgb_matrix_indicate_groups(void);
