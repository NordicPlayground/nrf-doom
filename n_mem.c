
#include <stdlib.h>
#include <stdio.h>


void *N_malloc(size_t size)
{
    void *result;
    result = malloc(size);
    size_t end = (size_t)result + size;
    if (end > 0x2003e000) { // NRFD-TODO: Get actual heap limit
        printf("Heap Overflow!!\n");
        return NULL;
    }
    printf("A %d at %X - %X\n", size, (size_t)result, end);
    return result;
}