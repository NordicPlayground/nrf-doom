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
#include <stdint.h>
#include <stdbool.h>

#include <nrf.h>

#include "mcu_config.h"

#include "uart.h"

#define RX_PIN_NUMBER  22
#define TX_PIN_NUMBER  20

volatile char uart_tx_buf[32];
volatile char uart_rx_buf[32];

bool uart_has_mutex = false;

void uart_mutex_grab() {
  while (NRF_MUTEX->MUTEX[0]){
  }
  uart_has_mutex = true;
}

void uart_mutex_release() {
  uart_has_mutex = false;
  NRF_MUTEX->MUTEX[0] = 0;
}

void uart_init(void)
{
    NRF_UARTE->PSEL.TXD = TX_PIN_NUMBER;
    NRF_UARTE->PSEL.RXD = RX_PIN_NUMBER;
    NRF_UARTE->BAUDRATE = UARTE_BAUDRATE_BAUDRATE_Baud115200;
    NRF_UARTE->CONFIG = 0;
    NRF_UARTE->ENABLE = UARTE_ENABLE_ENABLE_Enabled;
    // NRF_UARTE->TASKS_STARTTX = 1; 
    // NRF_UARTE->TASKS_STARTRX = 1; 
    NRF_UARTE->EVENTS_RXDRDY = 0;
    NRF_UARTE->EVENTS_TXDRDY = 0;

}


void uart_putc(char ch)
{
    NRF_UARTE->EVENTS_TXDRDY = 0;
    NRF_UARTE->EVENTS_ENDTX = 0;

    uart_tx_buf[0] = ch;
    NRF_UARTE->TXD.PTR = (uint32_t)(&uart_tx_buf[0]);
    NRF_UARTE->TXD.MAXCNT = 1;

    NRF_UARTE->TASKS_STARTTX = 1;

    while (!NRF_UARTE->EVENTS_ENDTX) {}

}

bool uart_has_data() {
    return NRF_UARTE->EVENTS_RXDRDY;
}


//  Retargeting (for printf etc)
// ------------------------------------

int __getchar(FILE * p_file)
{
    uint8_t input = 0;
    // TODO?
    return input;
}

int __putchar(int ch, FILE * p_file)
{
    // UNUSED_PARAMETER(p_file);
    uart_putc(ch);
    return ch;
}

int _write(int file, const char * p_char, int len)
{
    int i;

    // UNUSED_PARAMETER(file);
    for (i = 0; i < len; i++)
    {
        uart_putc(*p_char++);
    }
    return len;
}

int _read(int file, char * p_char, int len)
{
    // UNUSED_PARAMETER(file);
    // TODO?
    return 1;
}