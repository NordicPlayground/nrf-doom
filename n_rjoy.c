
// Implement SNES mini based controller (Nintendo Switch Wiimote Extension protocol)

#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "board_config.h"

#undef PACKED_STRUCT

#include "doomkeys.h"
#include "d_event.h"
#include "i_system.h"

const int x_cen = 112;
const int y_cen = 112;
const int guard = 4;

extern short st_faceindex;

typedef struct {
    uint8_t counter;
    uint8_t buttons;
    uint8_t joyX;
    uint8_t joyY;
} rjoy_radio_packet_t;

rjoy_radio_packet_t prev_joy_state;

int N_rjoy_init() {


    return 1;
}


void N_rjoy_read() {
    volatile uint32_t *ipc_ptr = &NRF_IPC_S->GPMEM[0];
    uint32_t radio_packet = *ipc_ptr;
    rjoy_radio_packet_t new_joy_state;
    uint32_t *tmp = (uint32_t*)(&new_joy_state);
    *tmp = radio_packet;
    // printf("%lx\n", *tmp);
    if (new_joy_state.counter != prev_joy_state.counter) {
        // printf("N_rjoy_read: %d\n", new_joy_state.counter);
        event_t ev;

        int joyX = new_joy_state.joyX-x_cen;
        int joyY = new_joy_state.joyY-y_cen;

        if (-guard < joyX && joyX < guard) joyX = 0;
        if (-guard < joyY && joyY < guard) joyY = 0;

        // printf("N_rjoy_read: %d %d\n", joyX, joyY);

        ev.type = ev_joystick;
        ev.data1 = new_joy_state.buttons;
        ev.data2 = joyY;
        ev.data3 = -joyX;
        ev.data4 = 0;
        ev.data5 = 0;

        D_PostEvent(&ev);
    }
    prev_joy_state = new_joy_state;

    // Provide current face to radio for transfer to gamepade
    volatile uint32_t *ipc_ptr_1 = &NRF_IPC_S->GPMEM[1];
    *ipc_ptr_1 = st_faceindex;
}

