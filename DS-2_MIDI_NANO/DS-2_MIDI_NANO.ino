/* DS-2 MIDI IN NANO
 *  Add MIDI input capabilities to CRUMAR DS-2
 *  Author: Alessio Degani, 2016 <alessio.degani@gmail.com>
 */

#include "pinConfig.h"
#include "noteList.h"

noteList_t midiNotes;

byte incomingByte;
byte note;
byte velocity;

int action=2; //0 =note off ; 1=note on ; 2= nada

void playNote(byte note, byte velocity){

  // A note off -> close the gate
  if (velocity == 0) {
    digitalWrite(DS2_GATE_NEG, HIGH);
    return;
  }
  
  if (( note >= 41 ) && ( note <= 84 )) {
    note = note - 40;
    ( (note&32) == 32 ) ? digitalWrite(DS2_32o, HIGH): digitalWrite(DS2_32o, LOW);
    ( (note&16) == 16 ) ? digitalWrite(DS2_16o, HIGH): digitalWrite(DS2_16o, LOW);
    ( (note&8)  == 8 )  ? digitalWrite(DS2_8o, HIGH): digitalWrite(DS2_8o, LOW);
    ( (note&4)  == 4 )  ? digitalWrite(DS2_4o, HIGH): digitalWrite(DS2_4o, LOW);
    ( (note&2)  == 2 )  ? digitalWrite(DS2_2o, HIGH): digitalWrite(DS2_2o, LOW);
    ( (note&1)  == 1 )  ? digitalWrite(DS2_1o, HIGH): digitalWrite(DS2_1o, LOW);
    digitalWrite(DS2_GATE_NEG, LOW);
  }

}

void setup() {
  // Setup decoded output port for DS-2
  pinMode(DS2_GATE_NEG, OUTPUT);
  pinMode(DS2_1o, OUTPUT);
  pinMode(DS2_2o, OUTPUT);
  pinMode(DS2_4o, OUTPUT);
  pinMode(DS2_8o, OUTPUT);
  pinMode(DS2_16o, OUTPUT);
  pinMode(DS2_32o, OUTPUT);

  // Default GATE for DS-2 is HIGH
  digitalWrite(DS2_GATE_NEG, HIGH); // DS-2 has a NOT-GATE

  // Serial port (MIDI), default baudrate 31250
  Serial.begin(31250);

  // Initialize the note list
  initMidiNoteList( &midiNotes, HIGHER );
}

void play( noteList_t *list ){
  byte noteIdx = midiNoteToPlay(list);
  playNote( list->midiNotes[noteIdx].noteNumber, list->midiNotes[noteIdx].velocity );
  /*if (list->midiNotes[noteIdx].velocity == 0){
    digitalWrite(DS2_1o, HIGH);
    digitalWrite(DS2_2o, LOW);
    }else{
      digitalWrite(DS2_1o, LOW);
    digitalWrite(DS2_2o, HIGH);
    }*/
}

void loop() {
 if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // wait for as status-byte, channel 1, note on or off
    if (incomingByte== 144){ // note on message starting starting
      action=1;
    }else if (incomingByte== 128){ // note off message starting
      action=0;
    }else if (incomingByte== 208){ // aftertouch message starting
       //not implemented yet
    }else if (incomingByte== 160){ // polypressure message starting
       //not implemented yet
    }else if ( (action==0)&&(note==0) ){ // if we received a "note off", we wait for which note (databyte)
      note=incomingByte;
      //playNote(note, 0);
      popNote(&midiNotes, note);
      note=0;
      velocity=0;
      action=2;
    }else if ( (action==1)&&(note==0) ){ // if we received a "note on", we wait for the note (databyte)
      note=incomingByte;
    }else if ( (action==1)&&(note!=0) ){ // ...and then the velocity
      velocity=incomingByte;
      //playNote(note, velocity);
      pushNote(&midiNotes, note, velocity);
      note=0;
      velocity=0;
      action=0;
    }else{
      //nada
    }
  }  

  play( &midiNotes );

}
