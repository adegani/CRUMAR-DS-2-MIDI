/* midiFSM.c
 *  MIDI Finite State Machine for MIDI message handle
 *  Author: Alessio Degani, 2017 <alessio.degani@gmail.com>
 */

#include "midiFSM.h"
#include "midiDefs.h"

void midiFSM_reset( midiFSM_t *fsm ){
    fsm->state = RESET;
}

void midiFSM_init( midiFSM_t *fsm, void (*handleMessage)(midiFSM_event_t *e),
                    void (*handleSysex)( uint8_t byte ),
                    void (*handleRealTimeMessage)( uint8_t byte ) ){
    fsm->state          = RESET;
    fsm->handleMessage  = handleMessage;
    fsm->handleSysex    = handleSysex;
    fsm->handleRealTimeMessage    = handleRealTimeMessage;
}

static void dispatchMidiStatus( midiFSM_t *fsm, uint8_t byte ){
    fsm->MIDIEvent.status = byte;
    uint8_t event = byte & 0xF0;
    switch (event){
        case MIDI_SYSEX:
            fsm->handleSysex( byte );
            fsm->state = WAIT_BYTE;
            break;
        case MIDI_PROGRAM_CHANGE:
        case MIDI_CHANNEL_PRESS:
        case MIDI_TIME_CODE:
        case MIDI_SONG_SELECT:
            fsm->state = WAIT_DATA_BYTE;
            break;
        default:
            fsm->state = WAIT_DATA_BYTE_1;
    }
}

void dispatchMessage( midiFSM_t *fsm ){
    switch ( fsm->MIDIEvent.status ){
        case MIDI_NOTE_OFF       :
        case MIDI_NOTE_ON        :
        case MIDI_POLY_KEY_PRESS :
        case MIDI_CONTROL_CHANGE :
        case MIDI_CHANNEL_PRESS  :
        case MIDI_PITCH_BEND     :
        case MIDI_PROGRAM_CHANGE :
            fsm->handleMessage( &(fsm->MIDIEvent) );
            break;
        case 0:
            break;
        default:
            fsm->handleMessage( &(fsm->MIDIEvent) );
            fsm->MIDIEvent.status = 0;
    }
}

void dispatchRealTimeMessage(  midiFSM_t *fsm, uint8_t byte ){
    fsm->handleRealTimeMessage( byte );
}

midiFSM_retVal midiFSM_handleByte( midiFSM_t *fsm, uint8_t byte ){
    if ( (byte & 0xF8) == 0xF8 ){
        // REAL TIME MESSAGES... ignore the rest
        dispatchRealTimeMessage ( fsm, byte );
        return FSM_RET_OK;
    }

    // MIDI FSM
    switch (fsm->state){
        case RESET:
        case WAIT_BYTE:
            if ( byte & MIDI_STATUS ){
                dispatchMidiStatus( fsm, byte );
            }
            break;
        case WAIT_DATA_BYTE:
            if ( byte & MIDI_STATUS ){
                dispatchMidiStatus( fsm, byte );
            } else {
                fsm->MIDIEvent.data1 = byte;
                fsm->MIDIEvent.data2 = 0;
                dispatchMessage( fsm );
            }
            break;
        case WAIT_DATA_BYTE_1:
            if ( byte & MIDI_STATUS ){
                dispatchMidiStatus( fsm, byte );
            } else {
                fsm->MIDIEvent.data1 = byte;
                fsm->MIDIEvent.data2 = 0;
                fsm->state = WAIT_DATA_BYTE_2;
            }
            break;
        case WAIT_DATA_BYTE_2:
            if ( byte & MIDI_STATUS ){
                dispatchMidiStatus( fsm, byte );
            } else {
                fsm->MIDIEvent.data2 = byte;
                fsm->state = WAIT_DATA_BYTE_1;
                dispatchMessage( fsm );
            }
            break;
        default:
            return FSM_RET_ERROR;
    }
    return FSM_RET_OK;
}
