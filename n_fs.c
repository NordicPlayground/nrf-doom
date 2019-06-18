

#include "nrf.h"
#include "bsp.h"
#include "ff.h"
#include "diskio_blkdev.h"
#include "nrf_block_dev_sdc.h"

#ifdef N_FS_LOG
    #include "nrf_log.h"
    #include "nrf_log_ctrl.h"
    #include "nrf_log_default_backends.h"
#else
    #define NRF_LOG_INFO(...)
    #define NRF_LOG_ERROR(...)
#endif

#include "n_mem.h"

#define SDC_SCK_PIN     ARDUINO_13_PIN  ///< SDC serial clock (SCK) pin.
#define SDC_MOSI_PIN    ARDUINO_11_PIN  ///< SDC serial data in (DI) pin.
#define SDC_MISO_PIN    ARDUINO_12_PIN  ///< SDC serial data out (DO) pin.
#define SDC_CS_PIN      ARDUINO_10_PIN  ///< SDC chip select (CS) pin.

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

    printf("\r\n Listing directory: /\n");
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
                // printf("%9lu  %s", fno.fsize, (uint32_t)fno.fname);
            }
        }
    }
    while (fno.fname[0]);
    printf("----");
    return;
}

void N_fs_shutdown()
{
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
            NRF_LOG_ERROR("N_fs_file_exists");
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
            NRF_LOG_ERROR("N_fs_file_length");
    }
    return -1;
}
