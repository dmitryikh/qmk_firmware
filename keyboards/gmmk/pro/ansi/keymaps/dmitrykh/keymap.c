/* Copyright 2021 Glorious, LLC <salman@pcgamingrace.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "rgb_matrix_map.h"
#include "keymap_from_json.inc"

#define ARRAYSIZE(arr) sizeof(arr) / sizeof(arr[0])

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (clockwise) {
      tap_code(KC_VOLU);
    } else {
      tap_code(KC_VOLD);
    }
    return false;
}
#endif // ENCODER_ENABLE

LEADER_EXTERNS();

static bool quote_swap = false;

#define USER_SEQ_ONE_KEY(key) if (sequence[0] == (key))
#define USER_SEQ_TWO_KEYS(key1, key2) if (sequence[0] == (key1) && sequence[1] == (key2))
#define USER_SEQ_THREE_KEYS(key1, key2, key3) if (sequence[0] == (key1) && sequence[1] == (key2) && sequence[2] == (key3))
#define USER_SEQ_FOUR_KEYS(key1, key2, key3, key4) if (sequence[0] == (key1) && sequence[1] == (key2) && sequence[2] == (key3) && sequence[3] == (key4))
#define USER_SEQ_FIVE_KEYS(key1, key2, key3, key4, key5) if (sequence[0] == (key1) && sequence[1] == (key2) && sequence[2] == (key3) && sequence[3] == (key4) && sequence[4] == (key5))

int8_t leader_user_process(uint8_t sequence_size, uint16_t sequence[5]) {
  if (sequence_size == 1) {
    USER_SEQ_ONE_KEY(KC_1) {
      keyrecord_t kr;
      kr.event.pressed = false;
      process_dynamic_macro(DYN_MACRO_PLAY1, &kr);
      return 1;
    }
    SEQ_ONE_KEY(KC_2) {
      keyrecord_t kr;
      kr.event.pressed = false;
      process_dynamic_macro(DYN_MACRO_PLAY2, &kr);
      return 1;
    }
    SEQ_ONE_KEY(KC_QUOT) {
      quote_swap = !quote_swap;
      return 1;
    }

    // Check all possible continuations. Return error if no continuation is possible.
    if (sequence[0] == KC_Q) {
      return 0;
    }
    // TODO: error signal
    return -1;
  } else if (sequence_size == 2) {
    USER_SEQ_TWO_KEYS(KC_Q, KC_1) {
      keyrecord_t kr;
      kr.event.pressed = false;
      process_dynamic_macro( DYN_REC_START1, &kr );
      return 1;
    }
    USER_SEQ_TWO_KEYS(KC_Q, KC_Q) {
      keyrecord_t kr;
      kr.event.pressed = true;
      process_dynamic_macro(DYN_REC_STOP, &kr);
      return 1;
    }
    USER_SEQ_TWO_KEYS(KC_Q, KC_2) {
      keyrecord_t kr;
      kr.event.pressed = false;
      process_dynamic_macro(DYN_REC_START2, &kr);
      return 1;
    }
    // TODO: error signal
    return -1;
  }
  return -1;
}

 #define MODS_SHIFT_MASK  (MOD_BIT(KC_LSHIFT)|MOD_BIT(KC_RSHIFT))

 bool process_record_user(uint16_t keycode, keyrecord_t *record) {
   if (quote_swap && keycode == KC_QUOTE) {
     if (record->event.pressed) {
       uint8_t prev_mods = get_mods();
       bool had_shift = prev_mods & MODS_SHIFT_MASK;
       clear_mods();
       if (had_shift) {
         register_code(KC_QUOTE);
       } else {
         register_mods(MOD_BIT(KC_LSHIFT));
         register_code(KC_QUOTE);
         unregister_mods(MOD_BIT(KC_LSHIFT));
       }
       set_mods(prev_mods);
     } else {
       unregister_code(KC_QUOTE);
     }
     return false;
   }
  // switch (keycode) {
    // case KC_QUOTE:
    //       if (quote_swap) {
    //           if (record->event.pressed) {
    //              if (keyboard_report->mods & MODS_SHIFT_MASK) {
    //                  register_code(KC_QUOTE);
    //              } else {
    //                  register_code(KC_LSFT);
    //                  register_code(KC_QUOTE);
    //              }
    //          } else { // Release the key
    //              if (keyboard_report->mods & MODS_SHIFT_MASK) {
    //                  unregister_code(KC_QUOTE);
    //              } else {
    //                  unregister_code(KC_LSFT);
    //                  unregister_code(KC_QUOTE);
    //              }
    //          }
    //          return false;
    //       }
      // if (quote_swap && record->event.pressed) {
      //   if (keyboard_report->mods & MOD_BIT (KC_LSFT)) {
      //       unregister_mods(MOD_LSFT);
      //       register_code(KC_QUOTE);
      //       // unregister_code(KC_QUOTE);
      //       register_mods(MOD_LSFT);
      //       return false;
      //   }
      //   if (keyboard_report->mods & MOD_BIT (KC_RSFT)) {
      //       unregister_mods(MOD_RSFT);
      //       register_code(KC_QUOTE);
      //       // unregister_code(KC_QUOTE);
      //       register_mods(MOD_RSFT);
      //       return false;
      //   }
      //   register_mods(MOD_LSFT);
      //   register_code(KC_QUOTE);
      //   // unregister_code(KC_QUOTE);
      //   unregister_mods(MOD_LSFT);
      //   return false;
      // }
      // return true;

      //   if ((keyboard_report->mods & MOD_BIT (KC_LSFT)) || (keyboard_report->mods & MOD_BIT (KC_RSFT))) {
      //     // Shift pressed
      //             register_mods(MOD_LSFT);
      //         } else {
      //             unregister_mods(MOD_LSFT);
      //     if (record->event.pressed)
      //       register_code(KC_QUOTE);
      //     else
      //       unregister_code(KC_QUOTE);
      //   } else {
      //     // No shift
      //     register_code(KC_LSFT);
      //     register_code(KC_QUOTE);
      //     unregister_code(KC_QUOTE);
      //     unregister_code(KC_LSFT);
      //   }
      //   return false;
      // }
      // return true;
    // default:
    //   return true; // Process all other keycodes normally
  // }
  return true;
}

 #ifdef RGB_MATRIX_ENABLE
// Capslock, Scroll lock and Numlock indicator on Left side lights.
void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // if (get_rgb_nightmode()) rgb_matrix_set_color_all(RGB_OFF);

    // Fn selector mode RGB setup
    switch (get_highest_layer(layer_state)) { // special handling per layer
    case 1: // on Fn layer select what the encoder does when pressed
        rgb_matrix_set_color(LED_FN, RGB_RED); //FN key

        //NEW RGB LIGHTING TO RING KEYBOARD ON FN LAYER ACTIVATION:
        for (uint8_t j = 0; j < ARRAYSIZE(LED_LIST_FUNCROW); j++) {
            rgb_matrix_set_color(LED_LIST_FUNCROW[j], RGB_RED);
        }
        // // Add RGB Timeout Indicator -- shows 0 to 139 using F row and num row; larger numbers using 16bit code
        // uint16_t timeout_threshold = get_timeout_threshold();
        // if (timeout_threshold <= 10) rgb_matrix_set_color(LED_LIST_FUNCROW[timeout_threshold], RGB_CYAN);
        // else if (timeout_threshold < 140) {
        //     rgb_matrix_set_color(LED_LIST_FUNCROW[(timeout_threshold / 10)], RGB_CYAN);
        //     rgb_matrix_set_color(LED_LIST_NUMROW[(timeout_threshold % 10)], RGB_CYAN);
        // } else { // >= 140 minutes, just show these 3 lights
        //     rgb_matrix_set_color(LED_LIST_NUMROW[10], RGB_CYAN);
        //     rgb_matrix_set_color(LED_LIST_NUMROW[11], RGB_CYAN);
        //     rgb_matrix_set_color(LED_LIST_NUMROW[12], RGB_CYAN);
        // }
        break;

        // Numpad & Mouse Keys overlay RGB
    case 2:
        for (uint8_t j = 0; j < ARRAYSIZE(LED_LIST_FUNCROW); j++) {
            rgb_matrix_set_color(LED_LIST_FUNCROW[j], RGB_GREEN);
        }
        break;

    default:
        break;
    }
    if (quote_swap) {
      rgb_matrix_set_color(LED_QUOT, RGB_TEST5);
    }
}
#endif

void keyboard_post_init_keymap(void) {
  // keyboard_post_init_user() moved to userspace
  #ifdef RGB_MATRIX_ENABLE
  rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
  rgb_matrix_sethsv_noeeprom(20, 255, 127); // Default startup color (Hue:amber Saturation:full Value(brightness):mid)
  // activate_rgb_nightmode(false); // Set to true if you want to startup in nightmode, otherwise use Fn + Z to toggle
  #endif
}