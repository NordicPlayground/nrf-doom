
#include "nrf.h"
#include "bsp.h"

#undef PACKED_STRUCT

#include "doomkeys.h"
#include "d_event.h"

bool button_prev_state[4];

char button_map[] = {KEY_UPARROW, KEY_DOWNARROW, KEY_ENTER, KEY_BACKSPACE};

void N_ReadButtons() 
{
    static event_t event;

    bool button_state[4];
    bool button_posedge[4];
    bool button_negedge[4];
    for (int i=0; i<4; i++) {
        button_state[i] = bsp_board_button_state_get(i);
        button_posedge[i] = button_state[i] && !button_prev_state[i];
        button_negedge[i] = !button_state[i] && button_prev_state[i];
    }

    for (int i=0; i<4; i++) {
        if (button_posedge[i]) {
            event.type = ev_keydown;
            event.data1 = button_map[i];
            event.data2 = 0;
            event.data3 = 0;
            D_PostEvent(&event);
        }
        else if (button_negedge[i]) {
            event.type = ev_keyup;
            event.data1 = button_map[i];
            event.data2 = 0;
            event.data3 = 0;
            D_PostEvent(&event);
        }
    }

    for (int i=0; i<4; i++) {
        button_prev_state[i] = button_state[i];
    }


}

int N_ButtonState(int num)
{
    return button_prev_state[num];
}