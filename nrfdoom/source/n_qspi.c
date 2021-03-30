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

#include "n_qspi.h"

#include <stdio.h>

#include <board_config.h>

#include <nrf.h>
#include <nrf_gpio.h>
#include <nrfx_qspi.h>
#include <nordic_common.h>
#include <app_error.h>

void I_Error (char *error, ...);

#define QSPI_STD_CMD_WRSR        0x01
#define QSPI_STD_CMD_RDSR        0x05
#define QSPI_STD_CMD_RDCR        0x15
#define QSPI_STD_CMD_RSTEN       0x66
#define QSPI_STD_CMD_RST         0x99
#define QSPI_STD_CMD_RDID        0x9F
#define QSPI_STD_CMD_CE          0x60

#define QSPI_SR_QUAD_ENABLE_BYTE 0x40

static volatile bool m_finished = false;

size_t qspi_next_loc;

void N_qspi_wait() {
    while (!m_finished) {}
    m_finished = false; 
}

static void qspi_handler(nrfx_qspi_evt_t event, void * p_context)
{
    UNUSED_PARAMETER(event);
    UNUSED_PARAMETER(p_context);
    m_finished = true;
}

static void configure_memory(void)
{
    uint32_t err_code;
    uint8_t rxdata[4];
    uint8_t data[4];

    nrf_qspi_cinstr_conf_t cinstr_cfg = {
        .opcode    = QSPI_STD_CMD_RSTEN,
        .length    = NRF_QSPI_CINSTR_LEN_1B,
        .io2_level = true,
        .io3_level = true,
        .wipwait   = true,
        .wren      = true
    };


    // Send reset enable
    printf("N_qspi: Send reset enable\n");
    err_code = nrfx_qspi_cinstr_xfer(&cinstr_cfg, NULL, NULL);
    APP_ERROR_CHECK(err_code);

    // Send reset command
    printf("N_qspi: Send reset command\n");
    cinstr_cfg.opcode = QSPI_STD_CMD_RST;
    err_code = nrfx_qspi_cinstr_xfer(&cinstr_cfg, NULL, NULL);
    APP_ERROR_CHECK(err_code);

    printf("N_qspi: Read ID\n");
    cinstr_cfg.opcode = QSPI_STD_CMD_RDID;
    cinstr_cfg.length = NRF_QSPI_CINSTR_LEN_3B;
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
    err_code = nrfx_qspi_cinstr_xfer(&cinstr_cfg, data, rxdata);
    APP_ERROR_CHECK(err_code);
    printf("N_qspi:   %x %x %x\n", rxdata[0], rxdata[1], rxdata[2]);


    printf("N_qspi: Read status\n");
    cinstr_cfg.opcode = QSPI_STD_CMD_RDSR;
    cinstr_cfg.length = NRF_QSPI_CINSTR_LEN_2B;
    data[0] = 0;
    err_code = nrfx_qspi_cinstr_xfer(&cinstr_cfg, data, rxdata);
    APP_ERROR_CHECK(err_code);
    printf("N_qspi:   %x\n", rxdata[0]);


    printf("N_qspi: Read Conf\n");
    cinstr_cfg.opcode = QSPI_STD_CMD_RDCR;
    cinstr_cfg.length = NRF_QSPI_CINSTR_LEN_3B;
    data[0] = 0;
    data[2] = 0;
    err_code = nrfx_qspi_cinstr_xfer(&cinstr_cfg, data, rxdata);
    APP_ERROR_CHECK(err_code);
    printf("N_qspi:   %x %x\n", rxdata[0], rxdata[1]);

    /*
    // Send reset enable
    err_code = nrfx_qspi_cinstr_xfer(&cinstr_cfg, NULL, NULL);
    APP_ERROR_CHECK(err_code);

    // Send reset command
    cinstr_cfg.opcode = QSPI_STD_CMD_RST;
    err_code = nrfx_qspi_cinstr_xfer(&cinstr_cfg, NULL, NULL);
    APP_ERROR_CHECK(err_code);

    // Switch to qspi and high-performance mode
    uint8_t data[3];
    data[0] = 0x40; // set bit 6 for Quad mode
    data[1] = 0x00;
    data[2] = 0x02; // set bit 2 for high-performance mode
    cinstr_cfg.opcode = QSPI_STD_CMD_WRSR;
    cinstr_cfg.length = NRF_QSPI_CINSTR_LEN_4B;
    err_code = nrfx_qspi_cinstr_xfer(&cinstr_cfg, data, NULL);
    APP_ERROR_CHECK(err_code);
*/
    // Switch to qspi mode
    /*
    data = QSPI_SR_QUAD_ENABLE_BYTE;
    cinstr_cfg.opcode = QSPI_STD_CMD_WRSR;
    cinstr_cfg.length = NRF_QSPI_CINSTR_LEN_2B;
    err_code = nrfx_qspi_cinstr_xfer(&cinstr_cfg, &data, NULL);
    APP_ERROR_CHECK(err_code);
    */

    while (nrfx_qspi_mem_busy_check())
    {}
}

void N_qspi_init() {

    uint32_t err_code = NRF_SUCCESS;

    // Set high-drive for QuadSpi pins
    nrf_gpio_cfg(QSPI_SCK_PIN, NRF_GPIO_PIN_DIR_INPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_H0H1, NRF_GPIO_PIN_NOSENSE);
    nrf_gpio_cfg(QSPI_CSN_PIN, NRF_GPIO_PIN_DIR_INPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_H0H1, NRF_GPIO_PIN_NOSENSE);
    nrf_gpio_cfg(QSPI_IO0_PIN, NRF_GPIO_PIN_DIR_INPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_H0H1, NRF_GPIO_PIN_NOSENSE);
    nrf_gpio_cfg(QSPI_IO1_PIN, NRF_GPIO_PIN_DIR_INPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_H0H1, NRF_GPIO_PIN_NOSENSE);
    nrf_gpio_cfg(QSPI_IO2_PIN, NRF_GPIO_PIN_DIR_INPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_H0H1, NRF_GPIO_PIN_NOSENSE);
    nrf_gpio_cfg(QSPI_IO3_PIN, NRF_GPIO_PIN_DIR_INPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_H0H1, NRF_GPIO_PIN_NOSENSE);

    // Set QSPI peripheral with default configuration.
    nrfx_qspi_config_t config = NRFX_QSPI_DEFAULT_CONFIG(
                                     QSPI_SCK_PIN, // _pin_sck
                                     QSPI_CSN_PIN, // _pin_csn
                                     QSPI_IO0_PIN, // _pin_io0
                                     QSPI_IO1_PIN, // _pin_io1
                                     QSPI_IO2_PIN, // _pin_io2
                                     QSPI_IO3_PIN  // _pin_io3
                                    );

    // Set SCK frequency to max
    config.prot_if.readoc     = (nrf_qspi_readoc_t)NRFX_QSPI_CONFIG_READOC;
    config.prot_if.writeoc    = (nrf_qspi_writeoc_t)NRFX_QSPI_CONFIG_WRITEOC;
    config.prot_if.addrmode   = (nrf_qspi_addrmode_t)NRFX_QSPI_CONFIG_ADDRMODE;
    config.prot_if.dpmconfig  = false;
    config.phy_if.sck_delay    = NRFX_QSPI_CONFIG_SCK_DELAY;
    config.phy_if.dpmen       = false;
    config.phy_if.spi_mode    = (nrf_qspi_spi_mode_t)NRFX_QSPI_CONFIG_MODE;
    config.phy_if.sck_freq    = (nrf_qspi_frequency_t)NRFX_QSPI_CONFIG_FREQUENCY;

    // Try to initialize QSPI peripheral in blocking mode.
    err_code = nrfx_qspi_init(&config, qspi_handler, NULL);
    APP_ERROR_CHECK(err_code);

    // Set RXDELAY to 1
    // TODO: Use define if MDK is updated to include it
    NRF_QSPI_S->IFTIMING = (1 << 8); 

    printf("QSPI initialized\n");
    printf("QSPI initialized %ld %ld\n", (NRF_QSPI_S->IFTIMING>>8), ((NRF_QSPI_S->IFCONFIG1>>28)&0xF));

    // Restart and configure external memory to use quad line mode for data exchange.
    configure_memory();
    printf("Memory device configured. Quad Mode activated.\n");

    qspi_next_loc = 0;
}

void N_qspi_erase_block(size_t loc) 
{
    uint32_t err_code = NRF_SUCCESS;

    err_code = nrfx_qspi_erase(NRF_QSPI_ERASE_LEN_64KB, loc);
    if (err_code != NRF_SUCCESS) I_Error("N_qspi_write_block: nrfx_qspi_erase fail");
    N_qspi_wait();
    printf("N_qspi: Erased 64KB block at %d\n", loc);
}

void N_qspi_write(size_t loc, void *buffer, size_t size) 
{
    uint32_t err_code = NRF_SUCCESS;
    err_code = nrfx_qspi_write(buffer, size, loc);
    if (err_code != NRF_SUCCESS) I_Error("N_qspi_write_block: nrfx_qspi_write fail");
    N_qspi_wait();
    printf("N_qspi: Wrote %d byte chunk at %d\n", size, loc);    
}


void N_qspi_write_block(size_t loc, void *buffer, size_t size) 
{
    uint32_t err_code = NRF_SUCCESS;

    if (size > N_QSPI_BLOCK_SIZE) {
        I_Error("N_qspi_write_block: Tried to write block > 64KB\n");
    }

    err_code = nrfx_qspi_erase(NRF_QSPI_ERASE_LEN_64KB, loc);
    if (err_code != NRF_SUCCESS) I_Error("N_qspi_write_block: nrfx_qspi_erase fail");
    N_qspi_wait();
    printf("N_qspi: Erased 64KB block at %d\n", loc);

    err_code = nrfx_qspi_write(buffer, size, loc);
    if (err_code != NRF_SUCCESS) I_Error("N_qspi_write_block: nrfx_qspi_write fail");
    N_qspi_wait();
    printf("N_qspi: Wrote %d byte chunk at %d\n", size, loc);

}

void *N_qspi_data_pointer(size_t loc)
{
    return (void*)(N_QSPI_XIP_START_ADDR+loc);
}

void N_qspi_reserve_blocks(size_t block_count)
{
    qspi_next_loc += block_count*N_QSPI_BLOCK_SIZE;
}

size_t N_qspi_alloc_block()
{
    size_t loc = qspi_next_loc;
    qspi_next_loc += N_QSPI_BLOCK_SIZE;
    return loc;
}