/*
 * Copyright (c) 2019 - 2020, Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */



#include "nrf.h"
#include "board_config.h"

#include "ff.h"
#include "diskio_blkdev.h"
#include "nrf_block_dev_sdc.h"


#include "n_mem.h"

void I_Error (char *error, ...);

/**
 * @brief  SDC block device definition
 * */
NRF_BLOCK_DEV_SDC_DEFINE(
        m_block_dev_sdc,
        NRF_BLOCK_DEV_SDC_CONFIG(
                SDC_SECTOR_SIZE,
                APP_SDCARD_CONFIG(SDC_MOSI_PIN, SDC_MISO_PIN, SDC_SCK_PIN, SDC_CS_PIN)
         ),
         NFR_BLOCK_DEV_INFO_CONFIG("Nordic", "SDC", "1.00")
);


FATFS *fs = NULL;

void N_fs_init()
{
    DIR dir;
    FILINFO fno;
    FIL file;

    fs = N_malloc(sizeof(FATFS));

    uint32_t bytes_written;
    FRESULT ff_result;
    DSTATUS disk_state = STA_NOINIT;

    // Initialize FATFS disk I/O interface by providing the block device.
    static diskio_blkdev_t drives[] =
    {
            DISKIO_BLOCKDEV_CONFIG(NRF_BLOCKDEV_BASE_ADDR(m_block_dev_sdc, block_dev), NULL)
    };

    diskio_blockdev_register(drives, ARRAY_SIZE(drives));

    printf("Initializing disk 0 (SDC)...\n");
    for (uint32_t retries = 3; retries && disk_state; --retries)
    {
        disk_state = disk_initialize(0);
    }
    if (disk_state)
    {
        printf("Disk initialization failed.\n");
        return;
    }

    uint32_t blocks_per_mb = (1024uL * 1024uL) / m_block_dev_sdc.block_dev.p_ops->geometry(&m_block_dev_sdc.block_dev)->blk_size;
    uint32_t capacity = m_block_dev_sdc.block_dev.p_ops->geometry(&m_block_dev_sdc.block_dev)->blk_count / blocks_per_mb;
    printf("Capacity: %ld MB\n", capacity);

    printf("Mounting volume...\n");
    ff_result = f_mount(fs, "", 1);
    if (ff_result)
    {
        printf("Mount failed.\n");
        return;
    }

    printf("Listing directory: /\n");
    ff_result = f_opendir(&dir, "/");
    if (ff_result)
    {
        printf("Directory listing failed!\n");
        return;
    }

    do
    {
        ff_result = f_readdir(&dir, &fno);
        if (ff_result != FR_OK)
        {
            printf("Directory read failed.\n");
            return;
        }

        if (fno.fname[0])
        {
            if (fno.fattrib & AM_DIR)
            {
                // printf("   <DIR>   %s",(uint32_t)fno.fname);
            }
            else
            {
                printf("%9lu  %s\n", fno.fsize, fno.fname);
            }
        }
    }
    while (fno.fname[0]);
    printf("----\n \n");
    return;
}

void N_fs_shutdown()
{
    printf("N_fs_shutdown\n");
    disk_uninitialize(0);
    N_free(fs);
    fs = NULL;
}

FIL *N_fs_open(char *path)
{
    if (!fs) I_Error("N_fs_open: fs not iniitialized\n");
    printf("FatFS: Opening: %s\n", path);
    FIL *fstream = N_malloc(sizeof(FIL));
    FRESULT ff_result;

    ff_result = f_open(fstream, path, FA_READ);
    if (ff_result != FR_OK)
    {
        return NULL;
    }

    printf("FatFS: Open OK\n");

    return fstream;
}

FSIZE_t N_fs_size(FIL *fstream)
{
    if (!fs) I_Error("N_fs_open: fs not iniitialized\n");
    return f_size(fstream);
}

static void N_fs_close(FIL *fstream)
{
    if (!fs) I_Error("N_fs_open: fs not iniitialized\n");
    (void) f_close(fstream);

    N_free(fstream);
}

// Read data from the specified position in the file into the 
// provided buffer.  Returns the number of bytes read.

size_t N_fs_read(FIL *fstream, unsigned int offset, void *buffer, size_t buffer_len)
{
    if (!fs) I_Error("N_fs_open: fs not iniitialized\n");
    size_t result;
    FRESULT ff_result;

    // Jump to the specified position in the file.

    ff_result = f_lseek(fstream, offset);
    if (ff_result != FR_OK)
    {
        printf("W_FatFS_Read: seek failed\n");
        return 0;
    }
    // Read into the buffer.

    ff_result = f_read(fstream, buffer, buffer_len, &result);
    if (ff_result != FR_OK)
    {
        printf("W_FatFS_Read: seek failed\n");
        return 0;
    }

    return result;
}


int N_fs_file_exists(char *path) {
    if (!fs) I_Error("N_fs_open: fs not iniitialized\n");

    FRESULT fr;
    FILINFO fno;

    fr = f_stat(path, &fno);
    switch (fr) {
        case FR_OK:
            return 1;
            break;

        case FR_NO_FILE:
            return 0;
            break;

        default:
            I_Error("N_fs_file_exists");
    }
    return 0;
}


long N_fs_file_length(char *path) {
    if (!fs) I_Error("N_fs_open: fs not iniitialized\n");

    FRESULT fr;
    FILINFO fno;

    fr = f_stat(path, &fno);
    switch (fr) {
        case FR_OK:
            return fno.fsize;
            break;

        case FR_NO_FILE:
            return 01;
            break;

        default:
            I_Error("N_fs_file_length");
    }
    return -1;
}
