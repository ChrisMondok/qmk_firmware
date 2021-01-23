static bool lshift = false;
static bool rshift = false;

static bool is_caps_lock_on(void) {
  return host_keyboard_led_state().caps_lock;
}

static void toggle_caps(void) {
  register_code(KC_CAPS);
  unregister_code(KC_CAPS);
}

bool process_record_shift_lock(uint16_t keycode, keyrecord_t *record) {
  switch(keycode) {
    case KC_SPC:
      if(is_caps_lock_on()) {
        if(record->event.pressed) {
          register_code(KC_LSFT);
          register_code(KC_MINS);
        }
        else {
          unregister_code(KC_MINS);
          unregister_code(KC_LSFT);
        }
        return false;
      }
      return true;
    case KC_LSFT:
    case KC_RSFT:
      if(record->event.pressed) {
        if(is_caps_lock_on()) toggle_caps();
        lshift |= keycode == KC_LSFT;
        rshift |= keycode == KC_RSFT;
        if(lshift && rshift) toggle_caps();
      } else {
        lshift &= keycode != KC_LSFT;
        rshift &= keycode != KC_RSFT;
      }
      return true;
  }
  return true;
}
