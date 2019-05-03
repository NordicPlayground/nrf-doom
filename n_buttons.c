
#include "nrf.h"
#include "bsp.h"

#undef PACKED_STRUCT

#include "doomkeys.h"
#include "d_event.h"

bool button_prev_state[4];

void N_ReadButtons() 
{
    bool button_state[4];
    bool button_posedge[4];
    bool button_negedge[4];
    for (int i=0; i<4; i++) {
        button_state[i] = bsp_board_button_state_get(i);
        button_posedge[i] = button_state[i] && !button_prev_state[i];
        button_negedge[i] = !button_state[i] && button_prev_state[i];
    }

    if (button_posedge[0]) {
        printf("BUTTON 0 DOWN\n");
        event_t event;
        event.type = ev_keydown;
        event.data1 = KEY_UPARROW;
        event.data2 = 0;
        event.data3 = 0;
        D_PostEvent(&event);
    }
    if (button_negedge[0]) {
        printf("BUTTON 0 UP\n");
        event_t event;
        event.type = ev_keyup;
        event.data1 = KEY_UPARROW;
        event.data2 = 0;
        event.data3 = 0;
        D_PostEvent(&event);
    }

    if (button_posedge[1]) {
        printf("BUTTON 1 DOWN\n");
        event_t event;
        event.type = ev_keydown;
        event.data1 = KEY_DOWNARROW;
        event.data2 = 0;
        event.data3 = 0;
        D_PostEvent(&event);
    }
    if (button_negedge[1]) {
        printf("BUTTON 1 UP\n");
        event_t event;
        event.type = ev_keyup;
        event.data1 = KEY_DOWNARROW;
        event.data2 = 0;
        event.data3 = 0;
        D_PostEvent(&event);
    }


    if (button_posedge[2]) {
        printf("BUTTON 2 DOWN\n");
        event_t event;
        event.type = ev_keydown;
        event.data1 = KEY_ENTER;
        event.data2 = 0;
        event.data3 = 0;
        D_PostEvent(&event);
    }
    if (button_negedge[2]) {
        printf("BUTTON 2 UP\n");
        event_t event;
        event.type = ev_keyup;
        event.data1 = KEY_ENTER;
        event.data2 = 0;
        event.data3 = 0;
        D_PostEvent(&event);
    }


    if (button_posedge[3]) {
        printf("BUTTON 3 DOWN\n");
        event_t event;
        event.type = ev_keydown;
        event.data1 = KEY_BACKSPACE;
        event.data2 = 0;
        event.data3 = 0;
        D_PostEvent(&event);
    }
    if (button_negedge[3]) {
        printf("BUTTON 3 UP\n");
        event_t event;
        event.type = ev_keyup;
        event.data1 = KEY_BACKSPACE;
        event.data2 = 0;
        event.data3 = 0;
        D_PostEvent(&event);
    }


    for (int i=0; i<4; i++) {
        button_prev_state[i] = button_state[i];
    }


}