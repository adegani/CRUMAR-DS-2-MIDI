/* DS-2 MIDI IN NANO
 *  Add MIDI input capabilities to CRUMAR DS-2
 *  Author: Alessio Degani, 2016 <alessio.degani@gmail.com>
 */

#include "config.h"
#include "noteList.h"
#include "midiDefs.h"
#include "midiFSM.h"

midiFSM_t midiFSM;

enum gate_mode_e { GATE_SINGLE, GATE_RETRIGGER };
gate_mode_e gate_mode = DEFAULT_GATE_MODE;

byte midiChannel = DEFAULT_MIDI_CH;

MidiNoteList<16> midiNotes;

byte lastNote;
byte midiByte;

// Note transposition
int transposeSemitone = NOTE_TRANSPOSE_SEMITONE;

// MIDI led activity, led_status is for pesistence of the led, otherwise, the led will stay on too few
bool          midi_led_status         = 0;
unsigned int  midi_led_duration_loops = 3;
unsigned int  midi_led_timer          = 0;

void playNote( byte note, byte velocity ){

  // TODO: forse questo ciclo adesso si può eliminare
  // A note off -> close the gate 
  if ( (velocity == 0) || (note < DS2_LOWER_NOTE) || (note > DS2_HIGHER_NOTE) ) {
    digitalWrite( DS2_GATE_NEG, HIGH );
    return;
  }

  note = note + transposeSemitone;

  if (( note >= DS2_LOWER_NOTE ) && ( note <= DS2_HIGHER_NOTE )) {
    note = note - DS2_LOWER_NOTE;

    if ( (gate_mode == GATE_RETRIGGER) && (lastNote != note) ) {
      digitalWrite( DS2_GATE_NEG, HIGH );
    }

    ( (note & 32) == 32 ) ? digitalWrite(DS2_32o, HIGH): digitalWrite(DS2_32o, LOW);
    ( (note & 16) == 16 ) ? digitalWrite(DS2_16o, HIGH): digitalWrite(DS2_16o, LOW);
    ( (note & 8)  == 8 )  ? digitalWrite(DS2_8o, HIGH): digitalWrite(DS2_8o, LOW);
    ( (note & 4)  == 4 )  ? digitalWrite(DS2_4o, HIGH): digitalWrite(DS2_4o, LOW);
    ( (note & 2)  == 2 )  ? digitalWrite(DS2_2o, HIGH): digitalWrite(DS2_2o, LOW);
    ( (note & 1)  == 1 )  ? digitalWrite(DS2_1o, HIGH): digitalWrite(DS2_1o, LOW);
    digitalWrite( DS2_GATE_NEG, LOW );
    lastNote = note;
  }

}

void handleCC(midiFSM_event_t *e){
  switch (e->data1){
    case MIDI_ALL_SOUND_OFF:
    case MIDI_ALL_NOTE_OFF:
    case MIDI_RESET_CTRL:
    case MIDI_RESET_ALL:
    midiNotes.reset();
  }
}

void handleMessage(midiFSM_event_t *e){
  switch (e->status){
    case MIDI_NOTE_ON:
      if (e->data2 == 0){
        midiNotes.remove(e->data1);
      } else {
        const bool firstNote = midiNotes.empty();
        midiNotes.add(MidiNote(e->data1, e->data2));
      }
      break;
    case MIDI_NOTE_OFF:
      midiNotes.remove(e->data1);
      break;
    case MIDI_CONTROL_CHANGE:
      handleCC(e);
      break;

  }
}
void handleSysex( uint8_t byte ){}
void handleRealTimeMessage( uint8_t byte ){}

void setup() {
  lastNote = 0;

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
  midiFSM_init(&midiFSM, &handleMessage, &handleSysex, &handleRealTimeMessage);

}

void sequencer( ){
  // If there are note to play or send via MIDI, this is the right moment to do that!
  byte currentNote = 0;
  midiNotes.getLast(currentNote);
  
  playNote( currentNote, 127 );

  #ifdef DS2_FULL_VARIANT
  // TODO: if FULL board, sequence the MIDI OUT notes
  #endif
}

void loop() {

  #ifdef DS2_FULL_VARIANT
  // TODO: if FULL board, read the DS-2 keyboard
  #endif

  // MIDI activity led mini FSM
  if ( midi_led_status == 1 ){
    midi_led_timer++;
    if ( midi_led_timer > midi_led_duration_loops ){
      midi_led_status = 0;
      midi_led_timer  = 0;
    }
  }


  if (Serial.available() > 0) {
    // A MIDI message is arrived

    // start MIDI led activity mini FSM
    midi_led_timer  = 0;
    midi_led_status = 1;
    
    midiByte = Serial.read();
    midiFSM_handleByte(&midiFSM,midiByte );

  }

  // Sequence the note received
  sequencer(  );

  digitalWrite( MIDI_LED, midi_led_status );

}
