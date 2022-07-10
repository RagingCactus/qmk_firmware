// Configuration for the atmega32u4 based Arduino Pro Micro

// QMK DFU Config
// Leave QMK DFU bootloader using the key under OLED/encoder on both halfs
#define QMK_ESC_OUTPUT F6 // usually COL
#define QMK_ESC_INPUT B4 // usually ROW
#define QMK_LED B0

// The pin configurations are specific to the MCU

#define MATRIX_ROW_PINS { D4, C6, D7, E6, B4 }
#define MATRIX_COL_PINS { B1, B2, B3, B6, F7, F6 }

#define ENCODERS_PAD_A_RIGHT { F4 }
#define ENCODERS_PAD_B_RIGHT { F5 }

#define SPLIT_HAND_PIN B5

#define SOFT_SERIAL_PIN D2

#define RGB_DI_PIN D3

/* The Pro Micro has very limited flash memory, try to squeeze out as much as
 * possible using the following settings */

/* disable action features */
#define NO_ACTION_ONESHOT

/* Squeezing AVR Guide suggests this */
#undef LOCKING_SUPPORT_ENABLE
#undef LOCKING_RESYNC_ENABLE

/* Only works when you're using less than 8 layers!
 * It does save some bytes though! */
#define LAYER_STATE_8BIT
