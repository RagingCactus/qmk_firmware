// Copyright 2024 Markus Knutsson (@TweetyDaBird)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#define SERIAL_PIO_USE_PIO1 // Force the usage of PIO1 peripheral, by default the Serial implementation uses the PIO0 peripheral

#define I2C_DRIVER I2CD1
#define I2C1_SDA_PIN GP2
#define I2C1_SCL_PIN GP3

// TODO
// Use EITHER USB_VBUS_PIN OR the auto detection!
// Chances are you don't have a USB_VBUS_PIN on your KB2040, unless you do what
// I did and build a small circuit that provides 0V if VBUS is not present and
// ~3.3V when it is present and provide that signal to GP12 on the KB2040 by
// using the JST connector. If you did not build that circuit, this won't work!
// #define USB_VBUS_PIN GP12
