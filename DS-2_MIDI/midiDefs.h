/* midiDefs.h
 *  Definitions of MIDI messages and events
 *  Ref: MIDI Manufacturers Association and https://www.midi.org
 *  Author: Alessio Degani, 2016 <alessio.degani@gmail.com>
 */

#ifndef DS2_MIDIDEFS_H
#define DS2_MIDIDEFS_H

#define MIDI_RATE           31250

// --- MIDI STATUS byte ---
#define MIDI_STATUS         0x80
// Status with 2 data bytes
#define MIDI_NOTE_OFF       0x80
#define MIDI_NOTE_ON        0x90
#define MIDI_POLY_KEY_PRESS 0xA0
#define MIDI_CONTROL_CHANGE 0xB0
#define MIDI_PITCH_BEND     0xE0
// Status with 1 data byte
#define MIDI_PROGRAM_CHANGE 0xC0
#define MIDI_CHANNEL_PRESS  0xD0

// SysEX
#define MIDI_SYSEX          0xF0
#define MIDI_TIME_CODE      0xF1
#define MIDI_SONG_POSITION  0xF2
#define MIDI_SONG_SELECT    0xF3
#define MIDI_END_OF_SYSEX   0xF7

// --- MIDI CHANNEL MODE ---
// Control Change (Defaults controllers)
#define MIDI_MODWHEEL       0x01

// Channel mode (CONTROL CHANGE reserved)
#define MIDI_ALL_SOUND_OFF  0x78
#define MIDI_RESET_CTRL     0x79
#define MIDI_ALL_NOTE_OFF   0x7B
#define MIDI_OMNI_OFF       0x7C
#define MIDI_OMNI_ON        0x7D
#define MIDI_POLY_OFF       0x7E
#define MIDI_POLY_ON        0x7F

// --- System REAL-TIME messages ---
#define MIDI_CLOCK          0xF8
#define MIDI_START          0xFA
#define MIDI_CONTINUE       0xFB
#define MIDI_STOP           0xFC
#define MIDI_ACTIVE_SENS    0xFE
#define MIDI_RESET_ALL      0xFF

#endif // DS2_MIDIDEFS_H
