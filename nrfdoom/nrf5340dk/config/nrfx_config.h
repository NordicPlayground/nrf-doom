
#define NRF_CLOCK        NRF_CLOCK_S
#define NRF_COMP         NRF_COMP_S
#define NRF_DCNF         NRF_DCNF_S
#define NRF_DPPIC        NRF_DPPIC_S
#define NRF_EGU0         NRF_EGU0_S
#define NRF_EGU1         NRF_EGU1_S
#define NRF_EGU2         NRF_EGU2_S
#define NRF_EGU3         NRF_EGU3_S
#define NRF_EGU4         NRF_EGU4_S
#define NRF_EGU5         NRF_EGU5_S
#define NRF_FPU          NRF_FPU_S
#define NRF_I2S0         NRF_I2S0_S
#define NRF_IPC          NRF_IPC_S
#define NRF_KMU          NRF_KMU_S
#define NRF_LPCOMP       NRF_LPCOMP_S
#define NRF_MUTEX        NRF_MUTEX_S
#define NRF_NFCT         NRF_NFCT_S
#define NRF_NVMC         NRF_NVMC_S
#define NRF_OSCILLATORS  NRF_OSCILLATORS_S
#define NRF_P0           NRF_P0_S
#define NRF_P1           NRF_P1_S
#define NRF_PDM0         NRF_PDM0_S
#define NRF_POWER        NRF_POWER_S
#define NRF_PWM0         NRF_PWM0_S
#define NRF_PWM1         NRF_PWM1_S
#define NRF_PWM2         NRF_PWM2_S
#define NRF_PWM3         NRF_PWM3_S
#define NRF_QDEC0        NRF_QDEC0_S
#define NRF_QDEC1        NRF_QDEC1_S
#define NRF_QSPI         NRF_QSPI_S
#define NRF_REGULATORS   NRF_REGULATORS_S
#define NRF_RESET        NRF_RESET_S
#define NRF_RTC0         NRF_RTC0_S
#define NRF_RTC1         NRF_RTC1_S
#define NRF_SAADC        NRF_SAADC_S
#define NRF_SPIM0        NRF_SPIM0_S
#define NRF_SPIM1        NRF_SPIM1_S
#define NRF_SPIM2        NRF_SPIM2_S
#define NRF_SPIM3        NRF_SPIM3_S
#define NRF_SPIM4        NRF_SPIM4_S
#define NRF_SPIS0        NRF_SPIS0_S
#define NRF_SPIS1        NRF_SPIS1_S
#define NRF_SPIS2        NRF_SPIS2_S
#define NRF_SPIS3        NRF_SPIS3_S
#define NRF_TIMER0       NRF_TIMER0_S
#define NRF_TIMER1       NRF_TIMER1_S
#define NRF_TIMER2       NRF_TIMER2_S
#define NRF_TWIM0        NRF_TWIM0_S
#define NRF_TWIM1        NRF_TWIM1_S
#define NRF_TWIM2        NRF_TWIM2_S
#define NRF_TWIM3        NRF_TWIM3_S
#define NRF_TWIS0        NRF_TWIS0_S
#define NRF_TWIS1        NRF_TWIS1_S
#define NRF_TWIS2        NRF_TWIS2_S
#define NRF_TWIS3        NRF_TWIS3_S
#define NRF_UARTE0       NRF_UARTE0_S
#define NRF_UARTE1       NRF_UARTE1_S
#define NRF_UARTE2       NRF_UARTE2_S
#define NRF_UARTE3       NRF_UARTE3_S
#define NRF_USBD         NRF_USBD_S
#define NRF_USBREGULATOR NRF_USBREGULATOR_S
#define NRF_VMC          NRF_VMC_S
#define NRF_WDT0         NRF_WDT0_S
#define NRF_WDT1         NRF_WDT1_S

/*
 * The following section provides the name translation for peripherals with
 * only one type of access available. For these peripherals, you cannot choose
 * between secure and non-secure mapping.
 */
#define NRF_CACHE        NRF_CACHE_S
#define NRF_CACHEINFO    NRF_CACHEINFO_S
#define NRF_CACHEDATA    NRF_CACHEDATA_S
#define NRF_CRYPTOCELL   NRF_CRYPTOCELL_S
#define NRF_CTI          NRF_CTI_S
#define NRF_FICR         NRF_FICR_S
#define NRF_GPIOTE0      NRF_GPIOTE0_S
#define NRF_SPU          NRF_SPU_S
#define NRF_TAD          NRF_TAD_S
#define NRF_UICR         NRF_UICR_S

/* Fixups for the GPIOTE driver. */
#define NRF_GPIOTE        NRF_GPIOTE0
#define GPIOTE_IRQHandler GPIOTE0_IRQHandler

/* Fixups for the QDEC driver. */
#define NRF_QDEC        NRF_QDEC0
#define QDEC_IRQHandler QDEC0_IRQHandler

//==========================================================

#define NRFX_POWER_ENABLED 1
#define NRFX_POWER_DEFAULT_CONFIG_IRQ_PRIORITY 3

#define NRFX_CLOCK_ENABLED 1
#define NRFX_CLOCK_DEFAULT_CONFIG_IRQ_PRIORITY 3

// <o> NRFX_CLOCK_CONFIG_LF_SRC  - LF clock source.

// <0=> ULP
// <1=> RC
// <2=> XTAL
// <3=> Synth

#ifndef NRFX_CLOCK_CONFIG_LF_SRC
#define NRFX_CLOCK_CONFIG_LF_SRC 2
#endif

// <o> NRFX_CLOCK_CONFIG_HFCLK192M_SRC  - HFCLK192M source.

// <0=> HFINT
// <1=> HFXO

#ifndef NRFX_CLOCK_CONFIG_HFCLK192M_SRC
#define NRFX_CLOCK_CONFIG_HFCLK192M_SRC 1
#endif
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
#define NRFX_QSPI_CONFIG_FREQUENCY 0

#define NRFX_QSPI_PIN_SCK NRF_QSPI_PIN_NOT_CONNECTED
#define NRFX_QSPI_PIN_CSN NRF_QSPI_PIN_NOT_CONNECTED
#define NRFX_QSPI_PIN_IO0 NRF_QSPI_PIN_NOT_CONNECTED
#define NRFX_QSPI_PIN_IO1 NRF_QSPI_PIN_NOT_CONNECTED
#define NRFX_QSPI_PIN_IO2 NRF_QSPI_PIN_NOT_CONNECTED
#define NRFX_QSPI_PIN_IO3 NRF_QSPI_PIN_NOT_CONNECTED

// <o> NRFX_QSPI_CONFIG_IRQ_PRIORITY  - Interrupt priority
#define NRFX_QSPI_DEFAULT_CONFIG_IRQ_PRIORITY 7
