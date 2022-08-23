/*
Copyright 2021-2022 TweetyDaBird, Marius Renner

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

#pragma once

#include "config_common.h"

// USB Device descriptor parameter
#define VENDOR_ID    0x0079
#define PRODUCT_ID   0x0058
#define DEVICE_VER   0x0001
#define MANUFACTURER TweetyDaBird
#define PRODUCT      Lotus58 Glow

/* Bootmagic Lite key configuration */
/* Top left key on left half (where 'escape' or 'grave' would usually be)*/
#define BOOTMAGIC_LITE_ROW 0
#define BOOTMAGIC_LITE_COLUMN 0

/* Top right key on right half (where 'backspace' or 'dash' would usually be)*/
#define BOOTMAGIC_LITE_ROW_RIGHT 5
#define BOOTMAGIC_LITE_COLUMN_RIGHT 0

// Set USB polling rate to 1000 Hz
#define USB_POLLING_INTERVAL_MS 1

// Keyboard Matrix (Rows are doubled)
#define MATRIX_ROWS 10
#define MATRIX_COLS 6

#define DIODE_DIRECTION COL2ROW

// Encoder support
#define ENCODERS_PAD_A { }
#define ENCODERS_PAD_B { }
#define ENCODER_RESOLUTIONS { }

#define ENCODER_RESOLUTIONS_RIGHT { 4 }

// Define Communication
#define USE_SERIAL

// Allows dual displays to show modifiers etc
#define SERIAL_USE_MULTI_TRANSACTION
#define SPLIT_MODS_ENABLE
#define SPLIT_TRANSPORT_MIRROR
#define SPLIT_LAYER_STATE_ENABLE

#ifdef RGB_MATRIX_ENABLE
#    define RGB_MATRIX_SPLIT {29, 29}
#    define RGBLED_NUM 58
#    define DRIVER_LED_TOTAL 58
#    define RGB_DISABLE_WHEN_USB_SUSPENDED // turn off effects when suspended
#    define RGB_MATRIX_MAXIMUM_BRIGHTNESS 128
#endif

# define OLED_BRIGHTNESS 0

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

/* DEBUG, only useable with console and when you disable other features */
/* #define DEBUG_MATRIX_SCAN_RATE */
