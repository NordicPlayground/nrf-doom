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


#include <nrf.h>
#include <nrf_delay.h>

#include "hardfault.h"

__WEAK void HardFault_process(HardFault_stack_t * p_stack)
{
    nrf_delay_ms(5000);
    // Restart the system by default
    NVIC_SystemReset();
}

void HardFault_Print(const char *msg) {
    for (int i=0;i<256;i++) {
        char c = msg[i];
        if (!c) return;
        NRF_UART0->EVENTS_TXDRDY = 0;
        NRF_UART0->TXD = c;
        while (!NRF_UART0->EVENTS_TXDRDY) {}
    }
}

void intToHex(unsigned long n, char *outbuf)
{

    int i = 12;
    int j = 0;

    do{
        outbuf[i] = "0123456789ABCDEF"[n % 16];
        i--;
        n = n/16;
    } while( n > 0);

    while( ++i < 13){
       outbuf[j++] = outbuf[i];
    }

    outbuf[j] = 0;

}

void HardFault_Print_Hex(unsigned int value) {
    char msg[13];
    intToHex(value, msg);
    HardFault_Print(msg);
}


void HardFault_c_handler(uint32_t * p_stack_address)
{
    bsp_board_led_on(2);

    HardFault_Print("HardFault!!\n");

#ifndef CFSR_MMARVALID
  #define CFSR_MMARVALID (1 << (0 + 7))
#endif

#ifndef CFSR_BFARVALID
  #define CFSR_BFARVALID (1 << (8 + 7))
#endif

    static const char *cfsr_msgs[] = {
        [0]  = "The processor has attempted to execute an undefined instruction",
        [1]  = "The processor attempted a load or store at a location that does not permit the operation",
        [2]  = NULL,
        [3]  = "Unstack for an exception return has caused one or more access violations",
        [4]  = "Stacking for an exception entry has caused one or more access violations",
        [5]  = "A MemManage fault occurred during floating-point lazy state preservation",
        [6]  = NULL,
        [7]  = NULL,
        [8]  = "Instruction bus error",
        [9]  = "Data bus error (PC value stacked for the exception return points to the instruction that caused the fault)",
        [10] = "Data bus error (return address in the stack frame is not related to the instruction that caused the error)",
        [11] = "Unstack for an exception return has caused one or more BusFaults",
        [12] = "Stacking for an exception entry has caused one or more BusFaults",
        [13] = "A bus fault occurred during floating-point lazy state preservation",
        [14] = NULL,
        [15] = NULL,
        [16] = "The processor has attempted to execute an undefined instruction",
        [17] = "The processor has attempted to execute an instruction that makes illegal use of the EPSR",
        [18] = "The processor has attempted an illegal load of EXC_RETURN to the PC, as a result of an invalid context, or an invalid EXC_RETURN value",
        [19] = "The processor has attempted to access a coprocessor",
        [20] = NULL,
        [21] = NULL,
        [22] = NULL,
        [23] = NULL,
        [24] = "The processor has made an unaligned memory access",
        [25] = "The processor has executed an SDIV or UDIV instruction with a divisor of 0",
    };

    uint32_t cfsr = SCB->CFSR;

    if (SCB->HFSR & SCB_HFSR_VECTTBL_Msk)
    {
        HardFault_Print("Cause: BusFault on a vector table read during exception processing.\n");
    }

    for (uint32_t i = 0; i < sizeof(cfsr_msgs) / sizeof(cfsr_msgs[0]); i++)
    {
        if (((cfsr & (1 << i)) != 0) && (cfsr_msgs[i] != NULL))
        {
            printf("Cause: %s.\n", (char*)cfsr_msgs[i]);
        }
    }

    if (cfsr & CFSR_MMARVALID)
    {
        HardFault_Print("MemManage Fault Address:");
    }

    if (cfsr & CFSR_BFARVALID)
    {
        HardFault_Print("Bus Fault Address:");
        HardFault_Print_Hex((unsigned int)SCB->BFAR);
    }

/*
#ifndef CFSR_MMARVALID
  #define CFSR_MMARVALID (1 << (0 + 7))
#endif

#ifndef CFSR_BFARVALID
  #define CFSR_BFARVALID (1 << (8 + 7))
#endif

    HardFault_stack_t * p_stack = (HardFault_stack_t *)p_stack_address;
    static const char *cfsr_msgs[] = {
        [0]  = "The processor has attempted to execute an undefined instruction",
        [1]  = "The processor attempted a load or store at a location that does not permit the operation",
        [2]  = NULL,
        [3]  = "Unstack for an exception return has caused one or more access violations",
        [4]  = "Stacking for an exception entry has caused one or more access violations",
        [5]  = "A MemManage fault occurred during floating-point lazy state preservation",
        [6]  = NULL,
        [7]  = NULL,
        [8]  = "Instruction bus error",
        [9]  = "Data bus error (PC value stacked for the exception return points to the instruction that caused the fault)",
        [10] = "Data bus error (return address in the stack frame is not related to the instruction that caused the error)",
        [11] = "Unstack for an exception return has caused one or more BusFaults",
        [12] = "Stacking for an exception entry has caused one or more BusFaults",
        [13] = "A bus fault occurred during floating-point lazy state preservation",
        [14] = NULL,
        [15] = NULL,
        [16] = "The processor has attempted to execute an undefined instruction",
        [17] = "The processor has attempted to execute an instruction that makes illegal use of the EPSR",
        [18] = "The processor has attempted an illegal load of EXC_RETURN to the PC, as a result of an invalid context, or an invalid EXC_RETURN value",
        [19] = "The processor has attempted to access a coprocessor",
        [20] = NULL,
        [21] = NULL,
        [22] = NULL,
        [23] = NULL,
        [24] = "The processor has made an unaligned memory access",
        [25] = "The processor has executed an SDIV or UDIV instruction with a divisor of 0",
    };

    uint32_t cfsr = SCB->CFSR;
    
    if (p_stack != NULL)
    {
        // Print information about error.
        printf("HARD FAULT at 0x%08X\n", (unsigned int)p_stack->pc);
        printf("  R0:  0x%08X  R1:  0x%08X  R2:  0x%08X  R3:  0x%08X\n",
                             (unsigned int)p_stack->r0, (unsigned int)p_stack->r1, (unsigned int)p_stack->r2, (unsigned int)p_stack->r3);
        printf("  R12: 0x%08X  LR:  0x%08X  PSR: 0x%08X\n",
                             (unsigned int)p_stack->r12, (unsigned int)p_stack->lr, (unsigned int)p_stack->psr);
    }
    else
    {
        printf("Stack violation: stack pointer outside stack area.\n");
    }

    if (SCB->HFSR & SCB_HFSR_VECTTBL_Msk)
    {
        printf("Cause: BusFault on a vector table read during exception processing.\n");
    }

    for (uint32_t i = 0; i < sizeof(cfsr_msgs) / sizeof(cfsr_msgs[0]); i++)
    {
        if (((cfsr & (1 << i)) != 0) && (cfsr_msgs[i] != NULL))
        {
            printf("Cause: %s.\n", (char*)cfsr_msgs[i]);
        }
    }

    if (cfsr & CFSR_MMARVALID)
    {
        printf("MemManage Fault Address: 0x%08X\n", (unsigned int)SCB->MMFAR);
    }

    if (cfsr & CFSR_BFARVALID)
    {
        printf("Bus Fault Address: 0x%08X\n", (unsigned int)SCB->BFAR);
    }
    */

    HardFault_process((HardFault_stack_t *)p_stack_address);
    while(1) {}
}
