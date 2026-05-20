#ifndef FSM_H
#define FSM_H

#include <stdint.h>
#include <stddef.h>

// Generic types for states and events
typedef uint8_t fsm_state_t;
typedef uint8_t fsm_event_t;

#define FSM_EVENT_NONE 0

// Structure defining a single row in the Transition Table
typedef struct {
    fsm_state_t current_state;
    fsm_event_t event;
    fsm_state_t next_state;
    void (*transition_action)(void); // Pointer to the state function to execute
} fsm_transition_t;

// FSM Instance Structure
typedef struct {
    fsm_state_t current_state;
    const fsm_transition_t* transition_table;
    size_t table_size;
} fsm_t;

// FSM Engine Functions
void fsm_init(fsm_t* fsm, fsm_state_t initial_state, const fsm_transition_t* table, size_t table_size);
void fsm_process_event(fsm_t* fsm, fsm_event_t event);

#endif