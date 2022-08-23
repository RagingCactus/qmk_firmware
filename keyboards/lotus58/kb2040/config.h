/*
Copyright 2022 Marius Renner

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

// Configuration for the Adafruit KB2040, a RP2040 based MCU with Pro Micro
// compatible pinout

#define MATRIX_ROW_PINS { GP4, GP5, GP6, GP7, GP8 }
#define MATRIX_COL_PINS { GP18, GP19, GP20, GP10, GP26, GP27 }

#define ENCODERS_PAD_A_RIGHT { GP29 }
#define ENCODERS_PAD_B_RIGHT { GP28 }

#define SPLIT_HAND_PIN GP9

#define SOFT_SERIAL_PIN GP1

// #define USB_VBUS_PIN GP12
// OR
// Detects USB and assigns master (Not recommended on Elite C/Pro Micro mix)
#define SPLIT_USB_DETECT
#define SPLIT_USB_TIMEOUT 2000
#define SPLIT_USB_TIMEOUT_POLL 10

#define RGB_DI_PIN GP0

// Other special settings for RP2040
#define HAL_USE_PIO TRUE
#define HAL_USE_I2C TRUE

#define SERIAL_PIO_USE_PIO1

// https://github.com/qmk/qmk_firmware/issues/17115#issuecomment-1157548566
// #define SERIAL_USART_SPEED 57600

#ifdef RGB_MATRIX_ENABLE
#    define WS2812_PIO_USE_PIO
#endif
