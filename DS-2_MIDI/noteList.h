/* noteList.h
 *  Data structure that contains the note list to be played
 *  Author: Alessio Degani, 2016 <alessio.degani@gmail.com>
 */

#ifndef DS2_NOTELIST_H
#define DS2_NOTELIST_H

#include <stdint.h>

#define MAX_NOTES 10
#define NULL 0

enum playMode_e{ HIGHER, LAST }; 

typedef struct note {
    uint8_t noteNumber;
    uint8_t velocity;
    //struct note * next;
} note_t;

typedef struct noteList {
  note_t      midiNotes[ MAX_NOTES ];
  uint8_t     lastMidiNote;
  playMode_e  mode;
} noteList_t;


void initMidiNoteList( noteList_t *list, playMode_e mode ){
  list->mode = mode;
  list->lastMidiNote = 0;
  for (uint8_t i = 0; i < MAX_NOTES; i++ ){
    list->midiNotes[i].noteNumber = 0;
    list->midiNotes[i].velocity = 0;
    //list->next = NULL;
  }  
}

void pushNote(noteList_t *list, uint8_t noteNumber, uint8_t velocity) {
  for (uint8_t i = 0; i < MAX_NOTES; i++ ){
    if ( (list->midiNotes[i].velocity == 0) || (list->midiNotes[i].noteNumber == noteNumber) ){
      list->midiNotes[i].noteNumber = noteNumber;
      list->midiNotes[i].velocity = velocity;
      list->lastMidiNote = i;
      return;
    }
  }  
}

void popNote(noteList_t *list, uint8_t noteNumber) {
  uint8_t deletedNote = 0;
  for (uint8_t i = 0; i < MAX_NOTES; i++ ){
    if (list->midiNotes[i].noteNumber == noteNumber){
      list->midiNotes[i].noteNumber = 0;
      list->midiNotes[i].velocity = 0;
      deletedNote = i;
      return;
    }
  }
  if (deletedNote = list->lastMidiNote){
    for (uint8_t i = 0; i < MAX_NOTES; i++ ){
      if (list->midiNotes[i].velocity > 0){
        list->lastMidiNote = i;
        return;
      }
    }
  }
}

uint8_t midiNoteToPlay(noteList_t *list){
  uint8_t higherNote = 0;
  if (list->mode == LAST){
    return list->lastMidiNote;
  } else {
    for (uint8_t i = 0; i < MAX_NOTES; i++ ){
      if (list->midiNotes[i].noteNumber > higherNote){
        higherNote = i;
      }
    }
  } 
  return higherNote;
}

#endif // DS2_NOTELIST_H
