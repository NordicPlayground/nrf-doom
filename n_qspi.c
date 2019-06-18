
#include "n_qspi.h"

#include "nrf.h"
#include "bsp.h"
#include <nrfx_qspi.h>

void I_Error (char *error, ...);

#define QSPI_STD_CMD_WRSR        0x01
#define QSPI_STD_CMD_RSTEN       0x66
#define QSPI_STD_CMD_RST         0x99

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
    uint8_t data;
    nrf_qspi_cinstr_conf_t cinstr_cfg = {
        .opcode    = QSPI_STD_CMD_RSTEN,
        .length    = NRF_QSPI_CINSTR_LEN_1B,
        .io2_level = true,
        .io3_level = true,
        .wipwait   = true,
        .wren      = true
    };

    // Send reset enable
    err_code = nrfx_qspi_cinstr_xfer(&cinstr_cfg, NULL, NULL);
    APP_ERROR_CHECK(err_code);

    // Send reset command
    cinstr_cfg.opcode = QSPI_STD_CMD_RST;
    err_code = nrfx_qspi_cinstr_xfer(&cinstr_cfg, NULL, NULL);
    APP_ERROR_CHECK(err_code);

    // Switch to qspi mode
    data = QSPI_SR_QUAD_ENABLE_BYTE;
    cinstr_cfg.opcode = QSPI_STD_CMD_WRSR;
    cinstr_cfg.length = NRF_QSPI_CINSTR_LEN_2B;
    err_code = nrfx_qspi_cinstr_xfer(&cinstr_cfg, &data, NULL);
    APP_ERROR_CHECK(err_code);

    while (nrfx_qspi_mem_busy_check())
    {}
}

void N_qspi_init() {

    uint32_t err_code = NRF_SUCCESS;

    // Set QSPI peripheral with default configuration.
    nrfx_qspi_config_t config = NRFX_QSPI_DEFAULT_CONFIG;

    // Set QSPI pins to pins related to connected board.
    config.pins.sck_pin = BSP_QSPI_SCK_PIN;
    config.pins.csn_pin = BSP_QSPI_CSN_PIN;
    config.pins.io0_pin = BSP_QSPI_IO0_PIN;
    config.pins.io1_pin = BSP_QSPI_IO1_PIN;
    config.pins.io2_pin = BSP_QSPI_IO2_PIN;
    config.pins.io3_pin = BSP_QSPI_IO3_PIN;

    // Try to initialize QSPI peripheral in blocking mode.
    err_code = nrfx_qspi_init(&config, qspi_handler, NULL);
    APP_ERROR_CHECK(err_code);
    printf("QSPI initialized\n");

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