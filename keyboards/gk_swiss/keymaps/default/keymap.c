// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

// Hyper key setup 
#define GK_LCAG LCAG(KC_NO)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
       [0] = LAYOUT(
        KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINUS, KC_EQUAL, KC_BACKSPACE,
        KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LEFT_BRACKET, KC_RIGHT_BRACKET,
        KC_RIGHT_CTRL, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SEMICOLON, KC_QUOTE, KC_ENTER, 
        KC_LEFT_SHIFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_RIGHT_GUI, KC_N, KC_M, KC_COMMA, KC_DOT, KC_SLASH, KC_RIGHT_SHIFT, 
        KC_GRAVE, KC_LEFT_CTRL, KC_LEFT_ALT, KC_LEFT_GUI, KC_SPACE, KC_RIGHT_GUI, KC_RIGHT_CTRL, KC_SPACE, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, KC_BACKSLASH
    )
};

// Shift + esc = ~
const key_override_t o_h = ko_make_basic(MOD_BIT(KC_RIGHT_CTRL), KC_H, KC_LEFT);
const key_override_t o_j = ko_make_basic(MOD_BIT(KC_RIGHT_CTRL), KC_J, KC_DOWN);
const key_override_t o_k = ko_make_basic(MOD_BIT(KC_RIGHT_CTRL), KC_K, KC_UP);
const key_override_t o_l = ko_make_basic(MOD_BIT(KC_RIGHT_CTRL), KC_L, KC_RIGHT);
const key_override_t o_d = ko_make_basic(MOD_BIT(KC_RIGHT_CTRL), KC_D, KC_BACKSPACE);
const key_override_t o_sp = ko_make_basic(MOD_BIT(KC_RIGHT_CTRL), KC_SPACE, KC_ENTER);
const key_override_t o_esc = ko_make_basic(MOD_BIT(KC_RIGHT_CTRL), KC_E, KC_ESC);

const key_override_t o_m = ko_make_basic(MOD_BIT(KC_RIGHT_CTRL), KC_M, KC_MINUS);
const key_override_t o_u = ko_make_basic(MOD_BIT(KC_RIGHT_CTRL), KC_U, KC_EQUAL);
const key_override_t o_i = ko_make_basic(MOD_BIT(KC_RIGHT_CTRL), KC_I, KC_LEFT_PAREN);
const key_override_t o_o = ko_make_basic(MOD_BIT(KC_RIGHT_CTRL), KC_O, KC_RIGHT_PAREN);


const key_override_t *key_overrides[] = {
	&o_h, &o_j, &o_k, &o_l, &o_d, &o_sp,
    &o_esc,
    &o_m, &o_u, &o_i, &o_o
};


// Process custom keycodes
bool __process_record_user(uint16_t keycode, keyrecord_t *record) {
    static bool is_hyper_pressed = false;

    static uint16_t bootloader_key_timer = 0;
    static bool bootloader_other_key_recorded = false;  // track if any key other than CapsLock has been pressed
    if (keycode != KC_BACKSLASH) { bootloader_other_key_recorded = true; }

    switch (keycode) {
        case KC_BACKSLASH:  // enter bootloader if CapsLock is held for 5 seconds
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

        case GK_LCAG:
            if (record->event.pressed) {
                is_hyper_pressed = true;
            } else {
                is_hyper_pressed = false;
            }
            break;

        case KC_D:
            if (record->event.pressed && is_hyper_pressed) { tap_code(KC_BSPC); return false; } break;
        case KC_H:
            if (record->event.pressed && is_hyper_pressed) {tap_code(KC_LEFT); return false; } break;
        case KC_J:
            if (record->event.pressed && is_hyper_pressed) { tap_code(KC_DOWN); return false; } break;
        case KC_K:
            if (record->event.pressed && is_hyper_pressed) { tap_code(KC_UP); return false; } break;
        case KC_L:
            if (record->event.pressed && is_hyper_pressed) { tap_code(KC_RIGHT); return false; } break;
        case KC_E:
            if (record->event.pressed && is_hyper_pressed) { tap_code(KC_ESC); return false; } break;
        case KC_SPACE:
            if (record->event.pressed && is_hyper_pressed) { tap_code(KC_ENTER); return false; } break; 
    }

    return true; // Process all other keycodes normally
}
