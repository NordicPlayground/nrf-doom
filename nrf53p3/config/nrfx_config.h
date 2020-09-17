

//==========================================================
// <e> NRFX_PRS_ENABLED - nrfx_prs - Peripheral Resource Sharing module
//==========================================================
#define NRFX_PRS_ENABLED 0
#define NRFX_PRS_BOX_0_ENABLED 0
#define NRFX_PRS_BOX_1_ENABLED 0
#define NRFX_PRS_BOX_2_ENABLED 0
#define NRFX_PRS_BOX_3_ENABLED 0
#define NRFX_PRS_BOX_4_ENABLED 1


// <e> NRFX_SPIM_ENABLED - nrfx_spim - SPIM peripheral driver
//==========================================================
#define NRFX_SPIM_ENABLED 1
#define NRFX_SPIM0_ENABLED 1
#define NRFX_SPIM1_ENABLED 0
#define NRFX_SPIM2_ENABLED 0
#define NRFX_SPIM3_ENABLED 0

// <o> NRFX_SPIM_DEFAULT_CONFIG_IRQ_PRIORITY  - Interrupt priority
#define NRFX_SPIM_DEFAULT_CONFIG_IRQ_PRIORITY 6
#define NRFX_SPIM_CONFIG_LOG_ENABLED 0
#define NRFX_SPIM_CONFIG_LOG_LEVEL 3
#define NRFX_SPIM_CONFIG_INFO_COLOR 0
#define NRFX_SPIM_CONFIG_DEBUG_COLOR 0

// <o> NRFX_SPIM_MISO_PULL_CFG  - MISO pin pull configuration.
// <0=> NRF_GPIO_PIN_NOPULL 
// <1=> NRF_GPIO_PIN_PULLDOWN 
// <3=> NRF_GPIO_PIN_PULLUP 
#define NRFX_SPIM_MISO_PULL_CFG 1


// <e> NRFX_QSPI_ENABLED - nrfx_qspi - QSPI peripheral driver
//==========================================================
#define NRFX_QSPI_ENABLED 1

#define NRFX_QSPI_CONFIG_SCK_DELAY 1

// <o> NRFX_QSPI_CONFIG_XIP_OFFSET - Address offset in the external memory for Execute in Place operation. 
#define NRFX_QSPI_CONFIG_XIP_OFFSET 0

// <o> NRFX_QSPI_CONFIG_READOC  - Number of data lines and opcode used for reading.
// <0=> FastRead 
// <1=> Read2O 
// <2=> Read2IO 
// <3=> Read4O 
// <4=> Read4IO 
#define NRFX_QSPI_CONFIG_READOC 4

// <o> NRFX_QSPI_CONFIG_WRITEOC  - Number of data lines and opcode used for writing.
// <0=> PP 
// <1=> PP2O 
// <2=> PP4O 
// <3=> PP4IO 
#define NRFX_QSPI_CONFIG_WRITEOC 3

// <o> NRFX_QSPI_CONFIG_ADDRMODE  - Addressing mode.
// <0=> 24bit 
// <1=> 32bit 
#define NRFX_QSPI_CONFIG_ADDRMODE 0

// <o> NRFX_QSPI_CONFIG_MODE  - SPI mode.
// <0=> Mode 0 
// <1=> Mode 1 
#define NRFX_QSPI_CONFIG_MODE 0

// <o> NRFX_QSPI_CONFIG_FREQUENCY  - Frequency divider.
#define NRFX_QSPI_CONFIG_FREQUENCY 1

#define NRFX_QSPI_PIN_SCK NRF_QSPI_PIN_NOT_CONNECTED
#define NRFX_QSPI_PIN_CSN NRF_QSPI_PIN_NOT_CONNECTED
#define NRFX_QSPI_PIN_IO0 NRF_QSPI_PIN_NOT_CONNECTED
#define NRFX_QSPI_PIN_IO1 NRF_QSPI_PIN_NOT_CONNECTED
#define NRFX_QSPI_PIN_IO2 NRF_QSPI_PIN_NOT_CONNECTED
#define NRFX_QSPI_PIN_IO3 NRF_QSPI_PIN_NOT_CONNECTED

// <o> NRFX_QSPI_CONFIG_IRQ_PRIORITY  - Interrupt priority
#define NRFX_QSPI_CONFIG_IRQ_PRIORITY 6
