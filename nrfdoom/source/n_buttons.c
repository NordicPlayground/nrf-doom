/*
 * Copyright (c) 2019 - 2020, Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#include <stdio.h>

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
    button_prev_state[0] = 0;
    button_prev_state[1] = 0;
    button_prev_state[2] = 0;
    button_prev_state[3] = 0;
}

int N_ButtonStateRaw(int id)
{
    switch(id) {
        case 0:
        return !nrf_gpio_pin_read(BUTTON_PIN_1);
        case 1:
        return !nrf_gpio_pin_read(BUTTON_PIN_2);
        case 2:
        return !nrf_gpio_pin_read(BUTTON_PIN_3);
        case 3:
        return !nrf_gpio_pin_read(BUTTON_PIN_4);
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