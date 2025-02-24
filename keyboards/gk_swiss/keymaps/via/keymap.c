// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
       [0] = LAYOUT(
        KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINUS, KC_EQUAL, KC_BACKSPACE,
        KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LEFT_BRACKET, KC_RIGHT_BRACKET,
        KC_CAPS_LOCK, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SEMICOLON, KC_QUOTE, KC_ENTER, 
        KC_LEFT_SHIFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_RIGHT_ALT, KC_N, KC_M, KC_COMMA, KC_DOT, KC_SLASH, KC_RIGHT_SHIFT, 
        KC_GRAVE,
        KC_LEFT_CTRL, KC_LEFT_ALT, KC_LEFT_GUI, KC_SPACE, KC_RIGHT_GUI,
        KC_RIGHT_CTRL, KC_SPACE, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, KC_BACKSLASH
    )
};

// Process custom keycodes
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    
    static uint16_t bootloader_key_timer = 0;
    static bool bootloader_other_key_recorded = false; 
    if (keycode != KC_BACKSLASH) { bootloader_other_key_recorded = true; }

    switch (keycode) {
        case KC_BACKSLASH:  // enter bootloader if is held for 5 seconds
            if (record->event.pressed) {
                bootloader_key_timer = timer_read();
                bootloader_other_key_recorded = false;  // start tracking other keys
            } else {
                if (!bootloader_other_key_recorded) {  // only go to bootloader if no other key has been pressed
                    if (timer_elapsed(bootloader_key_timer) >= 6000) {
                        bootloader_jump();
                    }
                }
                bootloader_key_timer = 0;  // reset timer counter on release so it can be used for tracking if CapsLock is pressed
            }
            break;
    }

    return true; // Process all other keycodes normally
}
