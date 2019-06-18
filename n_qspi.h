
#include <stdlib.h>

#define N_QSPI_XIP_START_ADDR      0x12000000

#define N_QSPI_BLOCK_SIZE (64*1024)


void *N_qspi_data_pointer(size_t loc);
void N_qspi_wait();
void N_qspi_init();
void N_qspi_erase_block(size_t loc) ;
void N_qspi_write(size_t loc, void *buffer, size_t size) ;
void N_qspi_write_block(size_t loc, void *buffer, size_t size);
void N_qspi_reserve_blocks(size_t block_count);
size_t N_qspi_alloc_block();