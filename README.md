# teensyIKBD
Quick and dirty Atari ST IKBD to USB keyboard interface for Teensy 2.0.

I created this to interface a Mega ST external keyboard to a Mac, but any ST keyboard (including ST(F), STE, Mega STE, TT and Falcon) should work too.

Simply connect RX and TX wires to Pins 7 and 8 respectively plus Power and GND. Upload the sketch to your device and the device should show up as a normal keyboard.

# Mapping keys
Some of the keys are mapped to make the keyboard usable on a Mac. For a PC you may want to change some of those by editing the mappings in keymap.ino.

Most of the keys result in a corresponding key code to be sent to the computer, except the following:

Alternate: Left-CMD/Gui.
Caps Lock: Right-Alt.
Help: F11
Undo: Right-CMD + Z. (there is a small delay between the CMD key and Z keypress)
Keypad ( and ): Volume down and up.


