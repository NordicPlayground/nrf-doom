
#include <doomtype.h>

void N_ReadUart();
void N_uart_init(void);
void N_uart_putc(char ch);
boolean N_uart_has_data();
char N_uart_getc();
