
#include <stdint.h>
#include <doomtype.h>

void N_I2S_init();
boolean N_I2S_next_buffer(int *buf_size, int16_t **buffer);
void N_I2S_process();

