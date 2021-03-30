
#include <stdio.h>
#include <stdint.h>

#include <nrf.h>
#include <nrf_delay.h>

#include "uart.h"

#define NRF_TIMER  NRF_TIMER0_NS
#define NRF_RADIO  NRF_RADIO_NS

volatile NRF_IPC_Type* NRF_APP_IPC = (NRF_IPC_Type*)0x5002A000UL;


// static uint32_t  packet;
volatile uint32_t  packet[2];

const int PACKET_BASE_ADDRESS_LENGTH = 4;
const int PACKET_STATIC_LENGTH = 4;
const int PACKET_PAYLOAD_MAXSIZE = 4;

const int PACKET_LENGTH_FIELD_SIZE = 8;
const int PACKET_S0_FIELD_SIZE = 0;
const int PACKET_S1_FIELD_SIZE = 0;


void radio_configure()
{
    NRF_RADIO->TXPOWER   = (RADIO_TXPOWER_TXPOWER_0dBm << RADIO_TXPOWER_TXPOWER_Pos);
    NRF_RADIO->FREQUENCY = 0UL;
    NRF_RADIO->MODE      = (RADIO_MODE_MODE_Nrf_1Mbit << RADIO_MODE_MODE_Pos);

    // Radio address config
    NRF_RADIO->PREFIX0 = 0x51;

    NRF_RADIO->BASE0 = 0x41312111;

    NRF_RADIO->TXADDRESS   = 0x00UL;  // Set device address 0 to use when transmitting
    NRF_RADIO->RXADDRESSES = 0x01UL;  // Enable device address 0 to use to select which addresses to receive

    // Packet configuration
    NRF_RADIO->PCNF0 = (PACKET_S1_FIELD_SIZE     << RADIO_PCNF0_S1LEN_Pos) |
                       (PACKET_S0_FIELD_SIZE     << RADIO_PCNF0_S0LEN_Pos) |
                       (PACKET_LENGTH_FIELD_SIZE << RADIO_PCNF0_LFLEN_Pos);

    // Packet configuration
    NRF_RADIO->PCNF1 = (RADIO_PCNF1_WHITEEN_Disabled << RADIO_PCNF1_WHITEEN_Pos) |
                       (RADIO_PCNF1_ENDIAN_Big       << RADIO_PCNF1_ENDIAN_Pos)  |
                       (PACKET_BASE_ADDRESS_LENGTH   << RADIO_PCNF1_BALEN_Pos)   |
                       (PACKET_STATIC_LENGTH         << RADIO_PCNF1_STATLEN_Pos) |
                       (PACKET_PAYLOAD_MAXSIZE       << RADIO_PCNF1_MAXLEN_Pos);

    // CRC Config
    NRF_RADIO->CRCCNF = (RADIO_CRCCNF_LEN_Two << RADIO_CRCCNF_LEN_Pos); // Number of checksum bits
    if ((NRF_RADIO->CRCCNF & RADIO_CRCCNF_LEN_Msk) == (RADIO_CRCCNF_LEN_Two << RADIO_CRCCNF_LEN_Pos))
    {
        NRF_RADIO->CRCINIT = 0xFFFFUL;   // Initial value
        NRF_RADIO->CRCPOLY = 0x11021UL;  // CRC poly: x^16 + x^12^x^5 + 1
    }
    else if ((NRF_RADIO->CRCCNF & RADIO_CRCCNF_LEN_Msk) == (RADIO_CRCCNF_LEN_One << RADIO_CRCCNF_LEN_Pos))
    {
        NRF_RADIO->CRCINIT = 0xFFUL;   // Initial value
        NRF_RADIO->CRCPOLY = 0x107UL;  // CRC poly: x^8 + x^2^x^1 + 1
    }

    // NRF_RADIO->PACKETPTR = (uint32_t)(&packet);
    NRF_RADIO->PACKETPTR = (uint32_t)(&packet[0]);

}


void radio_enable()
{
    NRF_RADIO->EVENTS_READY = 0U;
    // Enable radio and wait for ready
    NRF_RADIO->TASKS_RXEN = 1U;

    while (NRF_RADIO->EVENTS_READY == 0U)
    {
        // wait
    }
}


void send_packet()
{
    // send the packet:
    NRF_RADIO->EVENTS_READY = 0U;
    NRF_RADIO->TASKS_TXEN   = 1;

    while (NRF_RADIO->EVENTS_READY == 0U)
    {
        // wait
    }
    NRF_RADIO->EVENTS_END  = 0U;
    NRF_RADIO->TASKS_START = 1U;

    while (NRF_RADIO->EVENTS_END == 0U)
    {
        // wait
    }

    NRF_RADIO->EVENTS_DISABLED = 0U;
    // Disable radio
    NRF_RADIO->TASKS_DISABLE = 1U;

    while (NRF_RADIO->EVENTS_DISABLED == 0U)
    {
        // wait
    }
}


void radio_disable()
{
    NRF_RADIO->EVENTS_DISABLED = 0U;
    // Disable radio
    NRF_RADIO->TASKS_DISABLE = 1U;

    while (NRF_RADIO->EVENTS_DISABLED == 0U)
    {
        // wait
    }
}



/**@brief Function for reading packet.
 */
uint32_t read_packet2()
{
    uint32_t result = 0;

    NRF_RADIO->EVENTS_READY = 0U;
    // Enable radio and wait for ready
    NRF_RADIO->TASKS_RXEN = 1U;

    while (NRF_RADIO->EVENTS_READY == 0U)
    {
        // wait
    }
    NRF_RADIO->EVENTS_END = 0U;
    // Start listening and wait for address received event
    NRF_RADIO->TASKS_START = 1U;

    // Wait for end of packet or buttons state changed
    while (NRF_RADIO->EVENTS_END == 0U)
    {
        // wait
    }

    NRF_RADIO->EVENTS_DISABLED = 0U;
    // Disable radio
    NRF_RADIO->TASKS_DISABLE = 1U;

    while (NRF_RADIO->EVENTS_DISABLED == 0U)
    {
        // wait
    }
    return 1;
}

void clock_initialization()
{
    /* Start 16 MHz crystal oscillator */
    NRF_CLOCK_NS->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK_NS->TASKS_HFCLKSTART    = 1;

    /* Wait for the external oscillator to start up */
    while (NRF_CLOCK_NS->EVENTS_HFCLKSTARTED == 0)
    {
        // Do nothing.
    }

}

int main() {

    NRF_P0_NS->PIN_CNF[30] = 3;
    NRF_P0_NS->PIN_CNF[31] = 3;

    NRF_P0_NS->OUT = (1<<31);


    uart_init();

    // uart_mutex_grab();
    printf("Hello from nRF53 Network MCU\n");

    clock_initialization();

    // printf("Clock initialized\n");

    // initialize timer
    // NRF_TIMER->MODE = TIMER_MODE_MODE_Timer;
    // NRF_TIMER->BITMODE = TIMER_BITMODE_BITMODE_32Bit;
    // NRF_TIMER->PRESCALER = 1; // fTIMER = 16 MHz / (2*PRESCALER)
    // NRF_TIMER->TASKS_START = 1;

    radio_configure();
    // radio_enable();
    // printf("Radio configured\n");

    // uart_mutex_release();


    volatile uint32_t *ipc_ptr_0 = &NRF_APP_IPC->GPMEM[0];
    volatile uint32_t *ipc_ptr_1 = &NRF_APP_IPC->GPMEM[1];

    int i=0;
    while (1) {
        NRF_RADIO->EVENTS_CRCOK = 0;
        NRF_RADIO->EVENTS_CRCERROR = 0;
        packet[0] = 0;
        if (read_packet2()) {
            NRF_P0_NS->OUT = NRF_P0_NS->OUT^(1<<31);
            *ipc_ptr_0 = packet[0];

            uint32_t face = *ipc_ptr_1;
            // printf("%d: %ld\n", i++, face);
            packet[0] = face;
            send_packet();
            send_packet();
            send_packet();
        }
    }

    return 0;
}