/**
 * Copyright (c) 2016 - 2019, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdio.h>

#include "nrf.h"

#include "FT810.h"
#include "n_display.h"

#include "n_uart.h"
#include "n_qspi.h"
#include "n_fs.h"
#include "n_buttons.h"
#include "n_rjoy.h"
#include "n_i2s.h"

#include "nrf_delay.h"
#include "nrf_clock.h"
#include "nrf_gpio.h"
#include "nrfx_clock.h"

#include "board_config.h"

int no_sdcard = 1;

void D_DoomMain (void);
void M_ArgvInit(void);

/*
void MWU_IRQHandler(void)
{
    printf("!!!!!!Stack or Heap Overflow!!!!!!: %d %d %d %d\n", 
            (int)NRF_MWU->EVENTS_REGION[0].WA, 
            (int)NRF_MWU->EVENTS_REGION[0].RA, 
            (int)NRF_MWU->EVENTS_REGION[1].WA, 
            (int)NRF_MWU->EVENTS_REGION[1].RA);
    NRF_MWU->EVENTS_REGION[0].WA = 0;
    NRF_MWU->EVENTS_REGION[0].RA = 0;
    NRF_MWU->EVENTS_REGION[1].WA = 0;
    NRF_MWU->EVENTS_REGION[1].RA = 0;
}

*/

void mwu_init()
{
    /*
    NRF_MWU->REGION[0].START = 0x2003f000;
    NRF_MWU->REGION[0].END   = 0x2003f100;
    NRF_MWU->REGIONENSET = (1 << MWU_REGIONENSET_RGN0RA_Pos) | (1 << MWU_REGIONENSET_RGN0WA_Pos);
    NRF_MWU->INTENSET = (1<<0) | (1<<1);
    // NRF_MWU->NMIENSET = (1<<0) | (1<<1);

    NRF_MWU->REGION[1].START = 0x2003f100;
    NRF_MWU->REGION[1].END   = 0x2003f200;
    NRF_MWU->REGIONENSET = (1 << MWU_REGIONENSET_RGN1RA_Pos) | (1 << MWU_REGIONENSET_RGN1WA_Pos);
    NRF_MWU->INTENSET = (1<<2) | (1<<3);

    NRFX_IRQ_ENABLE(nrfx_get_irq_number(NRF_MWU));
    */
}

void app_error_handler_bare(uint32_t code)
{
    printf("nRF app_error\n");
}


void clock_initialization()
{
    nrfx_clock_hfclk_start();
    nrf_clock_hfclk_div_set(NRF_CLOCK_S, NRF_CLOCK_HFCLK_DIV_1);


}

void boot_net()
{
    printf("Booting NetMCU\n");

    // Network owns 30/31 (LED3/4)
    nrf_gpio_pin_mcu_select(LED_PIN_3, NRF_GPIO_PIN_MCUSEL_NETWORK);
    nrf_gpio_pin_mcu_select(LED_PIN_4, NRF_GPIO_PIN_MCUSEL_NETWORK);

    // Hand over UART GPIOs to NetMcu
    nrf_gpio_pin_mcu_select(LED_PIN_3, NRF_GPIO_PIN_MCUSEL_NETWORK);
    nrf_gpio_pin_mcu_select(LED_PIN_4, NRF_GPIO_PIN_MCUSEL_NETWORK);

    // Set NetMcu as secure
    NRF_SPU_S->EXTDOMAIN[0].PERM = 2 | (1<<4);

    // Wake up NetMcu
    NRF_RESET_S->NETWORK.FORCEOFF = 0;
}

int main(void)
{
    clock_initialization();

    N_uart_init();

    printf("\n\n");
    printf("----------------------------------\n");
    printf("UART Initialized\n");
    printf("---------------------------------\n");

    NRF_CACHE_S->ENABLE = 1;

    boot_net();

    N_qspi_init();

    if (!no_sdcard) {
        N_fs_init();
    }

    N_ButtonsInit();

    N_I2S_init();

    M_ArgvInit();

    D_DoomMain();

    while (true)
    {
        __WFE();
    }
}

void exit(int code) {
    printf("exit\n");
    while (true)
    {
        __WFE();
    }
}
