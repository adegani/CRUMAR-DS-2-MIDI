/* DS-2 MIDI IN NANO
 *  Add MIDI input capabilities to CRUMAR DS-2
 *  Author: Alessio Degani, 2016 <alessio.degani@gmail.com>
 */

#include "config.h"
#include "noteList.h"
#include "midiDefs.h"

// Fot the MIDI FSM
enum status_e { MIDI_IDLE, MIDI_WAIT_NOTE_ON, MIDI_WAIT_NOTE_OFF, MIDI_WAIT_VELOCITY, MIDI_WAIT_CC, MIDI_WAIT_CCVAL, MIDI_WAIT_PITCH_LSB, MIDI_WAIT_PITCH_MSB, MIDI_WAIT_SYSEX };
status_e status = MIDI_IDLE;

enum gate_mode_e { GATE_SINGLE, GATE_RETRIGGER };
gate_mode_e gate_mode = DEFAULT_GATE_MODE;

byte midiChannel = DEFAULT_MIDI_CH;

notePriority priority = PLAY_MODE_MIDI;
noteList_t midiNotes;

byte midiByte;
byte note;
byte velocity;
byte lastNote;
byte ccNum;
byte ccVal;
byte pitchLSB;
byte pitchMSB;

// MIDI led activity, led_status is for pesistence of the led, otherwise, the led will stay on too few
bool          midi_led_status         = 0;
unsigned int  midi_led_duration_loops = 3;
unsigned int  midi_led_timer          = 0;

void pitchWheel( byte pitchLSB, byte pitchMSB ){
  uint16_t pitchVal = pitchMSB*128;
  pitchVal = pitchVal + pitchLSB;
}

void controlChange( byte ccNum, byte ccVal ){
  /*Serial.print(ccNum);
  Serial.print("-");
  Serial.println(ccVal);*/
  switch (ccNum){
    case MIDI_MODWHEEL:
      break;
    case MIDI_PLAY_MODE:
      if ( ccVal == 0 ){
        priority = LAST;
      } else if ( ccVal == 1 ) {
        priority = HIGHER;
      } else if ( ccVal == 2 ) {
        priority = LOWER;
      }
      midiNotes.priority = priority;
      break;
    case MIDI_GATE_MODE:
      if ( ccVal == 0 ){
        gate_mode = GATE_SINGLE;
      } else {
        gate_mode = GATE_RETRIGGER;
      }
      break;
    case MIDI_SET_CHANNEL:
      if ( (ccVal >= 0) && (ccVal < 16) ){
        midiChannel = ccVal-1;
      }
      break;
    case MIDI_HOLD_NOTE:
      break;
    case MIDI_ARP_MODE:
      break;
    case MIDI_ALL_SOUND_OFF:
      break;
    case MIDI_RESET_CTRL:
      break;
    case MIDI_ALL_NOTE_OFF:
      break;
    case MIDI_OMNI_OFF:
      break;
    case MIDI_OMNI_ON:
      break;
    case MIDI_POLY_OFF:
      break;
    case MIDI_POLY_ON:
      break;
    default:
      break;
  }
}

void playNote( byte note, byte velocity ){

  // A note off -> close the gate
  if ( (velocity == 0) || (note < 41) || (note > 84) ) {
    digitalWrite( DS2_GATE_NEG, HIGH );
    return;
  }
  
  if (( note >= 41 ) && ( note <= 84 )) {
    note = note - 40;

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
  initMidiNoteList( &midiNotes, priority );
}

void sequencer( noteList_t *list ){
  // If there are note to play or send via MIDI, this is the right moment to do that!
  byte noteIdx = midiNoteToPlay(list);
  //playNote( list->midiNotes[noteIdx].noteNumber, list->midiNotes[noteIdx].velocity );
  playNote( noteIdx, 127 );

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
    
    // read the incoming byte:
    midiByte = Serial.read();

    if (midiByte & MIDI_STATUS){
      // Just in case I've missed some byte and a new status is arrived
      status = MIDI_IDLE;
    }

    // MIDI messages FSM
    if ( status == MIDI_IDLE ){
      /*if ( midiByte & MIDI_SYSEX){
        status = MIDI_WAIT_SYSEX;
      } else*/
      if ( midiByte == (MIDI_NOTE_ON | midiChannel) ){
        status = MIDI_WAIT_NOTE_ON;
      } else if ( midiByte == (MIDI_NOTE_OFF | midiChannel) ){
        status = MIDI_WAIT_NOTE_OFF;
      } else if (midiByte == (MIDI_CONTROL_CHANGE | midiChannel) ){
        status = MIDI_WAIT_CC;
      } else if ( midiByte == (MIDI_PITCH_BEND | midiChannel) ){
        status = MIDI_WAIT_PITCH_LSB;
      } else if ( (midiByte == (MIDI_ALL_NOTE_OFF | midiChannel)) ||
                 (midiByte == (MIDI_ALL_SOUND_OFF | midiChannel)) ){
        initMidiNoteList( &midiNotes, priority );
        status = MIDI_IDLE;
      }
    } else if ( status == MIDI_WAIT_NOTE_ON ){
      note = midiByte;
      status = MIDI_WAIT_VELOCITY;
    } else if ( status == MIDI_WAIT_NOTE_OFF ){
      note = midiByte;
      popNote( &midiNotes, note );
      status = MIDI_IDLE;
    } else if ( status == MIDI_WAIT_VELOCITY ){
      velocity = midiByte;
      if ( velocity != 0 ){
        pushNote( &midiNotes, note, velocity );
      } else {
        popNote( &midiNotes, note );
      }
      status = MIDI_IDLE;
    } else if ( status == MIDI_WAIT_CC ){
      ccNum = midiByte;
      status = MIDI_WAIT_CCVAL;
    } else if ( status == MIDI_WAIT_CCVAL ){
      ccVal = midiByte;
      controlChange( ccNum, ccVal );
      status = MIDI_IDLE;
    } else if ( status == MIDI_WAIT_PITCH_LSB ){
      pitchLSB = midiByte;
      status = MIDI_WAIT_PITCH_MSB;
    } else if ( status == MIDI_WAIT_PITCH_MSB ){
      pitchMSB = midiByte;
      pitchWheel( pitchLSB, pitchMSB );
      status = MIDI_IDLE;
    }  else if ( status == MIDI_WAIT_SYSEX ){
      // TODO
      status = MIDI_IDLE;
    }
  }

  // Sequence the note received
  sequencer( &midiNotes );

  digitalWrite( MIDI_LED, midi_led_status );
  
}
