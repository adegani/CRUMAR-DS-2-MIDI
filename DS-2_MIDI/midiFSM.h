/* midiFSM.h
 *  MIDI Finite State Machine for MIDI message handle
 *  Author: Alessio Degani, 2017 <alessio.degani@gmail.com>
 */

#ifndef MIDIFSM_H
#define MIDIFSM_H

#include <stdint.h>

enum midiFSM_state { RESET, WAIT_BYTE, WAIT_DATA_BYTE, WAIT_DATA_BYTE_1, WAIT_DATA_BYTE_2};
enum midiFSM_retVal{ FSM_RET_OK, FSM_RET_ERROR };
typedef enum midiFSM_state midiFSM_state;
typedef enum midiFSM_retVal midiFSM_retVal;

typedef struct midiFSM_event {
    uint8_t status;
    uint8_t data1;
    uint8_t data2;
} midiFSM_event_t;

typedef struct midiFSM {
    midiFSM_state state;
    midiFSM_event_t MIDIEvent;
    void (*handleMessage)(midiFSM_event_t *e);
    void (*handleSysex)( uint8_t byte );
    void (*handleRealTimeMessage)( uint8_t byte );
} midiFSM_t;

#ifdef __cplusplus
extern "C" {
#endif

void midiFSM_reset  ( midiFSM_t *fsm );
void midiFSM_init   ( midiFSM_t *fsm, void (*handleMessage)(midiFSM_event_t *e),
                    void (*handleSysex)( uint8_t byte ),
                    void (*handleRealTimeMessage)( uint8_t byte ) );
midiFSM_retVal  midiFSM_handleByte ( midiFSM_t *fsm, uint8_t byte );

#ifdef __cplusplus
}
#endif

#endif //MIDIFSM_H
