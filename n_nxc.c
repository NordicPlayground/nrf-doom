
// Implement SNES mini based controller (Nintendo Switch Wiimote Extension protocol)

#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "board_config.h"

#undef PACKED_STRUCT

#include "doomkeys.h"
#include "d_event.h"
#include "i_system.h"


#define NRF_TWIM NRF_TWIM0_S

#define PIN_SCL  24
#define PIN_SDA  23

uint8_t twimTxdBuffer[16];

void twim_init(void)
{
    NRF_P0_S->PIN_CNF[PIN_SCL] = ((GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)  
                                 | (GPIO_PIN_CNF_DRIVE_S0D1     << GPIO_PIN_CNF_DRIVE_Pos)
                                 | (GPIO_PIN_CNF_PULL_Pullup    << GPIO_PIN_CNF_PULL_Pos) 
                                 | (GPIO_PIN_CNF_INPUT_Connect  << GPIO_PIN_CNF_INPUT_Pos)
                                 | (GPIO_PIN_CNF_DIR_Input      << GPIO_PIN_CNF_DIR_Pos));
    NRF_P0_S->PIN_CNF[PIN_SDA] = ((GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos) 
                                  | (GPIO_PIN_CNF_DRIVE_S0D1     << GPIO_PIN_CNF_DRIVE_Pos)
                                  | (GPIO_PIN_CNF_PULL_Pullup    << GPIO_PIN_CNF_PULL_Pos)
                                  | (GPIO_PIN_CNF_INPUT_Connect  << GPIO_PIN_CNF_INPUT_Pos)
                                  | (GPIO_PIN_CNF_DIR_Input      << GPIO_PIN_CNF_DIR_Pos));

    NRF_TWIM->PSEL.SCL = PIN_SCL;
    NRF_TWIM->PSEL.SDA = PIN_SDA;

    NRF_TWIM->FREQUENCY = TWIM_FREQUENCY_FREQUENCY_K100;

    NRF_TWIM->SHORTS = TWIM_SHORTS_LASTRX_STOP_Msk|TWIM_SHORTS_LASTTX_STOP_Msk;

    NRF_TWIM->ENABLE = TWIM_ENABLE_ENABLE_Enabled;


}

int twim_read(uint8_t dev_address, uint8_t reg_address, uint32_t size, void *data)
{
    NRF_TWIM->ADDRESS = dev_address;

    NRF_TWIM->TXD.MAXCNT = 1;
    NRF_TWIM->TXD.PTR = (uint32_t)twimTxdBuffer;

    twimTxdBuffer[0] = reg_address;

    NRF_TWIM->EVENTS_STOPPED = 0;
    NRF_TWIM->EVENTS_ERROR = 0;
    NRF_TWIM->EVENTS_TXSTARTED = 0;
    NRF_TWIM->TASKS_STARTTX = 1;
    while (!NRF_TWIM->EVENTS_TXSTARTED) {}
    while (1) {
        if (NRF_TWIM->EVENTS_STOPPED) {
            break;
        }
        if (NRF_TWIM->EVENTS_ERROR) {
            printf("TWIM Error\n");
            return 0;
        }
    }

    // nrf_delay_us(100);

    NRF_TWIM->RXD.MAXCNT = size;
    NRF_TWIM->RXD.PTR = (uint32_t)data;

    NRF_TWIM->EVENTS_STOPPED = 0;
    NRF_TWIM->EVENTS_ERROR = 0;
    NRF_TWIM->TASKS_STARTRX = 1;

    while (1) {
        if (NRF_TWIM->EVENTS_STOPPED) {
            break;
        }
        if (NRF_TWIM->EVENTS_ERROR) {
            printf("TWIM Error\n");
            return 0;
        }
    }

    // printf("Got %ld bytes\n", NRF_TWIM->RXD.AMOUNT);

    return 1;
}


int twim_write_reg(uint8_t dev_address, uint8_t reg_address, uint8_t value)
{
    NRF_TWIM->ADDRESS = dev_address;

    NRF_TWIM->TXD.MAXCNT = 2;
    NRF_TWIM->TXD.PTR = (uint32_t)twimTxdBuffer;

    // *((uint32_t*)twimTxdBuffer) = reg_address;
    twimTxdBuffer[0] = reg_address;
    twimTxdBuffer[1] = value;

    NRF_TWIM->EVENTS_STOPPED = 0;
    NRF_TWIM->EVENTS_ERROR = 0;
    NRF_TWIM->EVENTS_TXSTARTED = 0;
    NRF_TWIM->TASKS_STARTTX = 1;
    while (!NRF_TWIM->EVENTS_TXSTARTED) {}

    while (1) {
        if (NRF_TWIM->EVENTS_STOPPED) {
            break;
        }
        if (NRF_TWIM->EVENTS_ERROR) {
            printf("TWIM Error\n");
            return 0;
        }
    }

    return 1;
}


int twim_write_one(uint8_t dev_address, uint8_t value)
{
    NRF_TWIM->ADDRESS = dev_address;

    NRF_TWIM->TXD.MAXCNT = 1;
    NRF_TWIM->TXD.PTR = (uint32_t)twimTxdBuffer;

    // *((uint32_t*)twimTxdBuffer) = reg_address;
    twimTxdBuffer[0] = value;

    NRF_TWIM->EVENTS_STOPPED = 0;
    NRF_TWIM->EVENTS_ERROR = 0;
    NRF_TWIM->EVENTS_TXSTARTED = 0;
    NRF_TWIM->TASKS_STARTTX = 1;
    while (!NRF_TWIM->EVENTS_TXSTARTED) {}

    while (1) {
        if (NRF_TWIM->EVENTS_STOPPED) {
            break;
        }
        if (NRF_TWIM->EVENTS_ERROR) {
            printf("TWIM Error\n");
            return 0;
        }
    }

    return 1;
}


uint8_t nxc_prevData[4] = {0,0,0,0};

void nxc_read_ctrl(uint8_t *data)
{
    uint8_t tmpData[8];
    twim_read(0x52, 0x00, 8, tmpData);
    for (int i=0;i<4;i++) data[i] = tmpData[4+i];
}

int N_nxc_init() {

    printf("N_nxc_init\n");

    twim_init();
    
    printf("Disable encryption 1\n");
    if (!twim_write_reg(0x52, 0xF0, 0x55)) return 0;
    nrf_delay_ms(10);
    printf("Disable encryption 2\n");
    if (!twim_write_reg(0x52, 0xFB, 0x00)) return 0;
    nrf_delay_ms(20);

    uint8_t idData[6];
    for (int i=0;i<6;i++) idData[i] = 0;
    if (twim_read(0x52, 0xFA, 6, idData)) {
        printf("  Controller ID: ");
        for (int i=0; i<6; i++) {
            printf("%.2x ", idData[i]);
        }
        printf("\n");
    }
    else {
        printf("ERROR\n");
        return 0;
    }

    nxc_read_ctrl(nxc_prevData);

    return 1;
}


void N_nxc_read() {

    // printf("N_nxc_read\n");

    uint8_t ctrlData[4];
    nxc_read_ctrl(ctrlData);
    // printf("%.2X %.2X\n", ctrlData[2], ctrlData[3]);
    for (int wi=0;wi<2;wi++) {
        uint8_t c = ctrlData[2+wi];
        uint8_t p = nxc_prevData[2+wi];
        if (c != p) {
            for (int bi=0;bi<8;bi++) {
                uint8_t cb, pb;
                cb = (c>>bi)&1;
                pb = (p>>bi)&1;
                uint8_t btnId = wi<<4 | bi;
                static event_t event;
                event.type = 0xFF;
                event.data2 = 0;
                event.data3 = 0;
                if (pb && !cb) {
                    event.type = ev_keydown;
                }
                if (cb && !pb) {
                    event.type = ev_keyup;
                }
                if (event.type != 0xFF) {
                    switch(btnId) {
                        case 0x01:
                            printf("R %d\n", event.type);
                            event.data1 = '.';
                            event.data2 = '.';
                            break;
                        case 0x02:
                            printf("Start %d\n", event.type);
                            event.data1 = KEY_ENTER;
                            break;
                        case 0x03:
                            printf("Reset %d\n", event.type);
                            event.data1 = KEY_ESCAPE;
                            break;
                        case 0x04:
                            printf("Select %d\n", event.type);
                            event.data1 = KEY_BACKSPACE;
                            break;
                        case 0x05:
                            printf("L %d\n", event.type);
                            event.data1 = ',';
                            event.data2 = ',';
                            break;
                        case 0x06:
                            printf("Down %d\n", event.type);
                            event.data1 = KEY_DOWNARROW;
                            break;
                        case 0x07:
                            printf("Right %d\n", event.type);
                            event.data1 = KEY_RIGHTARROW;
                            break;
                        case 0x10:
                            printf("Up %d\n", event.type);
                            event.data1 = KEY_UPARROW;
                            break;
                        case 0x11:
                            printf("Left %d\n", event.type);
                            event.data1 = KEY_LEFTARROW;
                            break;
                        case 0x13:
                            printf("X %d\n", event.type);
                            event.data1 = 'k';
                            event.data2 = 'k';
                            break;
                        case 0x14:
                            printf("A %d\n", event.type);
                            event.data1 = 'm';
                            event.data2 = 'm';
                            break;
                        case 0x15:
                            printf("Y %d\n", event.type);
                            event.data1 = 'h';
                            event.data2 = 'h';
                            break;
                        case 0x16:
                            printf("B %d\n", event.type);
                            event.data1 = 'j';
                            event.data2 = 'j';
                            break;
                        default:
                            continue;
                    }
                    D_PostEvent(&event);
                }
            }
        }
    }
    for (int i=0;i<4;i++) nxc_prevData[i] = ctrlData[i];
}

