
#include "sdk_common.h"

#include <stdio.h>
#include <stdint.h>

void uart_putc(char ch); // in main.c for now

#if defined(__CC_ARM)

// This part is taken from MDK-ARM template file and is required here to prevent
// linker from selecting libraries functions that use semihosting and failing
// because of multiple definitions of fgetc() and fputc().
// Refer to: http://www.keil.com/support/man/docs/gsac/gsac_retargetcortex.htm
// -- BEGIN --
struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;
// --- END ---

int fgetc(FILE * p_file)
{
    uint8_t input;
    // NRFD_TODO?
    // while (app_uart_get(&input) == NRF_ERROR_NOT_FOUND)
    // {
    //     // No implementation needed.
    // }
    return input;
}

int fputc(int ch, FILE * p_file)
{
    UNUSED_PARAMETER(p_file);
    uart_putc(ch);
    return ch;
}

#elif defined(__GNUC__) && defined(__SES_ARM)

int __getchar(FILE * p_file)
{
    uint8_t input;
    // NRFD-TODO?
    // while (app_uart_get(&input) == NRF_ERROR_NOT_FOUND)
    // {
    //     // No implementation needed.
    // }
    return input;
}

#if defined(__SES_VERSION) && (__SES_VERSION >= 34000)
int __putchar(int ch, __printf_tag_ptr tag_ptr)
{
    UNUSED_PARAMETER(tag_ptr);
    uart_putc(ch);
    return ch;
}
#else
int __putchar(int ch, FILE * p_file)
{
    UNUSED_PARAMETER(p_file);
    uart_putc(ch);
    return ch;
}
#endif

#elif defined(__GNUC__) && !defined(__SES_ARM)

int _write(int file, const char * p_char, int len)
{
    int i;

    UNUSED_PARAMETER(file);
    for (i = 0; i < len; i++)
    {
        uart_putc(*p_char++);
    }
    return len;
}

int _read(int file, char * p_char, int len)
{
    UNUSED_PARAMETER(file);
    // NRFD-TODO?
    // while (app_uart_get((uint8_t *)p_char) == NRF_ERROR_NOT_FOUND)
    // {
    //     // No implementation needed.
    // }
    return 1;
}
#elif defined(__ICCARM__)

size_t __write(int handle, const unsigned char * buffer, size_t size)
{
    int i;
    UNUSED_PARAMETER(handle);
    for (i = 0; i < size; i++)
    {
        uart_putc(*buffer++);
    }
    return size;
}

size_t __read(int handle, unsigned char * buffer, size_t size)
{
    UNUSED_PARAMETER(handle);
    UNUSED_PARAMETER(size);
    // NRFD-TODO?
    // while (app_uart_get((uint8_t *)buffer) == NRF_ERROR_NOT_FOUND)
    // {
    //     // No implementation needed.
    // }
    return 1;
}

long __lseek(int handle, long offset, int whence)
{
    return -1;
}
int __close(int handle)
{
    return 0;
}
int remove(const char * filename)
{
    return 0;
}

#endif
