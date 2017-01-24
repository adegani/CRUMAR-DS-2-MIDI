/* noteList.h
 *  Linked List Data structure that contains the note list to be played
 *  Author: Alessio Degani, 2017 <alessio.degani@gmail.com>
 */

#ifndef DS2_NOTELIST_H
#define DS2_NOTELIST_H

#include <stdint.h>
#include <stdlib.h>

#define NULL 0

// Note priority ("ALL" works only for the MIDI OUT)
enum notePriority { LOWER, HIGHER, LAST, ALL };

// Note struct
typedef struct note {
    uint8_t noteNumber;
    uint8_t velocity;
    note *next;
} note_t;

// Note List Struct
typedef struct noteList {
  note_t        *head ;
  notePriority  priority;
} noteList_t;

// Init the note list
void initMidiNoteList( noteList_t *list, notePriority priority ){
  list->head      = (note_t*)malloc(sizeof(note_t));
  list->head->noteNumber= 0;
  list->head->velocity  = 0;
  list->head->next      = NULL;
  list->priority  = priority;
}

// Push a note to the list (if not already in)
void pushNote(noteList_t *list, uint8_t noteNumber, uint8_t velocity) {
  note_t *currentNote = list->head;
  
  while (currentNote->next != NULL) {
    currentNote = currentNote->next;
  }
  
  currentNote->next = (note_t*)malloc(sizeof(note_t));
  currentNote->next->noteNumber = noteNumber;
  currentNote->next->velocity = velocity;
  currentNote->next->next = NULL;
}

// Pop a note from the list (if exist)
void popNote(noteList_t *list, uint8_t noteNumber) {
  note_t *currentNote = list->head;
  note_t *temp_note = NULL;
  
  while (currentNote->next != NULL) {
    if (currentNote->next->noteNumber == noteNumber){
      break;
    }
    currentNote = currentNote->next;
  }
  temp_note = currentNote->next;
  currentNote->next = temp_note->next;
  free(temp_note);
}

// Select the note to be played based on priority
// NOTE: The DS-2 is MONOPHONIC!
uint8_t midiNoteToPlay(noteList_t *list){
  uint8_t lower = 127;
  uint8_t higher = 0;
  uint8_t last = 0;
  uint8_t retVal = 0;
 
  note_t *currentNote = list->head;
  
  do {
    if (currentNote->noteNumber > higher){ 
      higher = currentNote->noteNumber; 
    }
    if ( (currentNote->noteNumber < lower) && (currentNote->noteNumber != 0) ){ 
      lower = currentNote->noteNumber; 
    }
    last = currentNote->noteNumber;
    currentNote = currentNote->next;
  } while (currentNote != NULL);
  
  switch (list->priority) {
    case LOWER:
      retVal = lower;
      break;
    case HIGHER:
      retVal = higher;
      break;
    default:
      retVal = last;
      break;
  }
  return retVal;
}

#endif // DS2_NOTELIST_H
