
#define NRF_UARTE  NRF_UARTE1_S

// Display
#define NRF_SPIM   NRF_SPIM2_S
#define NRF_GPIOTE NRF_GPIOTE0_S

#define NRF_TIMER0 NRF_TIMER0_S

#define NRF_GPIO_PIN_MAP(port, pin) (((port) << 5) | ((pin) & 0x1F))

#define BUTTON_PIN_0 23
#define BUTTON_PIN_1 24
#define BUTTON_PIN_2 8
#define BUTTON_PIN_3 9

#define UART_TX_PIN 20
#define UART_RX_PIN 22

#define QSPI_SCK_PIN 17
#define QSPI_CSN_PIN 18
#define QSPI_IO0_PIN 13
#define QSPI_IO1_PIN 14
#define QSPI_IO2_PIN 15
#define QSPI_IO3_PIN 16


#define SDC_SCK_PIN     NRF_GPIO_PIN_MAP(1, 15)
#define SDC_MOSI_PIN    NRF_GPIO_PIN_MAP(1, 13)
#define SDC_MISO_PIN    NRF_GPIO_PIN_MAP(1, 14)
#define SDC_CS_PIN      NRF_GPIO_PIN_MAP(1, 12)



// nrf52 DK
// #define DISPLAY_PIN_SCK  3
// #define DISPLAY_PIN_MISO  4
// #define DISPLAY_PIN_MOSI  28
// #define DISPLAY_PIN_CS_N  29
// #define DISPLAY_PIN_INT  30
// #define DISPLAY_PIN_PD_N  31

#define DISPLAY_PIN_SCK  4
#define DISPLAY_PIN_MISO 5
#define DISPLAY_PIN_MOSI 6
#define DISPLAY_PIN_CS_N 7
#define DISPLAY_PIN_INT  25
#define DISPLAY_PIN_PD_N 26