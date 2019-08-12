/**
  *********************************************************************************************************
  * @file    sdiosdcardapp.c
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

#include "sdiosdcardapp.h"
#include "sdiosdcarduserapp.h"
#include "sdiosdcard.h"
#include "sdiosdcardmessage.h"
#include "socketmessage.h"
#include "socketinitialization.h"
#include "bsp_usart.h"
#include "stm32_config.h"
#include "string.h"
#include "ff.h"
#include "rtc.h"

#define SDCARD_LOGFILE_WRITE				0
#define SDCARD_DATFILE_WRITE				0

#define SDCARD_DATFILE_INSDCARD			0
#define SDCARD_DATFILE_OUSDCARD			0

extern u32 Sdcard_Flgtime_Time_MS;

bool Sdcard_DataFile_Normal_Existence = true;

unsigned char fileyear, filemon, fileday, filehous, filemin, filesec;
unsigned char filepath[30];

SdcardDataHeadTypeDef	SdcardDataHead;

/**********************************************************************************************************
 @Function			void SDCard_APP_Initialized(void)
 @Description			SDCard_APP_Initialized
 @Input				void
 @Return				void
**********************************************************************************************************/
void SDCard_APP_Initialized(void)
{
	SDCard_StatusTypeDef SDCardStatus = SDCARD_OK;
	
#ifdef SDCARD_LED1
	SDCARD_LED1_ON();
#endif
	
	SDCard_Client_Init(&SDCardClient);
	
	Sdcard_Flgtime_Time_MS = 10000;
	
	/* 文件系统注册 */
	if ((SDCardStatus = SDCard_Client_RegistersFilesystem(&SDCardClient)) == SDCARD_OK) {
		SDCardClient.DictateRunCtl.dictateRegistersFilesystem = true;
	}
	else {
		SDCardClient.DictateRunCtl.dictateRegistersFilesystem = false;
		return;
	}
	
	/* 文件系统获取SDCard参数信息 */
	SDCardStatus = SDCard_Client_GetFreeClusters(&SDCardClient);
	
	/* 文件系统检查指定文件夹是否存在,不存在则创建文件夹并获取文件夹信息 */
	SDCardClient.FatfsDirectory[FATFS_DIRECTORY_LOGDIR_INDEX].path	= FATFS_DIRECTORY_LOGDIR_PATH;
	SDCardStatus = SDCard_Client_ChecksDirectoryorFile(&SDCardClient, &SDCardClient.FatfsDirectory[FATFS_DIRECTORY_LOGDIR_INDEX]);
	if ((SDCARD_NO_PATH == SDCardStatus) || (SDCARD_NO_FILE == SDCardStatus)) {
		SDCardStatus = SDCard_Client_CreateDirectory(&SDCardClient, &SDCardClient.FatfsDirectory[FATFS_DIRECTORY_LOGDIR_INDEX]);
		if (SDCARD_OK == SDCardStatus) {
			SDCardStatus = SDCard_Client_ChecksDirectoryorFile(&SDCardClient, &SDCardClient.FatfsDirectory[FATFS_DIRECTORY_LOGDIR_INDEX]);
			if (SDCARD_OK == SDCardStatus) SDCardClient.DictateRunCtl.dictateCreateDirectoryLOGDIR = true;
		}
	}
	else if (SDCARD_OK == SDCardStatus) {
		SDCardClient.DictateRunCtl.dictateCreateDirectoryLOGDIR = true;
	}
	
	SDCardClient.FatfsDirectory[FATFS_DIRECTORY_DATDIR_INDEX].path	= FATFS_DIRECTORY_DATDIR_PATH;
	SDCardStatus = SDCard_Client_ChecksDirectoryorFile(&SDCardClient, &SDCardClient.FatfsDirectory[FATFS_DIRECTORY_DATDIR_INDEX]);
	if ((SDCARD_NO_PATH == SDCardStatus) || (SDCARD_NO_FILE == SDCardStatus)) {
		SDCardStatus = SDCard_Client_CreateDirectory(&SDCardClient, &SDCardClient.FatfsDirectory[FATFS_DIRECTORY_DATDIR_INDEX]);
		if (SDCARD_OK == SDCardStatus) {
			SDCardStatus = SDCard_Client_ChecksDirectoryorFile(&SDCardClient, &SDCardClient.FatfsDirectory[FATFS_DIRECTORY_DATDIR_INDEX]);
			if (SDCARD_OK == SDCardStatus) SDCardClient.DictateRunCtl.dictateCreateDirectoryDATDIR = true;
		}
	}
	else if (SDCARD_OK == SDCardStatus) {
		SDCardClient.DictateRunCtl.dictateCreateDirectoryDATDIR = true;
	}
	
#ifdef SDCARD_LED1
	SDCARD_LED1_OFF();
#endif
}

#if SDCARD_DATFILE_WRITE
static unsigned char SDCard_Data_SumCheck(unsigned char* buf, unsigned int len)
{
	unsigned char sumCheck = 0;
	
	for (int i = 0; i < len; i++) {
		sumCheck += buf[i];
	}
	
	return sumCheck;
}
#endif

/**********************************************************************************************************
 @Function			void SDCard_APP_RunEvent(void)
 @Description			SDCard_APP_RunEvent
 @Input				void
 @Return				void
**********************************************************************************************************/
void SDCard_APP_RunEvent(void)
{
	SDCard_StatusTypeDef SDCardStatus = SDCARD_OK;
	
	/* 检查文件系统是否注册 */
	if (SDCardClient.DictateRunCtl.dictateRegistersFilesystem != true) {
		SDCardClient.DictateRunCtl.dictateCreateDirectoryLOGDIR = false;
		SDCardClient.DictateRunCtl.dictateCreateDirectoryDATDIR = false;
		
		Sdcard_Flgtime_Time_MS = 10000;
		
		if ((SDCardStatus = SDCard_Client_RegistersFilesystem(&SDCardClient)) == SDCARD_OK) {
			SDCardClient.DictateRunCtl.dictateRegistersFilesystem = true;
		}
		else {
			SDCardClient.DictateRunCtl.dictateRegistersFilesystem = false;
			return;
		}
		
		SDCardStatus = SDCard_Client_GetFreeClusters(&SDCardClient);
	}
	
	/* 检查文件夹是否创建 */
	if (SDCardClient.DictateRunCtl.dictateCreateDirectoryLOGDIR != true) {
		SDCardClient.FatfsDirectory[FATFS_DIRECTORY_LOGDIR_INDEX].path	= FATFS_DIRECTORY_LOGDIR_PATH;
		SDCardStatus = SDCard_Client_ChecksDirectoryorFile(&SDCardClient, &SDCardClient.FatfsDirectory[FATFS_DIRECTORY_LOGDIR_INDEX]);
		if ((SDCARD_NO_PATH == SDCardStatus) || (SDCARD_NO_FILE == SDCardStatus)) {
			SDCardStatus = SDCard_Client_CreateDirectory(&SDCardClient, &SDCardClient.FatfsDirectory[FATFS_DIRECTORY_LOGDIR_INDEX]);
			if (SDCARD_OK == SDCardStatus) {
				SDCardStatus = SDCard_Client_ChecksDirectoryorFile(&SDCardClient, &SDCardClient.FatfsDirectory[FATFS_DIRECTORY_LOGDIR_INDEX]);
				if (SDCARD_OK == SDCardStatus) SDCardClient.DictateRunCtl.dictateCreateDirectoryLOGDIR = true;
			}
		}
		else if (SDCARD_OK == SDCardStatus) {
			SDCardClient.DictateRunCtl.dictateCreateDirectoryLOGDIR = true;
		}
	}
	
	if (SDCardClient.DictateRunCtl.dictateCreateDirectoryDATDIR != true) {
		SDCardClient.FatfsDirectory[FATFS_DIRECTORY_DATDIR_INDEX].path	= FATFS_DIRECTORY_DATDIR_PATH;
		SDCardStatus = SDCard_Client_ChecksDirectoryorFile(&SDCardClient, &SDCardClient.FatfsDirectory[FATFS_DIRECTORY_DATDIR_INDEX]);
		if ((SDCARD_NO_PATH == SDCardStatus) || (SDCARD_NO_FILE == SDCardStatus)) {
			SDCardStatus = SDCard_Client_CreateDirectory(&SDCardClient, &SDCardClient.FatfsDirectory[FATFS_DIRECTORY_DATDIR_INDEX]);
			if (SDCARD_OK == SDCardStatus) {
				SDCardStatus = SDCard_Client_ChecksDirectoryorFile(&SDCardClient, &SDCardClient.FatfsDirectory[FATFS_DIRECTORY_DATDIR_INDEX]);
				if (SDCARD_OK == SDCardStatus) SDCardClient.DictateRunCtl.dictateCreateDirectoryDATDIR = true;
			}
		}
		else if (SDCARD_OK == SDCardStatus) {
			SDCardClient.DictateRunCtl.dictateCreateDirectoryDATDIR = true;
		}
	}
	
	/* 流量日志写入SDCARD方便查看数据 */
#if SDCARD_LOGFILE_WRITE
	if ((SDCardClient.DictateRunCtl.dictateRegistersFilesystem != false) && (SDCardClient.DictateRunCtl.dictateCreateDirectoryLOGDIR != false)) {
		if (NET_Sdcard_Message_SendDataDequeue(SDCardClient.DataProcessStack, &SDCardClient.DataProcessStack_len) == true) {
			
			Sdcard_Flgtime_Time_MS = 800;
			
			memset((void*)filepath, 0x00, sizeof(filepath));
			RTC_Time_GetTime(&fileyear, &filemon, &fileday, &filehous, &filemin, &filesec);
			sprintf((char*)filepath, "LOG/LOG%02d%02d%02d%02d.log", fileyear, filemon, fileday, filehous);
			SDCardClient.FatfsFile[FATFS_FILE_LOGDIR_INDEX].path = (TCHAR*)filepath;
			SDCardClient.FatfsFile[FATFS_FILE_LOGDIR_INDEX].fmode = FA_OPEN_APPEND | FA_WRITE | FA_READ;
			SDCardStatus = SDCard_Client_OpensFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_LOGDIR_INDEX]);
			if (SDCARD_OK == SDCardStatus) {
				SocketPacketHeadTypeDef* pHead = (SocketPacketHeadTypeDef*)SDCardClient.DataProcessStack;
				SocketPacketDataTypeDef* pData = (SocketPacketDataTypeDef*)&SDCardClient.DataProcessStack[sizeof(SocketPacketHeadTypeDef)];
				SocketPacketLastTypeDef* pLast = (SocketPacketLastTypeDef*)&SDCardClient.DataProcessStack[(SDCardClient.DataProcessStack_len - sizeof(SocketPacketLastTypeDef))];
				delay_1ms(5);
				
				/* 写入时间戳 */
				f_printf(&SDCardClient.FatfsFile[FATFS_FILE_LOGDIR_INDEX].fp, "20%02d-%02d-%02d %02d:%02d:%02d\n", fileyear, filemon, fileday, filehous, filemin, filesec);
				SDCard_Client_FlushesFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_LOGDIR_INDEX]);
				delay_1ms(5);
				
				/* 写入数据头 */
				memset((void*)SDCardClient.WriteProcessStack, 0x00, SDCardClient.WriteProcessStack_size);
				sprintf((char*)SDCardClient.WriteProcessStack, "CrossID:%c%c%c%c%c%c%c%c   PacketType:%d   PacketInfo:%d   ManufacturerCode:%d   CheckCode:%02X\n", \
				pHead->CrossID[0], pHead->CrossID[1], pHead->CrossID[2], pHead->CrossID[3], pHead->CrossID[4], pHead->CrossID[5], pHead->CrossID[6], pHead->CrossID[7], \
				pHead->PacketType, pHead->PacketInfo, pLast->ManuCode, pLast->CheckCode);
				SDCardClient.FatfsFile[FATFS_FILE_LOGDIR_INDEX].fbuf = (TCHAR*)SDCardClient.WriteProcessStack;
				SDCardClient.FatfsFile[FATFS_FILE_LOGDIR_INDEX].flen = strlen((const char*)SDCardClient.WriteProcessStack);
				SDCardStatus = SDCard_Client_WriteFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_LOGDIR_INDEX]);
				SDCard_Client_FlushesFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_LOGDIR_INDEX]);
				delay_1ms(5);
				
				/* 写入数据体 */
				memset((void*)SDCardClient.WriteProcessStack, 0x00, SDCardClient.WriteProcessStack_size);
				sprintf((char*)SDCardClient.WriteProcessStack, "DeviceType:%c   Interval:%d   LaneNo:%c%c%c   DateTime:%s  Volume:%d   AvgOccupancy:%d   AvgHeadTime:%d   AvgLength:%d   AvgSpeed:%d   \n", \
				pData->DeviceType, pData->Interval, pData->LaneNo[0], pData->LaneNo[1], pData->LaneNo[2], pData->DateTime, pData->Volume, pData->AvgOccupancy, pData->AvgHeadTime, (int)pData->AvgLength, (int)pData->AvgSpeed);
				SDCardClient.FatfsFile[FATFS_FILE_LOGDIR_INDEX].fbuf = (TCHAR*)SDCardClient.WriteProcessStack;
				SDCardClient.FatfsFile[FATFS_FILE_LOGDIR_INDEX].flen = strlen((const char*)SDCardClient.WriteProcessStack);
				SDCardStatus = SDCard_Client_WriteFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_LOGDIR_INDEX]);
				SDCard_Client_FlushesFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_LOGDIR_INDEX]);
				delay_1ms(5);
				
				/* 写入RawData */
				memset((void*)SDCardClient.WriteProcessStack, 0x00, SDCardClient.WriteProcessStack_size);
				sprintf((char*)SDCardClient.WriteProcessStack, "RawData:\n");
				for (int i = 0; i < SDCardClient.DataProcessStack_len; i++) {
					sprintf((char*)SDCardClient.WriteProcessStack + strlen((const char*)SDCardClient.WriteProcessStack), "%02X ", SDCardClient.DataProcessStack[i]);
				}
				sprintf((char*)SDCardClient.WriteProcessStack + strlen((const char*)SDCardClient.WriteProcessStack), "\n\n");
				SDCardClient.FatfsFile[FATFS_FILE_LOGDIR_INDEX].fbuf = (TCHAR*)SDCardClient.WriteProcessStack;
				SDCardClient.FatfsFile[FATFS_FILE_LOGDIR_INDEX].flen = strlen((const char*)SDCardClient.WriteProcessStack);
				SDCardStatus = SDCard_Client_WriteFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_LOGDIR_INDEX]);
				SDCard_Client_FlushesFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_LOGDIR_INDEX]);
				delay_1ms(5);
				
				SDCard_Client_ClosesFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_LOGDIR_INDEX]);
				delay_1ms(8);
				
				SDCardClient.FatfsFile[FATFS_FILE_LOGDIR_INDEX].path = FATFS_FILE_LOGDIR_PATH;
			}
			NET_Sdcard_Message_SendDataOffSet();
		}
	}
#endif
	
#if SDCARD_DATFILE_WRITE
	/* 未联网, 数据缓存剩余空间不足760字节, 将数据写入SDCard DAT文件中 */
#if SDCARD_DATFILE_INSDCARD
	if ((SDCardClient.DictateRunCtl.dictateRegistersFilesystem != false) && (SDCardClient.DictateRunCtl.dictateCreateDirectoryDATDIR != false)) {
		if ((SOCKET_ONLINE != true) && (NET_Socket_Message_SendDataFree() <= 760)) {
			
			NET_Socket_Message_SendDataDequeue(SDCardClient.DataProcessStack, &SDCardClient.DataProcessStack_len);
			if (SDCardClient.DataProcessStack_len != (sizeof(SocketPacketHeadTypeDef) + sizeof(SocketPacketDataTypeDef) + sizeof(SocketPacketLastTypeDef))) {
				NET_Socket_Message_SendDataOffSet();
				goto exitafter;
			}
			
			Sdcard_Flgtime_Time_MS = 800;
			
			SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].path = FATFS_FILE_DATDIR_PATH;
			SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fmode = FA_OPEN_ALWAYS | FA_WRITE | FA_READ;
			SDCardStatus = SDCard_Client_OpensFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
			if (SDCARD_OK == SDCardStatus) {
				
				/* 文件为新创建文件, 写入文件头 */
				if (f_size(&SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fp) < sizeof(SdcardDataHeadTypeDef)) {
					SdcardDataHead.StartCode = SDCARD_DATA_START_CODE;
					SdcardDataHead.FrontAddr = sizeof(SdcardDataHeadTypeDef);
					SdcardDataHead.ReartAddr = sizeof(SdcardDataHeadTypeDef);
					SdcardDataHead.SumCheck  = SDCard_Data_SumCheck((unsigned char*)&SdcardDataHead, sizeof(SdcardDataHeadTypeDef) - 1);
					SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fbuf = (TCHAR*)&SdcardDataHead;
					SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].flen = sizeof(SdcardDataHeadTypeDef);
					SDCardStatus = SDCard_Client_WriteFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
					SDCard_Client_FlushesFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
					SDCard_Client_TruncateFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
				}
				
				/* 获取文件头数据 */
				memset((void*)&SdcardDataHead, 0x00, sizeof(SdcardDataHeadTypeDef));
				SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fnofs = 0;
				SDCard_Client_LseekFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
				SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fbuf = (TCHAR*)&SdcardDataHead;
				SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].flen = sizeof(SdcardDataHeadTypeDef);
				SDCardStatus = SDCard_Client_ReadFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
				
				/* 检查文件头数据 */
				if ((SdcardDataHead.SumCheck != SDCard_Data_SumCheck((unsigned char*)&SdcardDataHead, sizeof(SdcardDataHeadTypeDef) - 1)) || (SdcardDataHead.StartCode != SDCARD_DATA_START_CODE) || (SdcardDataHead.FrontAddr != f_size(&SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fp))) {
					SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fnofs = 0;
					SDCard_Client_LseekFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
					SdcardDataHead.StartCode = SDCARD_DATA_START_CODE;
					SdcardDataHead.FrontAddr = sizeof(SdcardDataHeadTypeDef);
					SdcardDataHead.ReartAddr = sizeof(SdcardDataHeadTypeDef);
					SdcardDataHead.SumCheck  = SDCard_Data_SumCheck((unsigned char*)&SdcardDataHead, sizeof(SdcardDataHeadTypeDef) - 1);
					SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fbuf = (TCHAR*)&SdcardDataHead;
					SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].flen = sizeof(SdcardDataHeadTypeDef);
					SDCardStatus = SDCard_Client_WriteFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
					SDCard_Client_FlushesFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
					SDCard_Client_TruncateFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
					NET_Socket_Message_SendDataOffSet();
					goto exit;
				}
				
				/* 写入数据到文件 */
				SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fnofs = SdcardDataHead.FrontAddr;
				SDCard_Client_LseekFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
				SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fbuf = (TCHAR*)SDCardClient.DataProcessStack;
				SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].flen = SDCardClient.DataProcessStack_len;
				SDCardStatus = SDCard_Client_WriteFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
				if (SDCARD_OK == SDCardStatus) {
					SDCard_Client_FlushesFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
					SDCard_Client_TruncateFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
					if ((SdcardDataHead.FrontAddr + SDCardClient.DataProcessStack_len) != f_size(&SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fp)) {
						SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fnofs = 0;
						SDCard_Client_LseekFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
						SdcardDataHead.StartCode = SDCARD_DATA_START_CODE;
						SdcardDataHead.FrontAddr = sizeof(SdcardDataHeadTypeDef);
						SdcardDataHead.ReartAddr = sizeof(SdcardDataHeadTypeDef);
						SdcardDataHead.SumCheck  = SDCard_Data_SumCheck((unsigned char*)&SdcardDataHead, sizeof(SdcardDataHeadTypeDef) - 1);
						SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fbuf = (TCHAR*)&SdcardDataHead;
						SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].flen = sizeof(SdcardDataHeadTypeDef);
						SDCardStatus = SDCard_Client_WriteFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
						SDCard_Client_FlushesFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
						SDCard_Client_TruncateFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
						NET_Socket_Message_SendDataOffSet();
						goto exit;
					}
					SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fnofs = 0;
					SDCard_Client_LseekFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
					SdcardDataHead.StartCode = SDCARD_DATA_START_CODE;
					SdcardDataHead.FrontAddr = SdcardDataHead.FrontAddr + SDCardClient.DataProcessStack_len;
					SdcardDataHead.SumCheck  = SDCard_Data_SumCheck((unsigned char*)&SdcardDataHead, sizeof(SdcardDataHeadTypeDef) - 1);
					SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fbuf = (TCHAR*)&SdcardDataHead;
					SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].flen = sizeof(SdcardDataHeadTypeDef);
					SDCardStatus = SDCard_Client_WriteFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
					SDCard_Client_FlushesFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
					NET_Socket_Message_SendDataOffSet();
				}
				else {
					SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fnofs = 0;
					SDCard_Client_LseekFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
					SdcardDataHead.StartCode = SDCARD_DATA_START_CODE;
					SdcardDataHead.FrontAddr = sizeof(SdcardDataHeadTypeDef);
					SdcardDataHead.ReartAddr = sizeof(SdcardDataHeadTypeDef);
					SdcardDataHead.SumCheck  = SDCard_Data_SumCheck((unsigned char*)&SdcardDataHead, sizeof(SdcardDataHeadTypeDef) - 1);
					SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fbuf = (TCHAR*)&SdcardDataHead;
					SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].flen = sizeof(SdcardDataHeadTypeDef);
					SDCardStatus = SDCard_Client_WriteFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
					SDCard_Client_FlushesFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
					SDCard_Client_TruncateFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
					NET_Socket_Message_SendDataOffSet();
					goto exit;
				}
				
exit:
				SDCard_Client_ClosesFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
			}
		}
	}
#endif
	
	/* 已联网, 数据缓存剩余空间大于760字节, 从SDCard DAT文件中读取数据 */
#if SDCARD_DATFILE_OUSDCARD
	if ((SDCardClient.DictateRunCtl.dictateRegistersFilesystem != false) && (SDCardClient.DictateRunCtl.dictateCreateDirectoryDATDIR != false)) {
		if ((SOCKET_ONLINE != false) && (NET_Socket_Message_SendDataFree() > 760)) {
			
			bool deleteFileFlag = false;
			
			Sdcard_Flgtime_Time_MS = 800;
			
			if (Sdcard_DataFile_Normal_Existence != true) goto exitafter;
			
			/* 判断文件是否存在 */
			SDCardClient.FatfsDirectory[FATFS_DIRECTORY_DATDIR_INDEX].path = FATFS_FILE_DATDIR_PATH;
			SDCardStatus = SDCard_Client_ChecksDirectoryorFile(&SDCardClient, &SDCardClient.FatfsDirectory[FATFS_DIRECTORY_DATDIR_INDEX]);
			if (SDCARD_OK != SDCardStatus) {
				Sdcard_DataFile_Normal_Existence = false;
				goto exitafter;
			}
			
			SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].path = FATFS_FILE_DATDIR_PATH;
			SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fmode = FA_OPEN_ALWAYS | FA_WRITE | FA_READ;
			SDCardStatus = SDCard_Client_OpensFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
			if (SDCARD_OK == SDCardStatus) {
				
				/* 判断文件头 */
				if (f_size(&SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fp) < sizeof(SdcardDataHeadTypeDef)) {
					goto exitread;
				}
				
				/* 获取文件头数据 */
				memset((void*)&SdcardDataHead, 0x00, sizeof(SdcardDataHeadTypeDef));
				SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fnofs = 0;
				SDCard_Client_LseekFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
				SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fbuf = (TCHAR*)&SdcardDataHead;
				SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].flen = sizeof(SdcardDataHeadTypeDef);
				SDCardStatus = SDCard_Client_ReadFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
				if (SDCARD_OK != SDCardStatus) {
					goto exitread;
				}
				
				/* 检查文件头数据 */
				if ((SdcardDataHead.SumCheck != SDCard_Data_SumCheck((unsigned char*)&SdcardDataHead, sizeof(SdcardDataHeadTypeDef) - 1)) || (SdcardDataHead.StartCode != SDCARD_DATA_START_CODE) || (SdcardDataHead.FrontAddr != f_size(&SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fp))) {
					goto exitread;
				}
				
				/* 判断是否存在数据需要读取 */
				if ((SdcardDataHead.FrontAddr <= SdcardDataHead.ReartAddr) || ((SdcardDataHead.FrontAddr - SdcardDataHead.ReartAddr) < (sizeof(SocketPacketHeadTypeDef) + sizeof(SocketPacketDataTypeDef) + sizeof(SocketPacketLastTypeDef)))) {
					deleteFileFlag = true;
					goto exitread;
				}
				
				/* 从文件中读取数据 */
				SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fnofs = SdcardDataHead.ReartAddr;
				SDCard_Client_LseekFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
				memset((void*)SDCardClient.DataProcessStack, 0x00, SDCardClient.DataProcessStack_size);
				SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fbuf = (TCHAR*)SDCardClient.DataProcessStack;
				SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].flen = (sizeof(SocketPacketHeadTypeDef) + sizeof(SocketPacketDataTypeDef) + sizeof(SocketPacketLastTypeDef));
				SDCardStatus = SDCard_Client_ReadFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
				if (SDCARD_OK == SDCardStatus) {
					SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fnofs = 0;
					SDCard_Client_LseekFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
					SdcardDataHead.StartCode = SDCARD_DATA_START_CODE;
					SdcardDataHead.ReartAddr = SdcardDataHead.ReartAddr + (sizeof(SocketPacketHeadTypeDef) + sizeof(SocketPacketDataTypeDef) + sizeof(SocketPacketLastTypeDef));
					SdcardDataHead.SumCheck  = SDCard_Data_SumCheck((unsigned char*)&SdcardDataHead, sizeof(SdcardDataHeadTypeDef) - 1);
					SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].fbuf = (TCHAR*)&SdcardDataHead;
					SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX].flen = sizeof(SdcardDataHeadTypeDef);
					SDCardStatus = SDCard_Client_WriteFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
					if (SDCARD_OK == SDCardStatus) {
						SDCard_Client_FlushesFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
						/* 读出数据检查(校验检查) */
						unsigned char tcheckcode = 0;
						unsigned int  tdataindex = sizeof(SocketPacketHeadTypeDef);
						tcheckcode = SDCardClient.DataProcessStack[tdataindex];
						tdataindex += 1;
						for (int i = tdataindex; i < (sizeof(SocketPacketDataTypeDef) + tdataindex - 1); i++) {
							tcheckcode = tcheckcode ^ SDCardClient.DataProcessStack[i];
						}
						if (tcheckcode == SDCardClient.DataProcessStack[(sizeof(SocketPacketHeadTypeDef) + sizeof(SocketPacketDataTypeDef) + sizeof(SocketPacketLastTypeDef) - 1)]) {
							NET_Socket_Message_SendDataEnqueue((u8 *)SDCardClient.DataProcessStack, (sizeof(SocketPacketHeadTypeDef) + sizeof(SocketPacketDataTypeDef) + sizeof(SocketPacketLastTypeDef)));
						}
					}
					else {
						goto exitread;
					}
				}
				else {
					goto exitread;
				}
				
exitread:
				SDCard_Client_ClosesFile(&SDCardClient, &SDCardClient.FatfsFile[FATFS_FILE_DATDIR_INDEX]);
			}
			
			/* 判断文件是否需要删除 */
			if (deleteFileFlag != false) {
				SDCardClient.FatfsDirectory[FATFS_DIRECTORY_DATDIR_INDEX].path = FATFS_FILE_DATDIR_PATH;
				SDCardStatus = SDCard_Client_RemoveDirectoryorFile(&SDCardClient, &SDCardClient.FatfsDirectory[FATFS_DIRECTORY_DATDIR_INDEX]);
			}
			
			SOCKET_ONLINE = false;
		}
	}
#endif
	
exitafter:
	return;
#endif
}






























/********************************************** END OF FLEE **********************************************/
