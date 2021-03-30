
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <nrf.h>
#include <nrf_delay.h>

#define NRF_TIMER  NRF_TIMER0

#define RX_PIN_NUMBER  8
#define TX_PIN_NUMBER  6

volatile char uart_tx_buf[32];

const int joyBtnA = 17;  // MB pin  5
const int joyBtnB = 26; //  MB pin 11
const int joyBtnC = 21; //  MB pin 15
const int joyBtnD = 22; //  MB pin 14
const int joyBtnE = 23; //  MB pin 13
const int joyBtnF = 20; //  MB pin 12
const int joyBtnP = 18; //  MB pin 8

const int joyX = 1; //  MB pin 1 AIN2
const int joyY = 2; //  MB pin 2 AIN3


// -- Board configuration --
const int ledCol1 = 4;
const int ledCol2 = 5;
const int ledCol3 = 6;
const int ledCol4 = 7;
const int ledCol5 = 8;
const int ledCol6 = 9;
const int ledCol7 = 10;
const int ledCol8 = 11;
const int ledCol9 = 12;

const int ledRow1 = 13;
const int ledRow2 = 14;
const int ledRow3 = 15;

void uart_init(void)
{
    NRF_UART0->PSELTXD = 24; //TX_PIN_NUMBER;

    NRF_UART0->BAUDRATE = UART_BAUDRATE_BAUDRATE_Baud115200;
    NRF_UART0->CONFIG = 0;
    NRF_UART0->ENABLE = UART_ENABLE_ENABLE_Enabled;

    NRF_UART0->EVENTS_RXDRDY = 0;
    NRF_UART0->EVENTS_TXDRDY = 0;
}


void uart_putc(char ch)
{
    NRF_UART0->EVENTS_TXDRDY = 0;

    NRF_UART0->TXD = ch;

    NRF_UART0->TASKS_STARTTX = 1;

    while (!NRF_UART0->EVENTS_TXDRDY) {}

}


//  Retargeting (for printf etc)
// ------------------------------------

int __getchar(FILE * p_file)
{
    uint8_t input = 0;
    return input;
}

int __putchar(int ch, FILE * p_file)
{
    uart_putc(ch);
    return ch;
}

int _write(int file, const char * p_char, int len)
{
    int i;

    for (i = 0; i < len; i++)
    {
        uart_putc(*p_char++);
    }
    return len;
}

int _read(int file, char * p_char, int len)
{
    return 1;
}




void clock_initialization()
{
    /* Start 16 MHz crystal oscillator */
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART    = 1;

    /* Wait for the external oscillator to start up */
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
    {
        // Do nothing.
    }

    /* Start low frequency crystal oscillator for app_timer(used by bsp)*/
    // NRF_CLOCK->LFCLKSRC            = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
    // NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
    // NRF_CLOCK->TASKS_LFCLKSTART    = 1;

    // while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
    // {
    //     // Do nothing.
    // }
}

static volatile uint8_t  tx_packet[4];
static volatile uint8_t  rx_packet[4];

const int PACKET_BASE_ADDRESS_LENGTH = 4;
const int PACKET_STATIC_LENGTH = 4;
const int PACKET_PAYLOAD_MAXSIZE = 4;

const int PACKET_LENGTH_FIELD_SIZE = 8;
const int PACKET_S0_FIELD_SIZE = 0;
const int PACKET_S1_FIELD_SIZE = 0;


void radio_configure()
{

    // Radio config
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

}


void send_packet()
{
    // printf("Sending tx_packet with data %d %x %d %d\n", tx_packet[0], tx_packet[1], tx_packet[2], tx_packet[3]);

    NRF_RADIO->PACKETPTR = (uint32_t)(&tx_packet[0]);

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

/**@brief Function for reading packet.
 */
uint32_t read_packet(uint32_t timeout)
{
    uint32_t result = 0;

    NRF_RADIO->PACKETPTR = (uint32_t)(&rx_packet[0]);

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

    /*
    NRF_TIMER->TASKS_CLEAR = 1;
    // Wait for end of packet 
    while (NRF_RADIO->EVENTS_END == 0U)
    {
        // wait
        NRF_TIMER->TASKS_CAPTURE[0] = 1;
        if (NRF_TIMER->CC[0] > timeout) {
            NRF_RADIO->TASKS_STOP = 1U;
            // while (NRF_RADIO->EVENTS_DISABLED == 0U)
            // {
            //     // wait
            // }
            return 0;
        }
    }
    */
    nrf_delay_us(timeout);
    if (NRF_RADIO->EVENTS_END) {
        result = 1;
    }


    NRF_RADIO->EVENTS_DISABLED = 0U;
    // Disable radio
    NRF_RADIO->TASKS_DISABLE = 1U;

    while (NRF_RADIO->EVENTS_DISABLED == 0U)
    {
        // wait
    }
    return result;
}

uint8_t doomFaces[10][5] = {
        // Straight faces
        {0b00001,
         0b11011,
         0b00000,
         0b00000,
         0b01110},
        {0b00000,
         0b11011,
         0b00000,
         0b00000,
         0b01110},
        {0b10000,
         0b11011,
         0b00000,
         0b00000,
         0b01110},
        // Turn right
        {0b00000,
         0b01101,
         0b00000,
         0b00000,
         0b00111},
        // Turn right
        {0b00000,
         0b10110,
         0b00000,
         0b11100,
         0b00010},
        // Ouch
        {0b01010,
         0b10001,
         0b00100,
         0b01010,
         0b01110},
        // Grin
        {0b10001,
         0b01010,
         0b00000,
         0b10001,
         0b01110},
        // Kill / grit teeth
        {0b10001,
         0b01010,
         0b00000,
         0b01110,
         0b01110},
        // God
        {0b00000,
         0b11011,
         0b11011,
         0b00000,
         0b00000},
        // Dead
        {0b00000,
         0b00000,
         0b00000,
         0b11011,
         0b00000}
    };

uint8_t displayBuffer[5];
uint8_t displayLedRowCtr;


void displayClear() {
    for (int i=0;i<5;i++) displayBuffer[i] = 0;
}

void displayInit() {
    NRF_GPIO->PIN_CNF[ledCol1] = 3;
    NRF_GPIO->PIN_CNF[ledCol2] = 3;
    NRF_GPIO->PIN_CNF[ledCol3] = 3;
    NRF_GPIO->PIN_CNF[ledCol4] = 3;
    NRF_GPIO->PIN_CNF[ledCol5] = 3;
    NRF_GPIO->PIN_CNF[ledCol6] = 3;
    NRF_GPIO->PIN_CNF[ledCol7] = 3;
    NRF_GPIO->PIN_CNF[ledCol8] = 3;
    NRF_GPIO->PIN_CNF[ledCol9] = 3;
    NRF_GPIO->PIN_CNF[ledRow1] = 3;
    NRF_GPIO->PIN_CNF[ledRow2] = 3;
    NRF_GPIO->PIN_CNF[ledRow3] = 3;
    displayLedRowCtr = 1;
    displayClear();
}


// Mapping from grid coordinates to physical LED connections
void renderLedRow(int ledRow) {
    // Turn all LED columns/rows off
    NRF_GPIO->OUTSET = (1<<ledCol1)|(1<<ledCol2)|(1<<ledCol3)|(1<<ledCol4)|(1<<ledCol5)|(1<<ledCol6)|(1<<ledCol7)|(1<<ledCol8)|(1<<ledCol9);
    NRF_GPIO->OUTCLR = (1<<ledRow1) | (1<<ledRow2) | (1<<ledRow3);
    if (ledRow == 1) {
        NRF_GPIO->OUTCLR =
            (((displayBuffer[0]>>4) & 1) << ledCol1) |
            (((displayBuffer[0]>>2) & 1) << ledCol2) |
            (((displayBuffer[0]>>0) & 1) << ledCol3) |
            (((displayBuffer[3]>>0) & 1) << ledCol4) |
            (((displayBuffer[3]>>1) & 1) << ledCol5) |
            (((displayBuffer[3]>>2) & 1) << ledCol6) |
            (((displayBuffer[3]>>3) & 1) << ledCol7) |
            (((displayBuffer[3]>>4) & 1) << ledCol8) |
            (((displayBuffer[2]>>3) & 1) << ledCol9);
        NRF_GPIO->OUTSET = (1<<ledRow1);
    }
    if (ledRow == 2) {
        NRF_GPIO->OUTCLR =
            (((displayBuffer[2]>>0) & 1) << ledCol1) |
            (((displayBuffer[2]>>4) & 1) << ledCol2) |
            (((displayBuffer[2]>>2) & 1) << ledCol3) |
            (((displayBuffer[0]>>3) & 1) << ledCol4) |
            (((displayBuffer[0]>>1) & 1) << ledCol5) |
            (((displayBuffer[4]>>1) & 1) << ledCol6) |
            (((displayBuffer[4]>>3) & 1) << ledCol7) |
            (((displayBuffer[3]>>4) & 1) << ledCol8) |
            (((displayBuffer[2]>>3) & 1) << ledCol9);
        NRF_GPIO->OUTSET = (1<<ledRow2);
    }
    if (ledRow == 3) {
        NRF_GPIO->OUTCLR =
            (((displayBuffer[4]>>2) & 1) << ledCol1) |
            (((displayBuffer[4]>>0) & 1) << ledCol2) |
            (((displayBuffer[4]>>4) & 1) << ledCol3) |
            (((displayBuffer[1]>>4) & 1) << ledCol4) |
            (((displayBuffer[1]>>3) & 1) << ledCol5) |
            (((displayBuffer[1]>>2) & 1) << ledCol6) |
            (((displayBuffer[1]>>1) & 1) << ledCol7) |
            (((displayBuffer[1]>>0) & 1) << ledCol8) |
            (((displayBuffer[2]>>1) & 1) << ledCol9);
        NRF_GPIO->OUTSET = (1<<ledRow3);
    }
}

void displayRender() {
    displayLedRowCtr += 1;
    if (displayLedRowCtr > 3) {
        displayLedRowCtr = 1;
    }
    renderLedRow(displayLedRowCtr);
}

void displayLedOn(int x, int y) {
    displayBuffer[y] |= ((0x10)>>(x));
}

void displayLedOff(int x, int y) {
    displayBuffer[y] &= ~((0x10)>>(x));
}

void adc_init() {
    NRF_ADC->CONFIG = (ADC_CONFIG_RES_8bit << ADC_CONFIG_RES_Pos) |
                      (ADC_CONFIG_INPSEL_AnalogInputOneThirdPrescaling << ADC_CONFIG_INPSEL_Pos) |
                      (ADC_CONFIG_REFSEL_VBG << ADC_CONFIG_REFSEL_Pos);
    NRF_ADC->ENABLE = 1;
}


void timer_configure() {
    // initialize timer
    NRF_TIMER->MODE = TIMER_MODE_MODE_Timer;
    NRF_TIMER->BITMODE = TIMER_BITMODE_BITMODE_32Bit;
    NRF_TIMER->PRESCALER = 4; // fTIMER = 16 MHz / (2*PRESCALER)
    NRF_TIMER->TASKS_START = 1;
}

int main() {

    int joyBtnA_state;
    int joyBtnB_state;
    int joyBtnC_state;
    int joyBtnD_state;
    int joyBtnE_state;
    int joyBtnF_state;
    int joyBtnP_state;

    displayInit();

    NRF_GPIO->PIN_CNF[joyBtnA] = 0;
    NRF_GPIO->PIN_CNF[joyBtnB] = 0;
    NRF_GPIO->PIN_CNF[joyBtnC] = 0;
    NRF_GPIO->PIN_CNF[joyBtnD] = 0;
    NRF_GPIO->PIN_CNF[joyBtnE] = 0;
    NRF_GPIO->PIN_CNF[joyBtnF] = 0;
    NRF_GPIO->PIN_CNF[joyBtnP] = 0;
    // NRF_GPIO->PIN_CNF[joyBtnG] = 0;
    // NRF_GPIO->PIN_CNF[joyBtnH] = 0;

    NRF_GPIO->PIN_CNF[joyX] = 0;
    NRF_GPIO->PIN_CNF[joyY] = 0;


    uart_init();

    printf("Micro:bit Gamepad Controller\n");

    clock_initialization();

    radio_configure();

    timer_configure();

    // Set payload pointer
    adc_init();

    printf("Initialization done.\n");

    int joyX_value, joyY_value;

    static uint8_t  prevState[4];

    int face=0;
    int i=0,j=0;
    int tx_counter = 0;
    while (1) {
        NRF_ADC->CONFIG = (NRF_ADC->CONFIG & ~ADC_CONFIG_PSEL_Msk) | (ADC_CONFIG_PSEL_AnalogInput2 << ADC_CONFIG_PSEL_Pos);
        NRF_ADC->EVENTS_END = 0;
        NRF_ADC->TASKS_START = 1;
        while (!NRF_ADC->EVENTS_END) {}
        joyX_value = NRF_ADC->RESULT;

        NRF_ADC->CONFIG = (NRF_ADC->CONFIG & ~ADC_CONFIG_PSEL_Msk) | (ADC_CONFIG_PSEL_AnalogInput3 << ADC_CONFIG_PSEL_Pos);
        NRF_ADC->EVENTS_END = 0;
        NRF_ADC->TASKS_START = 1;
        while (!NRF_ADC->EVENTS_END) {}
        joyY_value = NRF_ADC->RESULT;

        joyBtnA_state = ((NRF_GPIO->IN & (1<<joyBtnA)) == 0);
        joyBtnB_state = ((NRF_GPIO->IN & (1<<joyBtnB)) == 0);
        joyBtnC_state = ((NRF_GPIO->IN & (1<<joyBtnC)) == 0);
        joyBtnD_state = ((NRF_GPIO->IN & (1<<joyBtnD)) == 0);
        joyBtnE_state = ((NRF_GPIO->IN & (1<<joyBtnE)) == 0);
        joyBtnF_state = ((NRF_GPIO->IN & (1<<joyBtnF)) == 0);
        joyBtnP_state = ((NRF_GPIO->IN & (1<<joyBtnP)) == 0);

        displayClear();

        if (face >= 0 && face <= 8*5+1) {
            int facePic = 0;
            if (face == 8*5)  { // God
                facePic = 8;
            }
            else if (face == 8*5+1) { // Dead
                facePic = 9;
            }
            else {
                facePic = face%8;
            }
            // printf("face = %d, facePic = %d\n", face, facePic);
            memcpy(displayBuffer, doomFaces[facePic], 5);
        }
/*
        if (joyBtnA_state) displayLedOn(0,0);
        if (joyBtnB_state) displayLedOn(4,0);
        if (joyBtnC_state) displayLedOn(0,1);
        if (joyBtnD_state) displayLedOn(1,1);
        if (joyBtnE_state) displayLedOn(2,1);
        if (joyBtnF_state) displayLedOn(3,1);
        if (joyBtnP_state) displayLedOn(4,1);

        if (joyX_value < 110 || joyX_value > 120) displayLedOn(2,3);
        if (joyX_value < 70)  displayLedOn(1,3);
        if (joyX_value < 35)  displayLedOn(0,3);
        if (joyX_value > 155) displayLedOn(3,3);
        if (joyX_value > 190) displayLedOn(4,3);

        if (joyY_value < 110 || joyY_value > 120) displayLedOn(2,4);
        if (joyY_value < 70)  displayLedOn(1,4);
        if (joyY_value < 35)  displayLedOn(0,4);
        if (joyY_value > 155) displayLedOn(3,4);
        if (joyY_value > 190) displayLedOn(4,4);
*/
        displayRender();

        tx_packet[1] =  (joyBtnA_state << 0) |
                     (joyBtnB_state << 1) |
                     (joyBtnC_state << 2) |
                     (joyBtnD_state << 3) |
                     (joyBtnE_state << 4) |
                     (joyBtnF_state << 5) |
                     (joyBtnP_state << 6);
        if (joyX_value > 105 && joyX_value < 125) joyX_value = 112;
        if (joyY_value > 105 && joyY_value < 125) joyY_value = 112;


        tx_packet[2] = joyX_value;
        tx_packet[3] = joyY_value;

        if ((tx_packet[1] != prevState[1]) ||
            (prevState[2] != tx_packet[2]) ||
            (prevState[3] != tx_packet[3]) ) {
            // printf("joyX = %d, joyY = %d\n", joyX_value, joyY_value);
            // printf("Sending tx_packet with data %d\n", tx_packet[0]);
            tx_counter++;
            send_packet();
            i = 0;
            j = 0;
        }

        tx_packet[0] = tx_counter;

        prevState[1] = tx_packet[1];
        prevState[2] = tx_packet[2];
        prevState[3] = tx_packet[3];

        i++; j++;
        if (j <= 3) {
            // Send bust of 3 packets 1 tic apart
            if (i==1) {
                send_packet();
                i = 0;
            }
        }
        else if (j < 2000) {
            // Then a few more 5 tics apart
            if (i==5) {
                send_packet();
                i = 0;
            }
        }
        else if (j < 80000) {
            if (i==100) {
                send_packet();
                i = 0;
            }
        }
        else if (j==80000) {
            printf("Stop\n");
        }
        else {
            // Then stop
        }

        // printf("Read packet for 500us\n");
        if (read_packet(200)) {
            // printf("Got packet! %d %d %d %d\n", rx_packet[0], rx_packet[1], rx_packet[2], rx_packet[3]);
            face = rx_packet[0];
        }
        else {
            // printf("Timeout\n");
        }
    }

    return 0;
}