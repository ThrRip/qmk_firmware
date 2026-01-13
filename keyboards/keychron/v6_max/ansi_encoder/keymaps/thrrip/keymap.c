/* Copyright 2024 ~ 2025 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "keychron_common.h"

// ThrRip start
extern MidiDevice midi_device;

#define MIDI_CC_NUM_BASE 102
#define MIDI_CC_NUM_OFFSET_MAX 15

#define MIDI_CC_NUM_SEL_INDIC
#define MIDI_CC_NUM_SEL_INDIC_INDEX_B3 16
#define MIDI_CC_NUM_SEL_INDIC_INDEX_B2 17
#define MIDI_CC_NUM_SEL_INDIC_INDEX_B1 18
#define MIDI_CC_NUM_SEL_INDIC_INDEX_B0 19

uint8_t midi_cc_num_offset     = 0;
uint8_t midi_cc_val_offset_exp = 0;

enum midi_keycodes {
    // QK_USER_0 = 0x7E40 = CUSTOM(64) in VIA
    MIDI_CCSMIN = QK_USER_0, // CC Send Min Value
                             //     (64 - 2 ^ midi_cc_val_offset_exp)
    MIDI_CCSMAX,             // CC Send Max Value
                             //     (64 + 2 ^ midi_cc_val_offset_exp,
                             //     but not greater than 127)
    MIDI_CCSMID,             // CC Send Midpoint Value (64)
    MIDI_CCNINC,             // CC Number Increment
    MIDI_CCNDEC,             // CC Number Decrement
    MIDI_CCNSB3,             // CC Number Selector Bit 3
    MIDI_CCNSB2,             // CC Number Selector Bit 2
    MIDI_CCNSB1,             // CC Number Selector Bit 1
    MIDI_CCNSB0,             // CC Number Selector Bit 0
    MIDI_CCNRST,             // CC Number Reset
    MIDI_CCVOEI,             // CC Value Offset Exponent Increment
    MIDI_CCVOED,             // CC Value Offset Exponent Decrement
    MIDI_CCVOER,             // CC Value Offset Exponent Reset
};
// ThrRip end

enum layers {
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    WIN_FN,
};

#define FN_MAC MO(MAC_FN)
#define FN_WIN MO(WIN_FN)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_ansi_109(
        KC_ESC,   KC_BRID,  KC_BRIU,  KC_MCTRL, KC_LNPAD, UG_VALD,  UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_MUTE,   KC_SNAP,  KC_SIRI,  UG_NEXT,   KC_F13,   KC_F14,   KC_F15,   KC_F16,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,   KC_INS,   KC_HOME,  KC_PGUP,   KC_NUM,   KC_PSLS,  KC_PAST,  KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,   KC_DEL,   KC_END,   KC_PGDN,   KC_P7,    KC_P8,    KC_P9,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,                                   KC_P4,    KC_P5,    KC_P6,    KC_PPLS,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,             KC_UP,               KC_P1,    KC_P2,    KC_P3,
        KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD, KC_ROPTN, FN_MAC,   KC_RCTL,   KC_LEFT,  KC_DOWN,  KC_RGHT,   KC_P0,              KC_PDOT,  KC_PENT),

    [MAC_FN] = LAYOUT_ansi_109(
        _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   UG_TOGG,   _______,  _______,  UG_TOGG,   _______,  _______,  _______,  _______,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,   _______,  _______,  _______,  _______,
        UG_TOGG,  UG_NEXT,  UG_VALU,  UG_HUEU,  UG_SATU,  UG_SPDU,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,   _______,  _______,  _______,
        _______,  UG_PREV,  UG_VALD,  UG_HUED,  UG_SATD,  UG_SPDD,  _______,  _______,  _______,  _______,  _______,  _______,            _______,                                  _______,  _______,  _______,  _______,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  _______,  _______,  _______,  _______,  _______,            _______,             _______,             _______,  _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,   _______,  _______,  _______,   _______,            _______,  _______),

    [WIN_BASE] = LAYOUT_ansi_109(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_MUTE,   KC_PSCR,  KC_CTANA, UG_NEXT,   KC_F13,   KC_F14,   KC_F15,   KC_F16,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,   KC_INS,   KC_HOME,  KC_PGUP,   KC_NUM,   KC_PSLS,  KC_PAST,  KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,   KC_DEL,   KC_END,   KC_PGDN,   KC_P7,    KC_P8,    KC_P9,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,                                   KC_P4,    KC_P5,    KC_P6,    KC_PPLS,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,             KC_UP,               KC_P1,    KC_P2,    KC_P3,
        KC_LCTL,  KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,  KC_RWIN,  FN_WIN,   KC_RCTL,   KC_LEFT,  KC_DOWN,  KC_RGHT,   KC_P0,              KC_PDOT,  KC_PENT),

    [WIN_FN] = LAYOUT_ansi_109(
        _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  UG_VALD,  UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  UG_TOGG,   _______,  _______,  UG_TOGG,   _______,  _______,  _______,  _______,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,   _______,  _______,  _______,  _______,
        UG_TOGG,  UG_NEXT,  UG_VALU,  UG_HUEU,  UG_SATU,  UG_SPDU,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,   _______,  _______,  _______,
        _______,  UG_PREV,  UG_VALD,  UG_HUED,  UG_SATD,  UG_SPDD,  _______,  _______,  _______,  _______,  _______,  _______,            _______,                                  _______,  _______,  _______,  _______,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  _______,  _______,  _______,  _______,  _______,            _______,             _______,             _______,  _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,   _______,  _______,  _______,   _______,            _______,  _______)
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [MAC_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [MAC_FN]   = {ENCODER_CCW_CW(UG_VALD, UG_VALU)},
    [WIN_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [WIN_FN]   = {ENCODER_CCW_CW(UG_VALD, UG_VALU)},
};
#endif // ENCODER_MAP_ENABLE

// ThrRip start
// clang-format on
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
            // clang-format off
        case MIDI_CCSMIN:
            if (record->event.pressed) {
                midi_send_cc(
                    &midi_device, midi_config.channel,
                    MIDI_CC_NUM_BASE + midi_cc_num_offset,
                    64 - (1 << midi_cc_val_offset_exp)
                );
            }
            return false;

        case MIDI_CCSMAX:
            if (record->event.pressed) {
                midi_send_cc(
                    &midi_device, midi_config.channel,
                    MIDI_CC_NUM_BASE + midi_cc_num_offset,
                    // 64 + 2 ^ 6 = 128, which is out of range
                    midi_cc_val_offset_exp != 6 ? 64 + (1 << midi_cc_val_offset_exp) : 127
                );
            }
            return false;

        case MIDI_CCSMID:
            if (record->event.pressed) {
                midi_send_cc(
                    &midi_device, midi_config.channel,
                    MIDI_CC_NUM_BASE + midi_cc_num_offset, 64
                );
            }
            return false;

        case MIDI_CCNINC:
            if (record->event.pressed) {
                midi_cc_num_offset =
                    (midi_cc_num_offset + 1) % (MIDI_CC_NUM_OFFSET_MAX + 1);
            }
            return false;

        case MIDI_CCNDEC:
            if (record->event.pressed) {
                midi_cc_num_offset =
                    (midi_cc_num_offset + MIDI_CC_NUM_OFFSET_MAX) % (MIDI_CC_NUM_OFFSET_MAX + 1);
            }
            return false;
            // clang-format on

        case MIDI_CCNSB3:
            if (record->event.pressed) {
                midi_cc_num_offset ^= 1 << 3;
            }
            return false;

        case MIDI_CCNSB2:
            if (record->event.pressed) {
                midi_cc_num_offset ^= 1 << 2;
            }
            return false;

        case MIDI_CCNSB1:
            if (record->event.pressed) {
                midi_cc_num_offset ^= 1 << 1;
            }
            return false;

        case MIDI_CCNSB0:
            if (record->event.pressed) {
                midi_cc_num_offset ^= 1;
            }
            return false;

        case MIDI_CCNRST:
            if (record->event.pressed) {
                midi_cc_num_offset = 0;
            }
            return false;

        case MIDI_CCVOEI:
            if (record->event.pressed) {
                midi_cc_val_offset_exp = (midi_cc_val_offset_exp + 1) % (6 + 1);
            }
            return false;

        case MIDI_CCVOED:
            if (record->event.pressed) {
                midi_cc_val_offset_exp = (midi_cc_val_offset_exp + 6) % (6 + 1);
            }
            return false;

        case MIDI_CCVOER:
            if (record->event.pressed) {
                midi_cc_val_offset_exp = 0;
            }
            return false;

        default:
            return true;
    }
}

#ifdef MIDI_CC_NUM_SEL_INDIC
bool rgb_matrix_indicators_user(void) {
    if (midi_cc_num_offset & (1 << 3)) {
        rgb_matrix_set_color(MIDI_CC_NUM_SEL_INDIC_INDEX_B3, RGB_WHITE);
    }
    if (midi_cc_num_offset & (1 << 2)) {
        rgb_matrix_set_color(MIDI_CC_NUM_SEL_INDIC_INDEX_B2, RGB_WHITE);
    }
    if (midi_cc_num_offset & (1 << 1)) {
        rgb_matrix_set_color(MIDI_CC_NUM_SEL_INDIC_INDEX_B1, RGB_WHITE);
    }
    if (midi_cc_num_offset & 1) {
        rgb_matrix_set_color(MIDI_CC_NUM_SEL_INDIC_INDEX_B0, RGB_WHITE);
    }
    return true;
}
#endif
// ThrRip end
