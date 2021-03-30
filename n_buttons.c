
#include "nrf.h"
#include "nrf_gpio.h"
#include "board_config.h"

#undef PACKED_STRUCT

#include "doomkeys.h"
#include "d_event.h"
#include "i_system.h"

boolean button_prev_state[4];

char button_map[] = {KEY_UPARROW, KEY_DOWNARROW, KEY_ENTER, KEY_ESCAPE};

void N_ButtonsInit()
{
    nrf_gpio_cfg_input(BUTTON_PIN_1, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON_PIN_2, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON_PIN_3, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON_PIN_4, NRF_GPIO_PIN_PULLUP);
}

int N_ButtonStateRaw(int id)
{
    switch(id) {
        case 0:
        return (NRF_P0_S->IN  & (1 << BUTTON_PIN_1)) == 0;
        case 1:
        return (NRF_P0_S->IN  & (1 << BUTTON_PIN_2)) == 0;
        case 2:
        return 0; //(NRF_P0_S->IN  & (1 << BUTTON_PIN_3)) == 0;
        case 3:
        return 0; //(NRF_P0_S->IN  & (1 << BUTTON_PIN_4)) == 0;
    }
    I_Error("N_ButtonStateRaw: Invalid button\n");
    return 0;
}

void N_ReadButtons() 
{
    
    static event_t event;

    boolean button_state[4];
    boolean button_posedge[4];
    boolean button_negedge[4];
    for (int i=0; i<4; i++) {
        button_state[i] = N_ButtonStateRaw(i);
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