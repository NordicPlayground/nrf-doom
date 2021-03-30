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



#include "n_i2s.h"

#include <stdio.h>
#include "board_config.h"
#include <nrf.h>

#define NRF_I2S NRF_I2S0_S

#define BUFFER_SIZE 512
#define NUM_BUFFERS 5
#define SAMPLE_RATE 10870
#define ZERO_SAMPLE 0

typedef enum {
    BUF_EMPTY   = 0,
    BUF_FILLED  = 1,
    BUF_QUEUED  = 2,
    BUF_PLAYING = 3
} buffer_state_t;

boolean          i2s_started;

int16_t       sampleBuffers[NUM_BUFFERS][BUFFER_SIZE];
buffer_state_t bufferStates[NUM_BUFFERS];
int            queuedBuffer;

extern uint8_t pistol[];
extern int pistol_length;


static void zero_buffer(int bufIdx)
{
    for  (int j=0; j<BUFFER_SIZE; j++) {
        sampleBuffers[bufIdx][j] = ZERO_SAMPLE;
    }
}

static void clear_buffers()
{
    for (int i=0; i<NUM_BUFFERS; i++) {
        zero_buffer(i);
        bufferStates[i] = BUF_EMPTY;
    }
}

void N_I2S_init()
{
    printf("N_I2S_init\n");

    // Pull SCK down
    /* FIXME
    NRF_P1_S->PIN_CNF[I2S_PIN_SCK&0x1F] = 3;
    NRF_P1_S->OUT = NRF_P1_S->OUT & ~(1<<(I2S_PIN_SCK&0x1F));
    */

    NRF_I2S->PSEL.SCK   = I2S_PIN_BCK;
    NRF_I2S->PSEL.LRCK  = I2S_PIN_LRCK;
    NRF_I2S->PSEL.SDOUT = I2S_PIN_DIN;

    // fs = 44.1kHz
    // bck = 44.1kHz*16*2 = 1.442Mhz
    // bck 32x/1.442Mhz  - 64/2.8224Mhz  (48?)

    NRF_I2S->CONFIG.MODE = 0; // Master
    NRF_I2S->CONFIG.TXEN = 1;
    NRF_I2S->CONFIG.MCKEN = 1;
    NRF_I2S->CONFIG.MCKFREQ = 185319424; // 1391304Hz MCK
    NRF_I2S->CONFIG.RATIO = 4; // 1391304/128 = 10869.5Hz (11025kHz)
    NRF_I2S->CONFIG.SWIDTH = 1; // 16-bit sample width
    NRF_I2S->CONFIG.CHANNELS = 0; // Stereo (1=left only)
    NRF_I2S->CONFIG.CLKCONFIG = 0; // 0=32Mhz clock, 1=AudioPLL


    NRF_I2S->RXTXD.MAXCNT = BUFFER_SIZE/2;

    clear_buffers();
    queuedBuffer = 0;
    bufferStates[queuedBuffer] = BUF_QUEUED;

    NRF_I2S->EVENTS_TXPTRUPD = 0;
    // IRQ setup
    NVIC_SetPriority(I2S0_IRQn, 3);
    NVIC_EnableIRQ(I2S0_IRQn);
    NRF_I2S->INTEN = I2S_INTEN_TXPTRUPD_Msk;

    NRF_I2S->ENABLE = 1;

    i2s_started = false;

}

boolean N_I2S_next_buffer(int *buf_size, int16_t **buffer) {
    int bufIdx = queuedBuffer;
    while (1) {
        bufIdx = (bufIdx+1)%NUM_BUFFERS;
        switch (bufferStates[bufIdx]) {
            case BUF_EMPTY:
                // printf("Empty buffer at %d\n", bufIdx);
                *buf_size = BUFFER_SIZE;
                *buffer = sampleBuffers[bufIdx];
                bufferStates[bufIdx] = BUF_FILLED;
                return true;
            case BUF_FILLED:
                continue;
            case BUF_QUEUED:
                return false;
            case BUF_PLAYING:
                return false;
            default:
                // I_Error
                break;
        }
    }
}

static void N_I2S_queue_buffers()
{
    for (int i=0; i<NUM_BUFFERS; i++) {
        if (bufferStates[i] == BUF_PLAYING) {
            bufferStates[i] = BUF_EMPTY;
        }
    }

    bufferStates[queuedBuffer] = BUF_PLAYING;
    queuedBuffer = (queuedBuffer+1)%NUM_BUFFERS;
    if (bufferStates[queuedBuffer] == BUF_EMPTY) {
        zero_buffer(queuedBuffer);
    }
    bufferStates[queuedBuffer] = BUF_QUEUED;

    NRF_I2S->TXD.PTR = (uint32_t)(&sampleBuffers[queuedBuffer][0]);
    NRF_I2S->EVENTS_TXPTRUPD = 0;
}

void N_I2S_process()
{
    if (!i2s_started) {
        printf("N_I2S_process: Starting I2S..\n");
        N_I2S_queue_buffers();
        NRF_I2S->TASKS_START = 1;
        i2s_started = true;
    }
}


void I2S0_IRQHandler(void)
{
    if(NRF_I2S->EVENTS_TXPTRUPD  != 0)
    {
        N_I2S_queue_buffers();
    }
}

