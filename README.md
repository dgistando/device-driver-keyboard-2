# device-driver-keyboard-2
I was interested in creating a device driver for a keyboard/char device. I'm following advice and examples in Third Edition of Linux Device Drivers, by Jonathan Corbet, Alessandro Rubini, and Greg Kroah-Hartman. I just also want more practice with low level C-like languages.

## Goal
 - I want the driver to measure WPM typing speed when a trigger is hit on the keyboard. When the trigger is hit again it logs the speed to the kernel log.
