
#include <stdio.h>

#include "doomkeys.h"
#include "d_event.h"

#include "nrf.h"
#include "board_config.h"

#ifdef OLD_UART

void N_uart_init(void)
{
    NRF_UART0->PSELTXD = UART_TX_PIN;
    NRF_UART0->PSELRXD = UART_RX_PIN;
    NRF_UART0->BAUDRATE = UART_BAUDRATE_BAUDRATE_Baud115200;
    NRF_UART0->CONFIG = 0;
    NRF_UART0->ENABLE = UART_ENABLE_ENABLE_Enabled;
    NRF_UART0->TASKS_STARTTX = 1; 
    NRF_UART0->TASKS_STARTRX = 1; 
    NRF_UART0->EVENTS_RXDRDY = 0;
}

void N_uart_putc(char ch)
{
    NRF_UART0->EVENTS_TXDRDY = 0;
    NRF_UART0->TXD = ch;
    while (!NRF_UART0->EVENTS_TXDRDY) {}
}

boolean N_uart_has_data() {
    return NRF_UART0->EVENTS_RXDRDY;
}

char N_uart_getc() {
    while (!NRF_UART0->EVENTS_RXDRDY) {}
    NRF_UART0->EVENTS_RXDRDY = 0;
    return NRF_UART0->RXD;
}

#else

volatile char uart_tx_buf[4];
volatile char uart_rx_buf[32];
int uart_rx_count;
int uart_rx_cur;

void N_uart_init(void)
{
    NRF_UARTE->PSEL.TXD = UART_TX_PIN;
    NRF_UARTE->PSEL.RXD = UART_RX_PIN;
    NRF_UARTE->BAUDRATE = UARTE_BAUDRATE_BAUDRATE_Baud115200;
    NRF_UARTE->CONFIG = 0;
    NRF_UARTE->ENABLE = UARTE_ENABLE_ENABLE_Enabled;
    NRF_UARTE->EVENTS_RXDRDY = 0;
    NRF_UARTE->EVENTS_TXDRDY = 0;

    NRF_UARTE->EVENTS_RXSTARTED = 0;

    NRF_UARTE->RXD.PTR = (uint32_t)(&uart_rx_buf[0]);
    NRF_UARTE->RXD.MAXCNT = 32;
    NRF_UARTE->TASKS_STARTRX = 1;

    while (!NRF_UARTE->EVENTS_RXSTARTED) {}

    uart_rx_count = 0;
    uart_rx_cur = 0;

}

void N_uart_putc(char ch)
{
    NRF_UARTE->EVENTS_TXDRDY = 0;
    NRF_UARTE->EVENTS_ENDTX = 0;

    uart_tx_buf[0] = ch;
    NRF_UARTE->TXD.PTR = (uint32_t)(&uart_tx_buf[0]);
    NRF_UARTE->TXD.MAXCNT = 1;

    NRF_UARTE->TASKS_STARTTX = 1;

    while (!NRF_UARTE->EVENTS_ENDTX) {}
}

boolean N_uart_has_data() {
    NRF_UARTE->TASKS_STOPRX = 1;
    while (!NRF_UARTE->EVENTS_ENDRX) {}
    NRF_UARTE->EVENTS_ENDRX = 0;
    uart_rx_count = NRF_UARTE->RXD.AMOUNT;
    uart_rx_cur = 0;

    NRF_UARTE->RXD.PTR = (uint32_t)(&uart_rx_buf[0]);
    NRF_UARTE->RXD.MAXCNT = 32;
    NRF_UARTE->TASKS_STARTRX = 1;
    while (!NRF_UARTE->EVENTS_RXSTARTED) {}

    return uart_rx_count > 0;
}

char N_uart_getc() {
    if (uart_rx_cur >= uart_rx_count) {
        return 0;
    }
    char result = uart_rx_buf[uart_rx_cur];
    uart_rx_cur++;
    return result;
}

#endif

void N_ReadUart() 
{
    static event_t event;
    boolean hd = N_uart_has_data();
    if (hd) {
        char code = N_uart_getc();
        if (code == 'D') {
            char keycode = N_uart_getc();
            char keychar = N_uart_getc();
            // printf("Key Down: %d %d\n", keycode, keychar);
            event.type = ev_keydown;
            event.data1 = keycode;
            event.data2 = keychar;
            event.data3 = keychar;

            if (keycode != 0)
            {
                D_PostEvent(&event);
            }
        }
        else if (code == 'U') {
            char keycode = N_uart_getc();
            char keychar = N_uart_getc();
            // printf("Key Up: %d %d\n", keycode, keychar);
            event.type = ev_keyup;
            event.data1 = keycode;
            event.data2 = 0;
            event.data3 = 0;

            if (keycode != 0)
            {
                D_PostEvent(&event);
            }
        }
        else {
            printf("ERROR in N_ReadUart: Unhandled code %x\n", code);
        }
    }
}
