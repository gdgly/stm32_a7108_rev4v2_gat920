#ifndef __SDIO_SDCARD_FATFS_INTERFACE_H
#define   __SDIO_SDCARD_FATFS_INTERFACE_H

#include "stm32f10x_lib.h"
#include "platform_config.h"
#include "stdint.h"

int SDIO_SDCard_Interface_disk_status(void);
int SDIO_SDCard_Interface_disk_initialize(void);
int SDIO_SDCard_Interface_disk_read(unsigned char* buff, uint64_t sector, unsigned int count);
int SDIO_SDCard_Interface_disk_write(const unsigned char* buff, uint64_t sector, unsigned int count);
int SDIO_SDCard_Interface_disk_ioctl(unsigned char cmd, void* buff);

#endif
