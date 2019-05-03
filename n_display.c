#include <stdbool.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"

#include "FT810.h"

#define BUF_MAXCNT 256

volatile uint8_t display_spi_txd_buf[BUF_MAXCNT];
volatile uint8_t display_spi_rxd_buf[BUF_MAXCNT];


const int PIN_SCK  = 3;
const int PIN_MISO = 4;
const int PIN_MOSI = 28;
const int PIN_CS_N = 29;
const int PIN_INT  = 30;
const int PIN_PD_N = 31;


void pin_set(int pin) {
  NRF_GPIO->OUTSET = (1<<pin);
}

void pin_clr(int pin) {
  NRF_GPIO->OUTCLR = (1<<pin);
}

void pin_tgl(int pin) {
  NRF_GPIO->OUT ^= (1<<pin);
}

void N_display_spi_init() {

  // Set up GPIO pins
  //                            High drv.  Inp/Out
  NRF_GPIO->PIN_CNF[PIN_SCK] =  (3 << 8)   | 3;
  NRF_GPIO->PIN_CNF[PIN_MOSI] = (3 << 8)   | 3;
  NRF_GPIO->PIN_CNF[PIN_MISO] = 0; 
  NRF_GPIO->PIN_CNF[PIN_CS_N] = (3 << 8)   | 3;
  NRF_GPIO->PIN_CNF[PIN_PD_N] = 3;
  
  pin_set(PIN_CS_N);
  pin_set(PIN_PD_N);

  // Configure SPI Master
  NRF_SPIM3->PSEL.SCK = PIN_SCK;
  NRF_SPIM3->PSEL.MOSI = PIN_MOSI;
  NRF_SPIM3->PSEL.MISO = PIN_MISO;
  //                  SCK pol    SCK phase  Bit Order (Msb First)
  NRF_SPIM3->CONFIG = (0 << 2) | (0 << 1) | (0 << 0);
  NRF_SPIM3->FREQUENCY = 0x14000000; // 0x14 = 32Mbps 0x20.. = 2mbps, 0x08.. = 500kbps
  NRF_SPIM3->ORC = 0; // Over-Read Character

  NRF_SPIM3->ENABLE = 7;

  NRF_SPIM3->TXD.PTR = 0xFFFFFFFF;

}

void N_display_power_reset() {
  pin_clr(PIN_PD_N);
  nrf_delay_ms(50);
  pin_set(PIN_PD_N);
  nrf_delay_ms(50);
}

void N_display_spi_setup(int txdMaxCnt, volatile uint8_t * txdPtr, 
                       int rxdMaxCnt, volatile uint8_t * rxdPtr) {

  NRF_SPIM3->TXD.MAXCNT = txdMaxCnt;
  NRF_SPIM3->TXD.PTR = (uint32_t)txdPtr;
  NRF_SPIM3->RXD.MAXCNT = rxdMaxCnt;
  NRF_SPIM3->RXD.PTR = (uint32_t)rxdPtr;
}

void N_display_spi_transfer_start() {
  pin_clr(PIN_CS_N);
}

void N_display_spi_transfer_data() {
  NRF_SPIM3->EVENTS_END = 0;
  NRF_SPIM3->TASKS_START = 1;
  while (NRF_SPIM3->EVENTS_END == 0) { 
  }
}

void N_display_spi_transfer_end() {
  pin_set(PIN_CS_N);
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

/*
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
  int restSize = dataSize;
  while (restSize > 0) {
    int txSize = restSize;
    if (txSize > 65535) txSize = 65535; // TODO: Update with Yoda
    N_display_spi_setup(txSize, dataPtr, 0, NULL);
    N_display_spi_transfer_data();
    dataPtr += txSize;
    restSize -= txSize;
  }

  N_display_spi_transfer_end();
}
*/

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
  N_display_spi_transfer_data();

  N_display_spi_transfer_end();
}



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
