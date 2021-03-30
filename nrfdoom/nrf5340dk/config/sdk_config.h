
#define APP_SDCARD_ENABLED 1

// <e> SPI_ENABLED - nrf_drv_spi - SPI/SPIM peripheral driver - legacy layer
//==========================================================
#define SPI_ENABLED 1
// <o> SPI_DEFAULT_CONFIG_IRQ_PRIORITY  - Interrupt priority

#define SPI0_ENABLED 1
#define SPI0_USE_EASY_DMA 1


// <i> Priorities 0,2 (nRF51) and 0,1,4,5 (nRF52) are reserved for SoftDevice
// <0=> 0 (highest) 
// <1=> 1 
// <2=> 2 
// <3=> 3 
// <4=> 4 
// <5=> 5 
// <6=> 6 
// <7=> 7 
#define SPI_DEFAULT_CONFIG_IRQ_PRIORITY 6

// <o> NRF_SPI_DRV_MISO_PULLUP_CFG  - MISO PIN pull-up configuration.
// <0=> NRF_GPIO_PIN_NOPULL 
// <1=> NRF_GPIO_PIN_PULLDOWN 
// <3=> NRF_GPIO_PIN_PULLUP 
#define NRF_SPI_DRV_MISO_PULLUP_CFG 1


// <e> APP_SDCARD_ENABLED - app_sdcard - SD/MMC card support using SPI
//==========================================================
#define APP_SDCARD_ENABLED 1
// <o> APP_SDCARD_SPI_INSTANCE  - SPI instance used
#define APP_SDCARD_SPI_INSTANCE 0

// <o> APP_SDCARD_FREQ_INIT  - SPI frequency
// <33554432=> 125 kHz 
// <67108864=> 250 kHz 
// <134217728=> 500 kHz 
// <268435456=> 1 MHz 
// <536870912=> 2 MHz 
// <1073741824=> 4 MHz 
// <2147483648=> 8 MHz 
#define APP_SDCARD_FREQ_INIT 67108864

// <o> APP_SDCARD_FREQ_DATA  - SPI frequency
// <33554432=> 125 kHz 
// <67108864=> 250 kHz 
// <134217728=> 500 kHz 
// <268435456=> 1 MHz 
// <536870912=> 2 MHz 
// <1073741824=> 4 MHz 
// <2147483648=> 8 MHz 
#define APP_SDCARD_FREQ_DATA 1073741824
