/**
 * Looks up keypos from led index.
 * Only works for bm60poker.
 */
void rgb_matrix_get_keypos(int index, keypos_t *keypos) {
  if(index < 14) {
    keypos->row = 0;
    keypos->col = index;
  } else if(index < 28) {
    keypos->row = 1;
    keypos->col = index - 14;
  } else if(index < 41) {
    // NO_LED in column 1
    keypos->row = 2;
    keypos->col = index == 28 ? 0 : index - 29 + 2;
  } else if(index < 53) {
    keypos->row = 3;
    // NO_LED in column 0 and 12
    keypos->col = index == 52 ? 13 : index - 41 + 1;
  } else if(index < 61) {
    keypos->row = 4;
    keypos->col = index - 53;
    // three NO_LED after 55
    if(index > 55) keypos->col += 3;
    // two NO_LED after 56
    if(index > 56) keypos->col += 2;
    // NO_LED after 58
    if(index > 58) keypos->col += 1;
  }
}
