/* config.h
 *  Config and Arduino pin names
 *  Author: Alessio Degani, 2016 <alessio.degani@gmail.com>
 */

#ifndef DS2_CONFIG_H
#define DS2_CONFIG_H

// General Config
//#define DS2_FULL_VARIANT // Uncomment to use FULL variant instead of NANO

// Note priority [LAST|HIGHER|LOWER]
#define PLAY_MODE_MIDI LAST

// Gate mode [GATE_SINGLE|GATE_RETRIGGER]
#define DEFAULT_GATE_MODE GATE_SINGLE

#define NOTE_TRANSPOSE_SEMITONE 0

// Default MIDI channel
#define DEFAULT_MIDI_CH 0

// Control Change for P431-MIDI
#define DS2_PLAY_MODE      0x66
#define DS2_GATE_MODE      0x67
#define DS2_SET_CHANNEL    0x68
#define DS2_HOLD_NOTE      0x69
#define DS2_ARP_MODE       0x6A
#define DS2_TRANSPOSE_SEMI 0x6B

// Configure the ATMega328 pin names

// OUTPUTS to CRUMAR
#define DS2_GATE_NEG  2
#define DS2_32o       3
#define DS2_16o       4
#define DS2_8o        5
#define DS2_4o        6
#define DS2_2o        7
#define DS2_1o        8

// DS-2 MIDI NOTE range
#define DS2_LOWER_NOTE  41
#define DS2_HIGHER_NOTE 84

// MIDI ports and LED
#define MIDI_LED      13
#define MIDI_OUT      0
#define MIDI_IN       1

// ----- NOT USED IN DS-2 MIDI IN only VERSION -----

// COUNTER
#define COUNT_UP_NEG  9
#define COUNT_CLR     10

// DIGIPOT --> OUT (pitch shifter)
#define POT_SCK       11
#define POT_DI        12

// ANALOG POT --> READ (pitch shifter)
#define PS_POT1       A5
#define PS_POT2       A4
#define PS_POT3       A3

// MUX READ (INPUT from CRUMAR keyboard)
#define K0            A0
#define K1            A1
#define K2            A2

#endif // DS2_CONFIG_H
