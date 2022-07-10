# MCU name
MCU = atmega32u4

# Bootloader selection
#BOOTLOADER = caterina # default for pro micro
BOOTLOADER = qmk-dfu # Requires you to reflash the bootloader using an ISP programmer (another arduino can serve that purpose)

## Options ##
BOOTMAGIC_ENABLE = yes     # Virtual DIP switch configuration
EXTRAKEY_ENABLE = yes       # Audio control and System control
MOUSEKEY_ENABLE = yes # Scrolling for encoders

CONSOLE_ENABLE = no         # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration

LTO_ENABLE = yes

SPLIT_KEYBOARD = yes

ENCODER_ENABLE = yes
OLED_ENABLE = yes

# RGBLIGHT_ENABLE = yes
# RGBLIGHT_DRIVER = WS2812 # Actually SK6812 mini-E, but should be compatible...

RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = WS2812 # Actually SK6812 mini-E, but should be compatible...

# Space saving
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no
MAGIC_ENABLE = no

AUTO_SHIFT_ENABLE = no
NKRO_ENABLE = no # save space and be as compatible as possible
TAP_DANCE_ENABLE = yes

# Debounce eagerly (report change immediately) to keep latency low
# See https://michael.stapelberg.ch/posts/2021-05-08-keyboard-input-latency-qmk-kinesis/#eagerdebounce
DEBOUNCE_TYPE = sym_eager_pk

# DEBUG
# CONSOLE_ENABLE = yes
