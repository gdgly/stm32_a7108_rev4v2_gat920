/**
  *********************************************************************************************************
  * @file    sdiosdcarddemo.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   SD卡
  *********************************************************************************************************
  * @attention
  *		
  *
  *
  *
  *********************************************************************************************************
  */

#include "sdiosdcardfatfsdemo.h"
#include "sdiosdcarddemo.h"
#include "sdiosdcarduserapp.h"
#include "sdiosdcard.h"
#include "bsp_usart.h"
#include "ff.h"

FATFS		fs;
FATFS*		pfs = &fs;

FIL			fnew;
UINT			fnum;
FSIZE_t		fsize;

FRESULT		res_sd;
BYTE			work[FF_MAX_SS];

/**********************************************************************************************************
 @Function			void SD_SDIO_Fatfs_Demo(void)
 @Description			SD_SDIO_Fatfs_Demo									: SDIO SD Card Fatfs 测试
 @Input				void
 @Return				void
**********************************************************************************************************/
void SD_SDIO_Fatfs_Demo(void)
{
	res_sd = f_mount(&fs, "0:", 1);
	
	RS485_Mode_TX();
	printf("f_mount res = %d\r\n", res_sd);
	
	if (res_sd == FR_NO_FILESYSTEM) {
		printf("Initiative SD Card.\r\n");
		
		res_sd = f_mkfs("0:", FM_FAT32, 0, work, sizeof(work));
		printf("f_mkfs res = %d\r\n", res_sd);
		
		if (res_sd == FR_OK) {
			printf("Initiative SD Card OK.\r\n");
			res_sd = f_mount(NULL, "0:", 1);
			res_sd = f_mount(&fs, "0:", 1);
		}
	}
	
	DWORD fre_clust;
	
	res_sd = f_getfree("0:", &fre_clust, &pfs);
	printf("f_getfree res = %d\r\n", res_sd);
	
	u32 tot_sect, fre_sect;
	
	tot_sect = (pfs->n_fatent - 2) * pfs->csize;
	fre_sect = fre_clust * pfs->csize;
	printf("%10lu MiB total drive space.\n%10lu MiB available.\n", tot_sect / 2 / 1024, fre_sect / 2 / 1024);
	printf("%10lu GiB total drive space.\n%10lu GiB available.\n", tot_sect / 2 / 1024 / 1024, fre_sect / 2 / 1024 / 1024);
	
	printf("Open File:\r\n");
	
	res_sd = f_open(&fnew, "0:Test1.txt", FA_OPEN_APPEND | FA_WRITE | FA_READ);
	printf("f_open res = %d\r\n", res_sd);
	if (res_sd == FR_OK) {
		printf("Open File %s OK\r\n", "0:Test1.txt");
		
		res_sd = f_write(&fnew, "This is SD Card Test Demo. Write Data buffer.", strlen("This is SD Card Test Demo. Write Data buffer."), &fnum);
		printf("f_write res = %d\r\n", res_sd);
		if (res_sd == FR_OK) {
			f_sync(&fnew);
			printf("Write Data %s OK\r\n", "0:Test1.txt");
		}
		else {
			printf("Write Data %s Fail\r\n", "0:Test1.txt");
		}
		
		fsize = f_size(&fnew);
		printf("%s fszie = %d\r\n", "0:Test1.txt", (int)fsize);
		
		
		f_close(&fnew);
	}
	
	res_sd = f_mount(NULL, "0:", 1);
}

TCHAR buffer[64];
UINT  buflen;

TCHAR* bufstr = "SD Card Write Buffer Test!\n";

/**********************************************************************************************************
 @Function			void SDCard_Fatfs_APP_Client_Demo(void)
 @Description			SDCard_Fatfs_APP_Client_Demo
 @Input				void
 @Return				void
**********************************************************************************************************/
void SDCard_Fatfs_APP_Client_Demo(void)
{
	SDCard_StatusTypeDef SDCardStatus = SDCARD_OK;
	
	SDCard_Client_Init(&SDCardClient);
	
	SDCard_Client_RegistersFilesystem(&SDCardClient);
	
	SDCard_Client_GetFreeClusters(&SDCardClient);
	
	SDCardClient.FatfsDirectory[0].path = "LogDir";
	SDCardStatus = SDCard_Client_ChecksDirectoryorFile(&SDCardClient, &SDCardClient.FatfsDirectory[0]);
	if ((SDCARD_NO_PATH == SDCardStatus) || (SDCARD_NO_FILE == SDCardStatus)) {
		SDCard_Client_CreateDirectory(&SDCardClient, &SDCardClient.FatfsDirectory[0]);
		SDCard_Client_ChecksDirectoryorFile(&SDCardClient, &SDCardClient.FatfsDirectory[0]);
	}
	
	SDCardClient.FatfsDirectory[1].path = "DataDir";
	SDCardStatus = SDCard_Client_ChecksDirectoryorFile(&SDCardClient, &SDCardClient.FatfsDirectory[1]);
	if ((SDCARD_NO_PATH == SDCardStatus) || (SDCARD_NO_FILE == SDCardStatus)) {
		SDCard_Client_CreateDirectory(&SDCardClient, &SDCardClient.FatfsDirectory[1]);
		SDCard_Client_ChecksDirectoryorFile(&SDCardClient, &SDCardClient.FatfsDirectory[1]);
	}
	
#if 0
	SDCardClient.FatfsDirectory[0].finfo.fdate = (WORD)(((2018 - 1980) * 512U) | 5 * 32U | 20);
	SDCardClient.FatfsDirectory[0].finfo.ftime = (WORD)(12 * 2048U | 30 * 32U | 30 / 2U);
	SDCard_Client_ChangeTimestamp(&SDCardClient, &SDCardClient.FatfsDirectory[0]);
	SDCardClient.FatfsDirectory[1].finfo.fdate = (WORD)(((2017 - 1980) * 512U) | 10 * 32U | 1);
	SDCardClient.FatfsDirectory[1].finfo.ftime = (WORD)(19 * 2048U | 00 * 32U | 00 / 2U);
	SDCard_Client_ChangeTimestamp(&SDCardClient, &SDCardClient.FatfsDirectory[1]);
#endif
	
#if 0
	SDCard_Client_RemoveDirectoryorFile(&SDCardClient, &SDCardClient.FatfsDirectory[0]);
	SDCard_Client_RemoveDirectoryorFile(&SDCardClient, &SDCardClient.FatfsDirectory[1]);
#endif
	
	SDCardClient.FatfsFile[0].path = "LogDir/LOG1.log";
	SDCardClient.FatfsFile[0].fmode = FA_OPEN_APPEND | FA_WRITE | FA_READ;
	SDCardStatus = SDCard_Client_OpensFile(&SDCardClient, &SDCardClient.FatfsFile[0]);
	if (SDCARD_OK == SDCardStatus) {
		
		memset((void*)buffer, 0x00, sizeof(buffer));
		memcpy((void*)buffer, (const void*)bufstr, strlen(bufstr));
		buflen = strlen(bufstr);
		SDCardClient.FatfsFile[0].fbuf = buffer;
		SDCardClient.FatfsFile[0].flen = buflen;
		SDCardStatus = SDCard_Client_WriteFile(&SDCardClient, &SDCardClient.FatfsFile[0]);
		if (SDCARD_OK == SDCardStatus) {
			SDCard_Client_FlushesFile(&SDCardClient, &SDCardClient.FatfsFile[0]);
			SDCard_Client_ParameterFile(&SDCardClient, &SDCardClient.FatfsFile[0]);
			SDCardClient.FatfsFile[0].fnofs = 0;
			SDCard_Client_LseekFile(&SDCardClient, &SDCardClient.FatfsFile[0]);
			memset((void*)buffer, 0x00, sizeof(buffer));
			buflen = strlen(bufstr);
			SDCardClient.FatfsFile[0].fbuf = buffer;
			SDCardClient.FatfsFile[0].flen = buflen;
			SDCardStatus = SDCard_Client_ReadFile(&SDCardClient, &SDCardClient.FatfsFile[0]);
			if (SDCARD_OK == SDCardStatus) {
				printf("\r\n%d:%s\r\n", SDCardClient.FatfsFile[0].fcont, SDCardClient.FatfsFile[0].fbuf);
			}
#if 0
			SDCardClient.FatfsFile[0].fnofs = 2 * strlen(bufstr);
			SDCard_Client_LseekFile(&SDCardClient, &SDCardClient.FatfsFile[0]);
			SDCard_Client_TruncateFile(&SDCardClient, &SDCardClient.FatfsFile[0]);
			SDCard_Client_ParameterFile(&SDCardClient, &SDCardClient.FatfsFile[0]);
#endif
		}
		
		SDCard_Client_ClosesFile(&SDCardClient, &SDCardClient.FatfsFile[0]);
	}
	
	SDCardClient.FatfsFile[1].path = "LogDir/LOG2.log";
	SDCardClient.FatfsFile[1].fmode = FA_OPEN_APPEND | FA_WRITE | FA_READ;
	SDCardStatus = SDCard_Client_OpensFile(&SDCardClient, &SDCardClient.FatfsFile[1]);
	if (SDCARD_OK == SDCardStatus) {
		
		memset((void*)buffer, 0x00, sizeof(buffer));
		memcpy((void*)buffer, (const void*)bufstr, strlen(bufstr));
		buflen = strlen(bufstr);
		SDCardClient.FatfsFile[1].fbuf = buffer;
		SDCardClient.FatfsFile[1].flen = buflen;
		SDCardStatus = SDCard_Client_WriteFile(&SDCardClient, &SDCardClient.FatfsFile[1]);
		if (SDCARD_OK == SDCardStatus) {
			SDCard_Client_FlushesFile(&SDCardClient, &SDCardClient.FatfsFile[1]);
			SDCard_Client_ParameterFile(&SDCardClient, &SDCardClient.FatfsFile[1]);
			SDCardClient.FatfsFile[1].fnofs = 0;
			SDCard_Client_LseekFile(&SDCardClient, &SDCardClient.FatfsFile[1]);
			memset((void*)buffer, 0x00, sizeof(buffer));
			buflen = strlen(bufstr);
			SDCardClient.FatfsFile[1].fbuf = buffer;
			SDCardClient.FatfsFile[1].flen = buflen;
			SDCardStatus = SDCard_Client_ReadFile(&SDCardClient, &SDCardClient.FatfsFile[1]);
			if (SDCARD_OK == SDCardStatus) {
				printf("\r\n%d:%s\r\n", SDCardClient.FatfsFile[1].fcont, SDCardClient.FatfsFile[1].fbuf);
			}
#if 0
			SDCardClient.FatfsFile[1].fnofs = 2 * strlen(bufstr);
			SDCard_Client_LseekFile(&SDCardClient, &SDCardClient.FatfsFile[1]);
			SDCard_Client_TruncateFile(&SDCardClient, &SDCardClient.FatfsFile[1]);
			SDCard_Client_ParameterFile(&SDCardClient, &SDCardClient.FatfsFile[1]);
#endif
		}
		
		SDCard_Client_ClosesFile(&SDCardClient, &SDCardClient.FatfsFile[1]);
	}
	
	SDCardClient.FatfsFile[0].path = "DataDir/DT1.log";
	SDCardClient.FatfsFile[0].fmode = FA_OPEN_APPEND | FA_WRITE | FA_READ;
	SDCardStatus = SDCard_Client_OpensFile(&SDCardClient, &SDCardClient.FatfsFile[0]);
	if (SDCARD_OK == SDCardStatus) {
		
		SDCardClient.FatfsFile[0].fchr = 'O';
		SDCardStatus = SDCard_Client_PutchrFile(&SDCardClient, &SDCardClient.FatfsFile[0]);
		SDCardClient.FatfsFile[0].fbuf = bufstr;
		SDCardClient.FatfsFile[0].flen = strlen(bufstr);
		SDCardStatus = SDCard_Client_PutstrFile(&SDCardClient, &SDCardClient.FatfsFile[0]);
		if (SDCARD_OK == SDCardStatus) {
			SDCard_Client_FlushesFile(&SDCardClient, &SDCardClient.FatfsFile[0]);
			SDCard_Client_ParameterFile(&SDCardClient, &SDCardClient.FatfsFile[0]);
			SDCardClient.FatfsFile[0].fnofs = 0;
			SDCard_Client_LseekFile(&SDCardClient, &SDCardClient.FatfsFile[0]);
			SDCardClient.FatfsFile[0].fbuf = buffer;
			SDCardClient.FatfsFile[0].flen = sizeof(buffer);
			SDCardStatus = SDCard_Client_GetstrFile(&SDCardClient, &SDCardClient.FatfsFile[0]);
			if (SDCARD_OK == SDCardStatus) {
				printf("\r\n%d:%s\r\n", SDCardClient.FatfsFile[0].fcont, SDCardClient.FatfsFile[0].fbuf);
			}
			SDCard_Client_ParameterFile(&SDCardClient, &SDCardClient.FatfsFile[0]);
			SDCardClient.FatfsFile[0].fnofs = SDCardClient.FatfsFile[0].fsize;
			SDCard_Client_LseekFile(&SDCardClient, &SDCardClient.FatfsFile[0]);
#if 1
			f_printf(&SDCardClient.FatfsFile[0].fp, "SD Card Printf %d\n", 1024);
#endif
			
			
			
		}
		
		SDCard_Client_ClosesFile(&SDCardClient, &SDCardClient.FatfsFile[0]);
	}
	
	SDCardClient.FatfsFile[1].path = "DataDir/DT2.log";
	SDCardClient.FatfsFile[1].fmode = FA_OPEN_APPEND | FA_WRITE | FA_READ;
	SDCardStatus = SDCard_Client_OpensFile(&SDCardClient, &SDCardClient.FatfsFile[1]);
	if (SDCARD_OK == SDCardStatus) {
		
		SDCardClient.FatfsFile[1].fchr = 'B';
		SDCardStatus = SDCard_Client_PutchrFile(&SDCardClient, &SDCardClient.FatfsFile[1]);
		SDCardClient.FatfsFile[1].fbuf = bufstr;
		SDCardClient.FatfsFile[1].flen = strlen(bufstr);
		SDCardStatus = SDCard_Client_PutstrFile(&SDCardClient, &SDCardClient.FatfsFile[1]);
		if (SDCARD_OK == SDCardStatus) {
			SDCard_Client_FlushesFile(&SDCardClient, &SDCardClient.FatfsFile[1]);
			SDCard_Client_ParameterFile(&SDCardClient, &SDCardClient.FatfsFile[1]);
			SDCardClient.FatfsFile[1].fnofs = 0;
			SDCard_Client_LseekFile(&SDCardClient, &SDCardClient.FatfsFile[1]);
			SDCardClient.FatfsFile[1].fbuf = buffer;
			SDCardClient.FatfsFile[1].flen = sizeof(buffer);
			SDCardStatus = SDCard_Client_GetstrFile(&SDCardClient, &SDCardClient.FatfsFile[1]);
			if (SDCARD_OK == SDCardStatus) {
				printf("\r\n%d:%s\r\n", SDCardClient.FatfsFile[1].fcont, SDCardClient.FatfsFile[1].fbuf);
			}
			SDCard_Client_ParameterFile(&SDCardClient, &SDCardClient.FatfsFile[1]);
			SDCardClient.FatfsFile[1].fnofs = SDCardClient.FatfsFile[1].fsize;
			SDCard_Client_LseekFile(&SDCardClient, &SDCardClient.FatfsFile[1]);
#if 1
			f_printf(&SDCardClient.FatfsFile[1].fp, "SD Card Printf %s\n", "AABB");
#endif
			
			
			
		}
		
		SDCard_Client_ClosesFile(&SDCardClient, &SDCardClient.FatfsFile[1]);
	}
	
	SDCard_Client_UnregistersFilesystem(&SDCardClient);
}

/********************************************** END OF FLEE **********************************************/
