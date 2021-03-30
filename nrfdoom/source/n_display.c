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

#include <stdbool.h>
#include <stdio.h>

#include "board_config.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"

#include "FT810.h"

#define BUF_MAXCNT 8

volatile int display_spi_tip; // transfer-in-progress
volatile uint8_t display_spi_txd_buf[BUF_MAXCNT];
volatile uint8_t display_spi_rxd_buf[BUF_MAXCNT];

void N_display_gpiote_end_to_cs() {
  const int TASK_MODE = 3;
  const int LO_TO_HI = 1;
  const int OUT_INIT_LOW = 0;

  // TODO: Confgigurable GPIOTE/DPPI channel
  NRF_DISPLAY_GPIOTE->CONFIG[0] = (TASK_MODE << 0) | (DISPLAY_PIN_CS_N << 8) | (LO_TO_HI << 16) | (OUT_INIT_LOW << 20);
  NRF_DISPLAY_GPIOTE->SUBSCRIBE_OUT[0] = 0 | GPIOTE_SUBSCRIBE_OUT_EN_Msk;
  NRF_DISPLAY_SPIM->PUBLISH_END = 0 | SPIM_PUBLISH_END_EN_Msk;
  NRF_DPPIC_S->CHENSET = 1;
}

void N_display_gpiote_clear() {
  NRF_DISPLAY_GPIOTE->CONFIG[0] = 0;
}

void N_display_spi_init() {

  // Set up GPIO pins
  nrf_gpio_cfg(DISPLAY_PIN_SCK, NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_H0H1, NRF_GPIO_PIN_NOSENSE);
  nrf_gpio_cfg(DISPLAY_PIN_MOSI,NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_H0H1, NRF_GPIO_PIN_NOSENSE);
  nrf_gpio_cfg(DISPLAY_PIN_MISO,NRF_GPIO_PIN_DIR_INPUT , NRF_GPIO_PIN_INPUT_CONNECT   , NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
  nrf_gpio_cfg(DISPLAY_PIN_CS_N,NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_H0H1, NRF_GPIO_PIN_NOSENSE);
  nrf_gpio_cfg(DISPLAY_PIN_PD_N,NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);

  nrf_gpio_pin_set(DISPLAY_PIN_CS_N);
  nrf_gpio_pin_set(DISPLAY_PIN_PD_N);

  // Configure SPI Master
  NRF_DISPLAY_SPIM->PSEL.SCK  = DISPLAY_PIN_SCK;
  NRF_DISPLAY_SPIM->PSEL.MOSI = DISPLAY_PIN_MOSI;
  NRF_DISPLAY_SPIM->PSEL.MISO = DISPLAY_PIN_MISO;
  //                  SCK pol    SCK phase  Bit Order (Msb First)
  NRF_DISPLAY_SPIM->CONFIG = (0 << 2) | (0 << 1) | (0 << 0);
  NRF_DISPLAY_SPIM->FREQUENCY = 0x01400000; // 0x14 = 32Mbps 0x0A = 16Mbps, 0x20.. = 2mbps, 0x08.. = 500kbps
  NRF_DISPLAY_SPIM->ORC = 0; // Over-Read Character

  NRF_DISPLAY_SPIM->ENABLE = 7;

  NRF_DISPLAY_SPIM->TXD.PTR = 0xFFFFFFFF;

  display_spi_tip = 0;

}

void N_display_power_reset() {
  nrf_gpio_pin_clear(DISPLAY_PIN_PD_N);
  nrf_delay_ms(50);
  nrf_gpio_pin_set(DISPLAY_PIN_PD_N);
  nrf_delay_ms(50);
}

void N_display_spi_setup(int txdMaxCnt, volatile uint8_t * txdPtr, 
                       int rxdMaxCnt, volatile uint8_t * rxdPtr) {

  NRF_DISPLAY_SPIM->TXD.MAXCNT = txdMaxCnt;
  NRF_DISPLAY_SPIM->TXD.PTR = (uint32_t)txdPtr;
  NRF_DISPLAY_SPIM->RXD.MAXCNT = rxdMaxCnt;
  NRF_DISPLAY_SPIM->RXD.PTR = (uint32_t)rxdPtr;
}

void N_display_spi_transfer_finish() {
  if (display_spi_tip) {
    while (NRF_DISPLAY_SPIM->EVENTS_END == 0) { 
    }
    NRF_DISPLAY_SPIM->EVENTS_END = 0;
    display_spi_tip = 0;
    N_display_gpiote_clear();
  }
}

void N_display_spi_transfer_start() {
  N_display_spi_transfer_finish();
  nrf_gpio_pin_clear(DISPLAY_PIN_CS_N);
}

void N_display_spi_transfer_data() {
  NRF_DISPLAY_SPIM->EVENTS_END = 0;
  NRF_DISPLAY_SPIM->TASKS_START = 1;
  while (NRF_DISPLAY_SPIM->EVENTS_END == 0) { 
  }
  NRF_DISPLAY_SPIM->EVENTS_END = 0;
}

void N_display_spi_transfer_data_end() {
  NRF_DISPLAY_SPIM->EVENTS_END = 0;

  N_display_gpiote_end_to_cs();

  NRF_DISPLAY_SPIM->TASKS_START = 1;
  display_spi_tip = 1;
}

void N_display_spi_transfer_end() {
  nrf_gpio_pin_set(DISPLAY_PIN_CS_N);
}

void N_display_spi_transfer() {
  N_display_spi_transfer_start();
  N_display_spi_transfer_data();
  N_display_spi_transfer_end();
}

void N_display_spi_cmd(uint8_t b1, uint8_t b2) {
  display_spi_txd_buf[0] = b1;
  display_spi_txd_buf[1] = b2;
  display_spi_txd_buf[2] = 0x00;

  N_display_spi_setup(3, display_spi_txd_buf, 0, NULL);
  N_display_spi_transfer();
}

void N_display_spi_wr8(uint32_t addr, uint8_t data) {
  uint8_t *addrBytes = (uint8_t*)&addr;

  // Assuming MCU is Little-Endian
  display_spi_txd_buf[0] = addrBytes[2] | 0x80;
  display_spi_txd_buf[1] = addrBytes[1];
  display_spi_txd_buf[2] = addrBytes[0];
  display_spi_txd_buf[3] = data;

  //printf("Wr8 %.2X %.2X %.2X %.2X\n", display_spi_txd_buf[0], display_spi_txd_buf[1], display_spi_txd_buf[2], display_spi_txd_buf[3]);

  N_display_spi_setup(4, display_spi_txd_buf, 0, NULL);
  N_display_spi_transfer();
}

void N_display_spi_wr16(uint32_t addr, uint16_t data) {
  uint8_t *addrBytes = (uint8_t*)&addr;
  uint8_t *dataBytes = (uint8_t*)&data;

  // Assuming MCU is Little-Endian
  display_spi_txd_buf[0] = addrBytes[2] | 0x80;
  display_spi_txd_buf[1] = addrBytes[1];
  display_spi_txd_buf[2] = addrBytes[0];
  display_spi_txd_buf[3] = dataBytes[0];
  display_spi_txd_buf[4] = dataBytes[1];

  //printf("Wr16 %.2X %.2X %.2X %.2X %.2X\n", display_spi_txd_buf[0], display_spi_txd_buf[1], display_spi_txd_buf[2], display_spi_txd_buf[3], display_spi_txd_buf[4]);

  N_display_spi_setup(5, display_spi_txd_buf, 0, NULL);
  N_display_spi_transfer();
}

void N_display_spi_wr32(uint32_t addr, uint32_t data) {
  uint8_t *addrBytes = (uint8_t*)&addr;
  uint8_t *dataBytes = (uint8_t*)&data;

  // Assuming MCU is Little-Endian
  display_spi_txd_buf[0] = addrBytes[2] | 0x80;
  display_spi_txd_buf[1] = addrBytes[1];
  display_spi_txd_buf[2] = addrBytes[0];

  display_spi_txd_buf[3] = dataBytes[0];
  display_spi_txd_buf[4] = dataBytes[1];
  display_spi_txd_buf[5] = dataBytes[2];
  display_spi_txd_buf[6] = dataBytes[3];

  N_display_spi_setup(7, display_spi_txd_buf, 0, NULL);
  N_display_spi_transfer();
}

void N_display_spi_wr(uint32_t addr, int dataSize, uint8_t *data) {
  uint8_t *addrBytes = (uint8_t*)&addr;

  // Assuming MCU is Little-Endian
  display_spi_txd_buf[0] = addrBytes[2] | 0x80;
  display_spi_txd_buf[1] = addrBytes[1];
  display_spi_txd_buf[2] = addrBytes[0];

  N_display_spi_transfer_start();

  N_display_spi_setup(3, display_spi_txd_buf, 0, NULL);
  N_display_spi_transfer_data();

  volatile uint8_t *dataPtr = (volatile uint8_t*)data;
  N_display_spi_setup(dataSize, dataPtr, 0, NULL);
  N_display_spi_transfer_data_end();

  // N_display_spi_transfer_end();
}

// void N_display_spi_transfer_data_async() {
//   NRF_DISPLAY_SPIM->EVENTS_END = 0;
//   NRF_DISPLAY_SPIM->TASKS_START = 1;
//   while (NRF_DISPLAY_SPIM->EVENTS_END == 0) { 
//   }
// }

// void N_display_spi_wr_async(uint32_t addr, int dataSize, uint8_t *data) {
//   uint8_t *addrBytes = (uint8_t*)&addr;

//   // Assuming MCU is Little-Endian
//   display_spi_txd_buf[0] = addrBytes[2] | 0x80;
//   display_spi_txd_buf[1] = addrBytes[1];
//   display_spi_txd_buf[2] = addrBytes[0];

//   N_display_spi_transfer_start();

//   N_display_spi_setup(3, display_spi_txd_buf, 0, NULL);
//   N_display_spi_transfer_data();

//   volatile uint8_t *dataPtr = (volatile uint8_t*)data;
//   N_display_spi_setup(dataSize, dataPtr, 0, NULL);
//   N_display_spi_transfer_data();

//   N_display_spi_transfer_end();
// }

uint8_t N_display_spi_rd8(uint32_t addr) {
  uint8_t *addrBytes = (uint8_t*)&addr;

  // Assuming MCU is Little-Endian
  display_spi_txd_buf[0] = addrBytes[2];
  display_spi_txd_buf[1] = addrBytes[1];
  display_spi_txd_buf[2] = addrBytes[0];
  display_spi_txd_buf[3] = 0x00;

  // printf("Rd %.2X %.2X %.2X %.2X\n", display_spi_txd_buf[0], display_spi_txd_buf[1], display_spi_txd_buf[2], display_spi_txd_buf[3]);

  N_display_spi_setup(4, display_spi_txd_buf, 5, display_spi_rxd_buf);
  N_display_spi_transfer();


  return display_spi_rxd_buf[4];
}

/// --------

uint32_t ram_free_loc = FT810_RAM_G;

void N_display_wakeup() {
  N_display_spi_cmd(0x00, 0x00);
}

void N_display_init() 
{

  N_display_spi_init();

  N_display_power_reset();
    
  N_display_wakeup();
  
  nrf_delay_ms(80);

  N_display_spi_cmd(FT810_CMD_CLKEXT, 0x00);

  nrf_delay_ms(80);

  printf("N_display_init - Display ID: %.2X\n", N_display_spi_rd8(FT810_REG_ID));
  printf("N_display_init - CPU Reset: %.2X\n", N_display_spi_rd8(FT810_REG_CPURESET));

  N_display_spi_wr16(FT810_REG_HSIZE,    800 ); // Active width of LCD display
  N_display_spi_wr16(FT810_REG_VSIZE,    480 ); // Active height of LCD display
  N_display_spi_wr16(FT810_REG_HCYCLE,   928 ); // Total number of clocks per line
  N_display_spi_wr16(FT810_REG_HOFFSET,  88  ); // Start of active line
  N_display_spi_wr16(FT810_REG_HSYNC0,   0   ); // Start of horizontal sync pulse
  N_display_spi_wr16(FT810_REG_HSYNC1,   48  ); // End of horizontal sync pulse
  N_display_spi_wr16(FT810_REG_VCYCLE,   525 ); // Total number of lines per screen
  N_display_spi_wr16(FT810_REG_VOFFSET,  32  ); // Start of active screen
  N_display_spi_wr16(FT810_REG_VSYNC0,   0   ); // Start of vertical sync pulse
  N_display_spi_wr16(FT810_REG_VSYNC1,   3   ); // End of vertical sync pulse
  N_display_spi_wr8 (FT810_REG_SWIZZLE,  0   ); // Define RGB output pins
  N_display_spi_wr8 (FT810_REG_PCLK_POL, 1   ); // Define active edge of PCLK
  
  uint8_t disGpio = N_display_spi_rd8(FT810_REG_GPIO);
  N_display_spi_wr8(FT810_REG_GPIO, disGpio | 0x80);

  N_display_spi_wr8 (FT810_REG_PWM_DUTY,  0xFF ); // Backlight PWM duty cycle

  N_display_spi_wr8 (FT810_REG_PCLK,      2   ); // Pixel Clock

  N_display_spi_wr8 (FT810_REG_ROTATE,  1   ); // inverted (up-down)

}

uint32_t N_display_ram_alloc(size_t size)
{
    uint32_t result = ram_free_loc;
    ram_free_loc += size;
    return result;
}

void N_display_dlswap_frame() {
  N_display_spi_wr32(FT810_REG_DLSWAP, FT810_DLSWAP_FRAME);  
}

uint32_t display_dli = 0;

void dl_start() {
  display_dli = FT810_RAM_DL;
}
void dl(uint32_t cmd) {
  N_display_spi_wr32(display_dli, cmd);
  display_dli += 4;
}
void dl_end() {
  N_display_spi_wr32(display_dli, FT810_DISPLAY());
}
