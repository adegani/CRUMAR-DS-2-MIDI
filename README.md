# CRUMAR-DS-2-MIDI
Add MIDI features to Crumar DS-2 Analog synth using Arduino or bare ATMega328 (with Arduino Bootloader)

This work is published under GNU/GPL.

**NOTE:** This is a WORK IN PROGRESS! Use with caution!

## Abstract
The Crumar DS-2 is a vintage made in Italy analog synth. Unfortunately, it comes without a MIDI IN/OUT capabilityes.
The goal of this project is to add the MIDI port to this wonderful peace of gear by simply substitute the original keyboard decoder board (P-431 on the DS-2 schematics) whit a new one that add MIDI capabilities.

In this repository you can find the Arduino sketch and the schematics of the DS-2_MIDI board and the connection diagram of the Crumar P-431 board.

The DS-2_MIDI board comes in two fashions:

### DS-2 MIDI IN
This simple board provides only MIDI IN to the DS-2. The schematics comes with two microcontroller footprints: one for the Arduino Nano and one for the bare ATMega328 chip.

What you **can do** with this board?
* Receive MIDI NOTE ON/OFF messages

**NOTE:** with this board, the DS-2 keyboard will be **disabled**.

In order to use this variant, please make sure to comment the line
```C
#define DS2_FULL_VARIANT
```
in **config.h**, and refer to the schematic and board *P-431_MIDI_IN_NANO* or *P-431_MIDI_IN_BARE* depending on if you use Arduino Nano or ATMega328 bare chip.

### DS-2 MIDI FULL (under development)
This advanced board provides MIDI IN and OUT to the DS-2. The schematics comes with two microcontroller footprints: one for the Arduino Nano and one for the bare ATMega328 chip.

What you **can do** with this board?
* Receive MIDI NOTE ON/OFF messages
* Send MIDI NOTE ON/OFF from DS-2 keyboard
* Send and Receive MIDI pitch shift messages (TO BE IMPLEMENTED)

**NOTE:** with this board, the DS-2 keyboard are still **usable**.

In order to use this variant, please UNcomment the line
```C
#define DS2_FULL_VARIANT
```
in **config.h**, and refer to the schematic and board *P-431_MIDI_FULL*.

## DISCLAIMER
This is not a commercial work. USE IT AT YOUR OWN RISK. I'm not responsible of any damage to your Arduino / DS-2 or your person.
