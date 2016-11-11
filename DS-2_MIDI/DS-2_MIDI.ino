/* DS-2 MIDI IN NANO
 *  Add MIDI input capabilities to CRUMAR DS-2
 *  Author: Alessio Degani, 2016 <alessio.degani@gmail.com>
 */

#include "config.h"
#include "noteList.h"
#include "midiDefs.h"

// Fot the MIDI FSM
enum status_e { IDLE, WAIT_NOTE_ON, WAIT_NOTE_OFF, WAIT_VELOCITY, WAIT_CC, WAIT_PITCH };
status_e status = IDLE;

byte midiChannel = DEFAULT_MIDI_CH;

notePriority priority = PLAY_MODE_MIDI;
noteList_t midiNotes;

byte midiByte;
byte note;
byte velocity;

void playNote(byte note, byte velocity){

  // A note off -> close the gate
  if (velocity == 0) {
    digitalWrite(DS2_GATE_NEG, HIGH);
    return;
  }

  if (( note >= 41 ) && ( note <= 84 )) {
    note = note - 40;
    ( (note & 32) == 32 ) ? digitalWrite(DS2_32o, HIGH): digitalWrite(DS2_32o, LOW);
    ( (note & 16) == 16 ) ? digitalWrite(DS2_16o, HIGH): digitalWrite(DS2_16o, LOW);
    ( (note & 8)  == 8 )  ? digitalWrite(DS2_8o, HIGH): digitalWrite(DS2_8o, LOW);
    ( (note & 4)  == 4 )  ? digitalWrite(DS2_4o, HIGH): digitalWrite(DS2_4o, LOW);
    ( (note & 2)  == 2 )  ? digitalWrite(DS2_2o, HIGH): digitalWrite(DS2_2o, LOW);
    ( (note & 1)  == 1 )  ? digitalWrite(DS2_1o, HIGH): digitalWrite(DS2_1o, LOW);
    digitalWrite(DS2_GATE_NEG, LOW);
  }

}

void setup() {
  // Setup decoded output port for DS-2
  pinMode( DS2_GATE_NEG, OUTPUT );
  pinMode( DS2_1o,       OUTPUT );
  pinMode( DS2_2o,       OUTPUT );
  pinMode( DS2_4o,       OUTPUT );
  pinMode( DS2_8o,       OUTPUT );
  pinMode( DS2_16o,      OUTPUT );
  pinMode( DS2_32o,      OUTPUT );
  pinMode( MIDI_LED,     OUTPUT );

  // Default GATE for DS-2 is HIGH
  digitalWrite( DS2_GATE_NEG, HIGH ); // DS-2 has a NOT-GATE

  // Serial port (MIDI), default baud rate 31250
  Serial.begin( MIDI_RATE );

  // Initialize the note list
  initMidiNoteList( &midiNotes, priority );
}

void sequencer( noteList_t *list ){
  // If there are note to play or send via MIDI, this is the right moment to do that!
  byte noteIdx = midiNoteToPlay(list);
  playNote( list->midiNotes[noteIdx].noteNumber, list->midiNotes[noteIdx].velocity );

  #ifdef DS2_FULL_VARIANT
  // TODO: if FULL board, sequence the MIDI OUT notes
  #endif
}

void loop() {
  #ifdef DS2_FULL_VARIANT
  // TODO: if FULL board, read the DS-2 keyboard
  #endif



  if (Serial.available() > 0) {

    // MIDI LED actovity ON
    digitalWrite( MIDI_LED, HIGH );

    // read the incoming byte:
    midiByte = Serial.read();

    if (midiByte & 0x80){
      // Just in case I've missed some byte
      status = IDLE;
    }

    // MIDI messages FSM
    if (status == IDLE){
      if ( midiByte == (MIDI_NOTE_ON | midiChannel) ){
        status = WAIT_NOTE_ON;
      } else if ( midiByte == (MIDI_NOTE_OFF | midiChannel) ){
        status = WAIT_NOTE_OFF;
      } else if (midiByte == (MIDI_CONTROL_CHANGE | midiChannel) ){
        status = WAIT_CC;
      } else if ( midiByte == (MIDI_PITCH_BEND | midiChannel) ){
        status = WAIT_PITCH;
      } else if ( (midiByte == (MIDI_ALL_NOTE_OFF | midiChannel)) ||
                 (midiByte == (MIDI_ALL_SOUND_OFF | midiChannel)) ){
        initMidiNoteList( &midiNotes, priority );
        status = IDLE;
      }
    } else if (status == WAIT_NOTE_ON){
      note = midiByte;
      status = WAIT_VELOCITY;
    } else if (status == WAIT_NOTE_OFF){
      note = midiByte;
      popNote(&midiNotes, note);
      status = IDLE;
    } else if (status == WAIT_VELOCITY){
      velocity = midiByte;
      if (velocity != 0){
        pushNote(&midiNotes, note, velocity);
      } else {
        popNote(&midiNotes, note);
      }
      status = IDLE;
    } else if (status == WAIT_CC){
      // TODO
      status = IDLE;
    } else if (status == WAIT_PITCH){
      // TODO
      status = IDLE;
    }

    // MIDI LED actovity OFF
    digitalWrite( MIDI_LED, LOW );
  }

  // Sequence the note received
  sequencer( &midiNotes );
}
