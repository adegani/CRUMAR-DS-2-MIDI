/* midiDefs.h
 *  Definitions of MIDI messages and events
 *  Ref: MIDI Manufacturers Association and https://www.midi.org
 *  Author: Alessio Degani, 2016 <alessio.degani@gmail.com>
 */

#ifndef DS2_MIDIDEFS_H
#define DS2_MIDIDEFS_H

#define MIDI_NOTE_OFF       0x80
#define MIDI_NOTE_ON        0x90
#define MIDI_POLY_KEY_PRESS 0xA0
#define MIDI_CONTROL_CHANGE 0xB0
#define MIDI_PROGRAM_CHANGE 0xC0
#define MIDI_CH_PRESSURE    0xD0
#define MIDI_PITCH_BEND     0xE0
#define MIDI_SYSEX          0xF0

// Channel mode (after CONTROL CHANGE)
#define MIDI_ALL_SOUND_OFF  0x78
#define MIDI_RESET_CTRL     0x79
#define MIDI_ALL_NOTE_OFF   0x7B
#define MIDI_OMNI_OFF       0x7C
#define MIDI_OMNI_ON        0x7D
#define MIDI_POLY_OFF       0x7E
#define MIDI_POLY_ON        0x7F

#define MIDI_RESET_ALL      0xFF

#endif // DS2_MIDIDEFS_H
