//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//      Timer functions.
//

#include "i_timer.h"
#include "doomtype.h"

#undef PACKED_STRUCT
#include "nrf_delay.h"

//
// I_GetTime
// returns time in 1/35th second tics
//

//static Uint32 basetime = 0;

// NRFD-TODO: Handle overflow of timer

int  I_GetTime (void)
{
    NRF_TIMER0->TASKS_CAPTURE[0] = 1;
    uint32_t cc = NRF_TIMER0->CC[0];
    return (cc * TICRATE)*10/625/1000; 
}

//
// Same as I_GetTime, but returns time in milliseconds
//

int I_GetTimeMS(void)
{
    NRF_TIMER0->TASKS_CAPTURE[0] = 1;
    uint32_t cc = NRF_TIMER0->CC[0];
    cc = cc*10/625;
    return cc;
}

// Sleep for a specified number of ms

void I_Sleep(int ms)
{
    nrf_delay_ms(ms);
}

void I_WaitVBL(int count)
{
    I_Sleep((count * 1000) / 70);
}


void I_InitTimer(void)
{
    // initialize timer
    NRF_TIMER0->MODE = TIMER_MODE_MODE_Timer;
    NRF_TIMER0->BITMODE = TIMER_BITMODE_BITMODE_32Bit;
    NRF_TIMER0->PRESCALER = 8;
    // fTIMER = 16 MHz / (2*PRESCALER)
    // 2**8 = 256
    // fTIMER = 62.5Khz;
    NRF_TIMER0->TASKS_START = 1;
}

