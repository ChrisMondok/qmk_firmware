bool g_shift_lock = false;

static bool lshift = false;
static bool rshift = false;

bool process_record_shift_lock(uint16_t keycode, keyrecord_t *record) {
  switch(keycode) {
    case KC_SPC:
      if(g_shift_lock) {
        if(record->event.pressed) register_code(KC_MINS);
        else unregister_code(KC_MINS);
        return false;
      }
      return true;
    case KC_LSFT:
    case KC_RSFT:
      if(record->event.pressed) {
        // exit shift lock when pressing either shift
        if(g_shift_lock) {
          unregister_code(KC_RSFT);
          g_shift_lock = false;
          // don't process pressing KC_RSFT, we discarded its previous release
          return keycode == KC_LSFT;
        } else {
          lshift |= keycode == KC_LSFT;
          rshift |= keycode == KC_RSFT;
          g_shift_lock = lshift & rshift;
          return true;
        }
      } else {
        lshift &= keycode != KC_LSFT;
        rshift &= keycode != KC_RSFT;

        // keep right shift held when shift lock is on
        return !g_shift_lock || keycode != KC_RSFT;
      }
      return true;
  }
  return true;
}
