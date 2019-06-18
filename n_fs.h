
#include "ff.h"

typedef FIL* N_FILE;

void N_fs_init();
void N_fs_shutdown();

FIL *N_fs_open(char *path);
static void N_fs_close(FIL *fstream);
FSIZE_t N_fs_size(FIL *fstream);
size_t N_fs_read(FIL *fstream, unsigned int offset, void *buffer, size_t buffer_len);

int N_fs_file_exists(char *path);
long N_fs_file_size(char *path);