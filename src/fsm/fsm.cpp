#include "fsm.h"

void fsm_init(fsm_t* fsm, fsm_state_t initial_state, const fsm_transition_t* table, size_t table_size) {
    if (fsm == NULL) return;
    fsm->current_state = initial_state;
    fsm->transition_table = table;
    fsm->table_size = table_size;
}

// The core engine that processes all events and searches the table
void fsm_process_event(fsm_t* fsm, fsm_event_t event) {
    if (fsm == NULL || event == FSM_EVENT_NONE || fsm->transition_table == NULL) return;

    for (size_t i = 0; i < fsm->table_size; i++) {
        // Compare current state and the received event with the table rules
        if (fsm->transition_table[i].current_state == fsm->current_state && 
            fsm->transition_table[i].event == event) {
            
            // Execute the state action function defined by the user
            if (fsm->transition_table[i].transition_action != NULL) {
                fsm->transition_table[i].transition_action();
            }

            // Update the system state (it will safely stay in this state)
            fsm->current_state = fsm->transition_table[i].next_state;
            return; 
        }
    }
}