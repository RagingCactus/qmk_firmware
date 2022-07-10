MCU = atmega32u4

# Bootloader selection
#BOOTLOADER = caterina # default for pro micro
BOOTLOADER = qmk-dfu # Requires you to reflash the bootloader using an ISP programmer (another arduino can serve that purpose)

# DEBUG
# We don't have enough space for this, if you need it, kill remove the OLED
# code (for example)
# CONSOLE_ENABLE = yes
