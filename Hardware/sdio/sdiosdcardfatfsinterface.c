/**
  *********************************************************************************************************
  * @file    sdiosdcardfatfsinterface.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   
  *********************************************************************************************************
  * @attention
  *		
  *
  *
  *
  *********************************************************************************************************
  */

#include "sdiosdcardfatfsinterface.h"
#include "sdiosdcard.h"
#include "stm32_config.h"
#include "bsp_usart.h"

u8  Sdcard_Systime_Time_Meter		= 0;
u32 Sdcard_Runtime_Time_MS		= 0;
u32 Sdcard_Flgtime_Time_MS		= 500;

#define BLOCK_SIZE				512										/* Block Size in Bytes */

/* Command code for disk_ioctrl fucntion */

/* Generic command (Used by FatFs) */
#define CTRL_SYNC			0	/* Complete pending write process (needed at FF_FS_READONLY == 0) */
#define GET_SECTOR_COUNT		1	/* Get media size (needed at FF_USE_MKFS == 1) */
#define GET_SECTOR_SIZE		2	/* Get sector size (needed at FF_MAX_SS != FF_MIN_SS) */
#define GET_BLOCK_SIZE		3	/* Get erase block size (needed at FF_USE_MKFS == 1) */
#define CTRL_TRIM			4	/* Inform device that the data on the block of sectors is no longer used (needed at FF_USE_TRIM == 1) */

/* Generic command (Not used by FatFs) */
#define CTRL_POWER			5	/* Get/Set power status */
#define CTRL_LOCK			6	/* Lock/Unlock media removal */
#define CTRL_EJECT			7	/* Eject media */
#define CTRL_FORMAT			8	/* Create physical format on the media */

/* MMC/SDC specific ioctl command */
#define MMC_GET_TYPE		10	/* Get card type */
#define MMC_GET_CSD			11	/* Get CSD */
#define MMC_GET_CID			12	/* Get CID */
#define MMC_GET_OCR			13	/* Get OCR */
#define MMC_GET_SDSTAT		14	/* Get SD status */
#define ISDIO_READ			55	/* Read data form SD iSDIO register */
#define ISDIO_WRITE			56	/* Write data to SD iSDIO register */
#define ISDIO_MRITE			57	/* Masked write data to SD iSDIO register */

/* ATA/CF specific ioctl command */
#define ATA_GET_REV			20	/* Get F/W revision */
#define ATA_GET_MODEL		21	/* Get model name */
#define ATA_GET_SN			22	/* Get serial number */

/**********************************************************************************************************
 @Function			int SDIO_SDCard_Interface_disk_status(void)
 @Description			SDIO_SDCard_Interface_disk_status
 @Input				void
 @Return				int
**********************************************************************************************************/
int SDIO_SDCard_Interface_disk_status(void)
{
	return 0;
}

/**********************************************************************************************************
 @Function			int SDIO_SDCard_Interface_disk_initialize(void)
 @Description			SDIO_SDCard_Interface_disk_initialize
 @Input				void
 @Return				int
**********************************************************************************************************/
int SDIO_SDCard_Interface_disk_initialize(void)
{
	SD_Error Status = SD_OK;
	
	Status = SD_Init();
	
	if (Status == SD_OK) {
		return 0;
	}
	else {
		return 1;
	}
}

/**********************************************************************************************************
 @Function			int SDIO_SDCard_Interface_disk_read(unsigned char* buff, uint64_t sector, unsigned int count)
 @Description			SDIO_SDCard_Interface_disk_read
 @Input				buff
					sector
					count
 @Return				int
**********************************************************************************************************/
int SDIO_SDCard_Interface_disk_read(unsigned char* buff, uint64_t sector, unsigned int count)
{
	SD_Error Status = SD_OK;
	
	if (count > 1) {
		Status = SD_ReadMultiBlocks(buff, sector * BLOCK_SIZE, BLOCK_SIZE, count);
		if (Status != SD_OK) return 1;
		
		/* Check if the Transfer is finished */
		Status = SD_WaitReadOperation();
		if (Status != SD_OK) return 1;
		
		/* Wait until end of DMA transfer */
		Sdcard_Systime_Time_Meter = 1;
		Sdcard_Runtime_Time_MS    = 0;
		while (SD_GetStatus() != SD_TRANSFER_OK) {
#ifdef SOFTWAREIWDG
			IWDG_ReloadCounter();
#endif
#ifdef HARDWAREIWDG
			MAX823_IWDGReloadCounter();
#endif
			if (Sdcard_Runtime_Time_MS > Sdcard_Flgtime_Time_MS) {
				Sdcard_Systime_Time_Meter = 0;
				return 1;
			}
		}
		Sdcard_Systime_Time_Meter = 0;
	}
	else {
		Status = SD_ReadBlock(buff, sector * BLOCK_SIZE, BLOCK_SIZE);
		if (Status != SD_OK) return 1;
		
		/* Check if the Transfer is finished */
		Status = SD_WaitReadOperation();
		if (Status != SD_OK) return 1;
		
		/* Wait until end of DMA transfer */
		Sdcard_Systime_Time_Meter = 1;
		Sdcard_Runtime_Time_MS    = 0;
		while (SD_GetStatus() != SD_TRANSFER_OK) {
#ifdef SOFTWAREIWDG
			IWDG_ReloadCounter();
#endif
#ifdef HARDWAREIWDG
			MAX823_IWDGReloadCounter();
#endif
			if (Sdcard_Runtime_Time_MS > Sdcard_Flgtime_Time_MS) {
				Sdcard_Systime_Time_Meter = 0;
				return 1;
			}
		}
		Sdcard_Systime_Time_Meter = 0;
	}
	
	return 0;
}

/**********************************************************************************************************
 @Function			int SDIO_SDCard_Interface_disk_write(const unsigned char* buff, uint64_t sector, unsigned int count)
 @Description			SDIO_SDCard_Interface_disk_write
 @Input				buff
					sector
					count
 @Return				int
**********************************************************************************************************/
int SDIO_SDCard_Interface_disk_write(const unsigned char* buff, uint64_t sector, unsigned int count)
{
	SD_Error Status = SD_OK;
	
	if (count > 1) {
		Status = SD_WriteMultiBlocks((uint8_t *)buff, sector * BLOCK_SIZE, BLOCK_SIZE, count);
		if (Status != SD_OK) return 1;
		
		/* Check if the Transfer is finished */
		Status = SD_WaitWriteOperation();
		if (Status != SD_OK) return 1;
		
		/* Wait until end of DMA transfer */
		Sdcard_Systime_Time_Meter = 1;
		Sdcard_Runtime_Time_MS    = 0;
		while (SD_GetStatus() != SD_TRANSFER_OK) {
#ifdef SOFTWAREIWDG
			IWDG_ReloadCounter();
#endif
#ifdef HARDWAREIWDG
			MAX823_IWDGReloadCounter();
#endif
			if (Sdcard_Runtime_Time_MS > Sdcard_Flgtime_Time_MS) {
				Sdcard_Systime_Time_Meter = 0;
				return 1;
			}
		}
		Sdcard_Systime_Time_Meter = 0;
	}
	else {
		Status = SD_WriteBlock((uint8_t *)buff, sector * BLOCK_SIZE, BLOCK_SIZE);
		if (Status != SD_OK) return 1;
		
		/* Check if the Transfer is finished */
		Status = SD_WaitWriteOperation();
		if (Status != SD_OK) return 1;
		
		/* Wait until end of DMA transfer */
		Sdcard_Systime_Time_Meter = 1;
		Sdcard_Runtime_Time_MS    = 0;
		while (SD_GetStatus() != SD_TRANSFER_OK) {
#ifdef SOFTWAREIWDG
			IWDG_ReloadCounter();
#endif
#ifdef HARDWAREIWDG
			MAX823_IWDGReloadCounter();
#endif
			if (Sdcard_Runtime_Time_MS > Sdcard_Flgtime_Time_MS) {
				Sdcard_Systime_Time_Meter = 0;
				return 1;
			}
		}
		Sdcard_Systime_Time_Meter = 0;
	}
	
	return 0;
}

/**********************************************************************************************************
 @Function			int SDIO_SDCard_Interface_disk_ioctl(unsigned char cmd, void* buff)
 @Description			SDIO_SDCard_Interface_disk_ioctl
 @Input				cmd
					buff
 @Return				int
**********************************************************************************************************/
int SDIO_SDCard_Interface_disk_ioctl(unsigned char cmd, void* buff)
{
	int res = 0;
	
	switch (cmd) {
		case CTRL_SYNC:
			res = 0;
			break;
		
		case GET_SECTOR_COUNT:
			*(uint64_t*)buff = SDCardInfo.CardCapacity / 512;
			res = 0;
			break;
		
		case GET_SECTOR_SIZE:
			*(uint64_t*)buff = BLOCK_SIZE;
			res = 0;
			break;
		
		case GET_BLOCK_SIZE:
			*(uint64_t*)buff = SDCardInfo.CardBlockSize;
			res = 0;
			break;
		
		default:
			res = 4;
			break;
	}
	
	return res;
}

/********************************************** END OF FLEE **********************************************/
