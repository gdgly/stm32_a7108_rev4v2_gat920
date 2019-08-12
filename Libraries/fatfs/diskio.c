/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"			/* Declarations of disk functions */
#include "rtc.h"
#include "sdiosdcard.h"
#include "sdiosdcardfatfsinterface.h"

/* Definitions of physical drive number for each drive */

#if 0
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */
#endif

#define SD_CARD		0	/* Example: Map SD Card to physical drive 0 */
#define EX_FLASH		1	/* Example: Map Ex FLASH to physical drive 1 */
#define EX_NAND		2	/* Example: Map Ex NAND to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case SD_CARD :
		result = SDIO_SDCard_Interface_disk_status();

		// translate the reslut code here
		if (!result) {
			stat = RES_OK;
		}
		else {
			stat = STA_NOINIT;
		}

		return stat;

	case EX_FLASH :
		result = STA_NOINIT;

		// translate the reslut code here
		stat = result;

		return stat;

	case EX_NAND :
		result = STA_NOINIT;

		// translate the reslut code here
		stat = result;

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case SD_CARD :
		result = SDIO_SDCard_Interface_disk_initialize();

		// translate the reslut code here
		if (!result) {
			stat = RES_OK;
		}
		else {
			stat = STA_NOINIT;
		}

		return stat;

	case EX_FLASH :
		result = STA_NOINIT;

		// translate the reslut code here
		stat = result;

		return stat;

	case EX_NAND :
		result = STA_NOINIT;

		// translate the reslut code here
		stat = result;

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	BYTE *buff,			/* Data buffer to store read data */
	DWORD sector,			/* Start sector in LBA */
	UINT count			/* Number of sectors to read */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case SD_CARD :
		// translate the arguments here

		result = SDIO_SDCard_Interface_disk_read(buff, sector, count);
		while (result) {
			SDIO_SDCard_Interface_disk_initialize();
			result = SDIO_SDCard_Interface_disk_read(buff, sector, count);
		}

		// translate the reslut code here
		if (!result) {
			res = (DRESULT)RES_OK;
		}
		else {
			res = (DRESULT)RES_ERROR;
		}

		return res;

	case EX_FLASH :
		// translate the arguments here

		result = RES_NOTRDY;

		// translate the reslut code here
		res = (DRESULT)result;

		return res;

	case EX_NAND :
		// translate the arguments here

		result = RES_NOTRDY;

		// translate the reslut code here
		res = (DRESULT)result;

		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,		/* Data to be written */
	DWORD sector,			/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case SD_CARD :
		// translate the arguments here

		result = SDIO_SDCard_Interface_disk_write(buff, sector, count);
		while (result) {
			SDIO_SDCard_Interface_disk_initialize();
			result = SDIO_SDCard_Interface_disk_write(buff, sector, count);
		}

		// translate the reslut code here
		if (!result) {
			res = (DRESULT)RES_OK;
		}
		else {
			res = (DRESULT)RES_ERROR;
		}

		return res;

	case EX_FLASH :
		// translate the arguments here

		result = RES_NOTRDY;

		// translate the reslut code here
		res = (DRESULT)result;

		return res;

	case EX_NAND :
		// translate the arguments here

		result = RES_NOTRDY;

		// translate the reslut code here
		res = (DRESULT)result;

		return res;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	BYTE cmd,				/* Control code */
	void *buff			/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case SD_CARD :

		// Process of the command for the RAM drive
		result = SDIO_SDCard_Interface_disk_ioctl(cmd, buff);
		if (!result) {
			res = (DRESULT)RES_OK;
		}
		else {
			res = (DRESULT)RES_PARERR;
		}
		

		return res;

	case EX_FLASH :

		// Process of the command for the MMC/SD card
		result = RES_NOTRDY;
		res = (DRESULT)result;

		return res;

	case EX_NAND :

		// Process of the command the USB drive
		result = RES_NOTRDY;
		res = (DRESULT)result;

		return res;
	}

	return RES_PARERR;
}

/**********************************************************************************************************
 @Function			DWORD get_fattime (void)
 @Description			get_fattime
 @Input				void
 @Return				DWORD
**********************************************************************************************************/
DWORD get_fattime (void)
{
	u8 year = 0, mon = 0, day = 0, hous = 0, min = 0, sec = 0;
	
	DWORD ffattime = 0;
	
	RTC_Time_GetTime(&year, &mon, &day, &hous, &min, &sec);
	
	ffattime = ((DWORD)(year+2000-1980) << 25 | (DWORD)mon << 21 | (DWORD)day << 16 | (DWORD)hous << 11 | (DWORD)min << 5 | (DWORD)sec >> 1);
	
	return ffattime;
}

/********************************************** END OF FLEE **********************************************/
