# Lotus58

![lotus58](imgur.com image replace me!)

Working example of QMK for the Lotus58. Note that this repository is intended to be used by forking it, as you may need to alter some settings for your specific build.
Nonetheless it should be a good starting point for a QMK build with working per-key RGB.

The current configurations assumes you re-flash the bootloader of your Pro Micros to the QMK DFU bootloader.
This bootloader is much more pleasant to work with for frequent re-flashing than the default bootloader.
It also allows you to easily enter the bootloader by holding a button when you plug in your keyboard and also exit the bootloader using a keypress instead of having to re-plug your keyboard.
See the [QMK DFU documentation](https://docs.qmk.fm/#/flashing?id=qmk-dfu) for more information.
You will need to flash this using an ISP programmer.
Another Arduino Pro Micro, Arduino Nano or similar can do the job.
See the [ISP flashing guide](https://docs.qmk.fm/#/isp_flashing_guide?id=isp-flashing-guide) for more information on flashing.

You can also change the bootloader to `caterina` and fix any errors that pop up to use the default bootloader instead..

You can also change the bootloader to `caterina` and fix any errors that pop up to use the default bootloader instead.

* Keyboard Maintainer: [TweetyDaBird](https://github.com/TweetyDaBird)
* Hardware Supported:
  * Tested with Lotus58 Glow PCB v1.23
  * Tested only with per-key RGB LEDs, without the optional underglow LEDs
  * Tested with one OLED (left side) and one encoder (right side)
* Hardware Availability:
  * See the [Hardware Repository](https://github.com/TweetyDaBird/Lotus58), where you can find the files to order PCBs yourself or order directly from the hardware creator

Make example for this keyboard (after setting up your build environment):

    make tweetydabird/lotus58:default

Flashing example for this keyboard:

    make tweetydabird/lotus58:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).
