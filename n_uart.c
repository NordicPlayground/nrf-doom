
#include "doomkeys.h"
#include "d_event.h"

#include <stdio.h>

// in main.c for now
boolean uart_has_data();
char uart_getc();

void N_ReadUart() 
{
    static event_t event;
    boolean hd = uart_has_data();
    if (hd) {
        char code = uart_getc();
        if (code == 'D') {
            char keycode = uart_getc();
            char keychar = uart_getc();
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
            char keycode = uart_getc();
            char keychar = uart_getc();
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
            printf("ERROR in N_ReadUart: Unhandled code %c\n", code);
        }
    }
}