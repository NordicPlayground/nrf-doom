

#include <stdint.h>

#define DISPLAY_PALETTE_SIZE (256*4)

// SPI
void N_display_spi_init();
void N_display_power_reset();
void N_display_spi_transfer_finish();
void N_display_spi_cmd(uint8_t b1, uint8_t b2);
void N_display_spi_wr8(uint32_t addr, uint8_t data);
void N_display_spi_wr16(uint32_t addr, uint16_t data);
void N_display_spi_wr32(uint32_t addr, uint32_t data);
void N_display_spi_wr(uint32_t addr, int dataSize, uint8_t *data);
uint8_t N_display_spi_rd8(uint32_t addr);

void N_display_init();
void N_display_wakeup();
uint32_t N_display_ram_alloc(size_t size);
void N_display_dlswap_frame();
void dl_start();
void dl(uint32_t cmd);
void dl_end();