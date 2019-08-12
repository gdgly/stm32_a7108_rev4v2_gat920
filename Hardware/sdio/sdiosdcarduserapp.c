/**
  *********************************************************************************************************
  * @file    sdiosdcarduserapp.c
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

#include "sdiosdcarduserapp.h"
#include "sdiosdcard.h"
#include "sdiosdcardmessage.h"
#include "bsp_usart.h"

//#define SDCARD_DEBUG_LOG_RF_PRINT																					//定义开启输出DEBUG信息
#define SDCARD_DEBUG_LOG_PRINTF			printf																	//定义开启输出DEBUG信息函数

unsigned char SDCARD_DataProcessStack[SDCARD_DATASTACK_SIZE];
unsigned char SDCARD_WriteProcessStack[SDCARD_WRITESTACK_SIZE];

SDCard_ClientsTypeDef					SDCardClient;

/**********************************************************************************************************
 @Function			void SDCard_Client_Init(SDCard_ClientsTypeDef* pClient)
 @Description			SDCard_Client_Init						: 初始化SDCard客户端
 @Input				pClient								: SDCard客户端实例
 @Return				void
**********************************************************************************************************/
void SDCard_Client_Init(SDCard_ClientsTypeDef* pClient)
{
	pClient->MountState										= Unmount;
	
	pClient->DataProcessStack								= SDCARD_DataProcessStack;
	pClient->DataProcessStack_size							= sizeof(SDCARD_DataProcessStack);
	pClient->DataProcessStack_len								= 0;
	
	pClient->WriteProcessStack								= SDCARD_WriteProcessStack;
	pClient->WriteProcessStack_size							= sizeof(SDCARD_WriteProcessStack);
	pClient->WriteProcessStack_len							= 0;
	
	pClient->FatfsClient.pfs									= &pClient->FatfsClient.fs;
	
	memset((void*)&pClient->FatfsClient.fs, 0x00, sizeof(pClient->FatfsClient.fs));
	memset((void*)pClient->FatfsClient.work, 0x00, sizeof(pClient->FatfsClient.work));
	
	pClient->FatfsClient.fsres								= FR_OK;
	
	pClient->FatfsDirectory[FATFS_DIRECTORY_LOGDIR_INDEX].path		= FATFS_DIRECTORY_LOGDIR_PATH;
	pClient->FatfsDirectory[FATFS_DIRECTORY_DATDIR_INDEX].path		= FATFS_DIRECTORY_DATDIR_PATH;
	pClient->FatfsFile[FATFS_FILE_LOGDIR_INDEX].path				= FATFS_FILE_LOGDIR_PATH;
	pClient->FatfsFile[FATFS_FILE_DATDIR_INDEX].path				= FATFS_FILE_DATDIR_PATH;
	
	
	
	
	pClient->DictateRunCtl.dictateRegistersFilesystem				= false;
	pClient->DictateRunCtl.dictateCreateDirectoryLOGDIR			= false;
	pClient->DictateRunCtl.dictateCreateDirectoryDATDIR			= false;
	
	
	
	
	
	
	
	
	NET_Sdcard_FifoSendMessageInit();
	NET_Sdcard_FifoRecvMessageInit();
	
	
	
	
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
	RS485_Mode_TX();
#endif
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
	SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Client Init OK >\r\n");
#endif
}

/**********************************************************************************************************
 @Function			SDCard_StatusTypeDef SDCard_Client_RegistersFilesystem(SDCard_ClientsTypeDef* pClient)
 @Description			SDCard_Client_RegistersFilesystem			: SDCard注册文件系统
 @Input				pClient								: SDCard客户端实例
 @Return				SDCard_StatusTypeDef					: SDCard执行结果
**********************************************************************************************************/
SDCard_StatusTypeDef SDCard_Client_RegistersFilesystem(SDCard_ClientsTypeDef* pClient)
{
	SDCard_StatusTypeDef SDCardStatus = SDCARD_OK;
	
	pClient->MountState = Unmount;
	
	/* SDCard挂载 */
	pClient->FatfsClient.fsres = f_mount(pClient->FatfsClient.pfs, "0:", 1);
	
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
	SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Mount Filesystem: %d >\r\n", pClient->FatfsClient.fsres);
#endif
	
	if (FR_NO_FILESYSTEM == pClient->FatfsClient.fsres) {
		
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Creates an FAT32 on the logical drive >\r\n");
#endif
		/* SDCard格式化 */
		pClient->FatfsClient.fsres = f_mkfs("0:", FM_FAT32, 0, pClient->FatfsClient.work, sizeof(pClient->FatfsClient.work));
		
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Mkfs FAT32: %d >\r\n", pClient->FatfsClient.fsres);
#endif
		
		if (FR_OK == pClient->FatfsClient.fsres) {
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
			SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Mkfs FAT32 OK >\r\n");
#endif
			pClient->FatfsClient.fsres = f_mount(NULL, "0:", 1);
			if ((FR_OK != pClient->FatfsClient.fsres) && (FR_NOT_ENABLED != pClient->FatfsClient.fsres)) {
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
				SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Unregisters Filesystem: %d >\r\n", pClient->FatfsClient.fsres);
#endif
				SDCardStatus = (SDCard_StatusTypeDef)pClient->FatfsClient.fsres;
				goto exit;
			}
			pClient->FatfsClient.fsres = f_mount(pClient->FatfsClient.pfs, "0:", 1);
			if (FR_OK != pClient->FatfsClient.fsres) {
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
				SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Registers Filesystem: %d >\r\n", pClient->FatfsClient.fsres);
#endif
				SDCardStatus = (SDCard_StatusTypeDef)pClient->FatfsClient.fsres;
				goto exit;
			}
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
			SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Mount Filesystem: %d >\r\n", pClient->FatfsClient.fsres);
			SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Mount Filesystem OK >\r\n");
#endif
		}
		else {
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
			SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Mkfs FAT32 Fail >\r\n");
#endif
			SDCardStatus = (SDCard_StatusTypeDef)pClient->FatfsClient.fsres;
			goto exit;
		}
	}
	else if (FR_OK != pClient->FatfsClient.fsres) {
		/* SDCard挂载失败 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Mount Filesystem Fail >\r\n");
#endif
		SDCardStatus = (SDCard_StatusTypeDef)pClient->FatfsClient.fsres;
		goto exit;
	}
	else {
		/* SDCard挂载成功 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Mount Filesystem OK >\r\n");
#endif
	}
	
	pClient->MountState = Ismount;
	
	SDCardStatus = SDCARD_OK;
	
exit:
	return SDCardStatus;
}

/**********************************************************************************************************
 @Function			SDCard_StatusTypeDef SDCard_Client_UnregistersFilesystem(SDCard_ClientsTypeDef* pClient)
 @Description			SDCard_Client_UnregistersFilesystem		: SDCard注销文件系统
 @Input				pClient								: SDCard客户端实例
 @Return				SDCard_StatusTypeDef					: SDCard执行结果
**********************************************************************************************************/
SDCard_StatusTypeDef SDCard_Client_UnregistersFilesystem(SDCard_ClientsTypeDef* pClient)
{
	SDCard_StatusTypeDef SDCardStatus = SDCARD_OK;
	
	pClient->MountState = Unmount;
	
	/* SDCard卸载 */
	pClient->FatfsClient.fsres = f_mount(NULL, "0:", 1);
	
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
	SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Unmount Filesystem: %d >\r\n", pClient->FatfsClient.fsres);
#endif
	
	if ((FR_OK != pClient->FatfsClient.fsres) && (FR_NOT_ENABLED != pClient->FatfsClient.fsres)) {
		/* SDCard卸载失败 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Unmount Filesystem Fail >\r\n");
#endif
		SDCardStatus = (SDCard_StatusTypeDef)pClient->FatfsClient.fsres;
		goto exit;
	}
	else {
		/* SDCard卸载成功 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Unmount Filesystem OK >\r\n");
#endif
	}
	
	SDCardStatus = SDCARD_OK;
	
exit:
	return SDCardStatus;
}

/**********************************************************************************************************
 @Function			SDCard_StatusTypeDef SDCard_Client_GetFreeClusters(SDCard_ClientsTypeDef* pClient)
 @Description			SDCard_Client_GetFreeClusters				: SDCard获取卷的空闲簇数量
 @Input				pClient								: SDCard客户端实例
 @Return				SDCard_StatusTypeDef					: SDCard执行结果
**********************************************************************************************************/
SDCard_StatusTypeDef SDCard_Client_GetFreeClusters(SDCard_ClientsTypeDef* pClient)
{
	SDCard_StatusTypeDef SDCardStatus = SDCARD_OK;
	
	if (pClient->MountState != Ismount) {
		SDCardStatus = SDCard_Client_RegistersFilesystem(pClient);
		if (SDCardStatus != SDCARD_OK) {
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
			SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Is Unmount Filesystem >\r\n");
#endif
			goto exit;
		}
	}
	
	/* SDCard获取卷的空闲簇数量 */
	pClient->FatfsClient.fsres = f_getfree("0:", &pClient->FatfsParameter.fre_clust, &pClient->FatfsClient.pfs);
	
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
	SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Getfree Clusters: %d >\r\n", pClient->FatfsClient.fsres);
#endif
	
	if (FR_OK != pClient->FatfsClient.fsres) {
		/* SDCard空间获取失败 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Getfree Clusters Fail >\r\n");
#endif
		SDCardStatus = (SDCard_StatusTypeDef)pClient->FatfsClient.fsres;
		goto exit;
	}
	else {
		/* SDCard空间获取成功 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Getfree Clusters OK >\r\n");
#endif
	}
	
	pClient->FatfsParameter.tot_sect = (DWORD)(pClient->FatfsClient.pfs->n_fatent - 2) * pClient->FatfsClient.pfs->csize;
	pClient->FatfsParameter.fre_sect = (DWORD)(pClient->FatfsParameter.fre_clust) * pClient->FatfsClient.pfs->csize;
	
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
	SDCARD_DEBUG_LOG_PRINTF("\r\n");
	SDCARD_DEBUG_LOG_PRINTF("[SD Card Info:]<! Card Total Deive Space     : %10lu KiB >\r\n", (u32)pClient->FatfsParameter.tot_sect / 2);
	SDCARD_DEBUG_LOG_PRINTF("[SD Card Info:]<! Card Total Deive Space     : %10lu MiB >\r\n", (u32)pClient->FatfsParameter.tot_sect / 2 / 1024);
	SDCARD_DEBUG_LOG_PRINTF("[SD Card Info:]<! Card Total Deive Space     : %10lu GiB >\r\n", (u32)pClient->FatfsParameter.tot_sect / 2 / 1024 / 1024);
	SDCARD_DEBUG_LOG_PRINTF("[SD Card Info:]<! Card Available Deive Space : %10lu KiB >\r\n", (u32)pClient->FatfsParameter.fre_sect / 2);
	SDCARD_DEBUG_LOG_PRINTF("[SD Card Info:]<! Card Available Deive Space : %10lu MiB >\r\n", (u32)pClient->FatfsParameter.fre_sect / 2 / 1024);
	SDCARD_DEBUG_LOG_PRINTF("[SD Card Info:]<! Card Available Deive Space : %10lu GiB >\r\n", (u32)pClient->FatfsParameter.fre_sect / 2 / 1024 / 1024);
#endif
	
	SDCardStatus = SDCARD_OK;
	
exit:
	return SDCardStatus;
}

/**********************************************************************************************************
 @Function			static SDCard_StatusTypeDef SDCard_Client_FileAddDirectoryErrorReport(SDCard_ClientsTypeDef* pClient)
 @Description			SDCard_Client_FileAddDirectoryErrorReport	: SDCard文件与文件夹操作警报
 @Input				pClient								: SDCard客户端实例
 @Return				SDCard_StatusTypeDef					: SDCard执行结果
**********************************************************************************************************/
static SDCard_StatusTypeDef SDCard_Client_FileAndDirectoryErrorReport(SDCard_ClientsTypeDef* pClient)
{
	SDCard_StatusTypeDef SDCardStatus = SDCARD_OK;
	
	if (FR_NO_FILE == pClient->FatfsClient.fsres) {
		/* SDCard未找到文件 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Could not find the file >\r\n");
#endif
		SDCardStatus = (SDCard_StatusTypeDef)pClient->FatfsClient.fsres;
		goto exit;
	}
	else if (FR_NO_PATH == pClient->FatfsClient.fsres) {
		/* SDCard未找到目录 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Could not find the directory >\r\n");
#endif
		SDCardStatus = (SDCard_StatusTypeDef)pClient->FatfsClient.fsres;
		goto exit;
	}
	else if (FR_INVALID_NAME == pClient->FatfsClient.fsres) {
		/* SDCard路径名无效 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Path name as invalid >\r\n");
#endif
		SDCardStatus = (SDCard_StatusTypeDef)pClient->FatfsClient.fsres;
		goto exit;
	}
	else if (FR_EXIST == pClient->FatfsClient.fsres) {
		/* SDCard名称冲突 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Name collision >\r\n");
#endif
		SDCardStatus = (SDCard_StatusTypeDef)pClient->FatfsClient.fsres;
		goto exit;
	}
	else if (FR_NOT_ENABLED == pClient->FatfsClient.fsres) {
		/* SDCard文件系统未挂载 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card drive has not been registered >\r\n");
#endif
		SDCardStatus = (SDCard_StatusTypeDef)pClient->FatfsClient.fsres;
		goto exit;
	}
	else if (FR_NO_FILESYSTEM == pClient->FatfsClient.fsres) {
		/* SDCard无文件系统 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card valid FAT volume on the drive >\r\n");
#endif
		SDCardStatus = (SDCard_StatusTypeDef)pClient->FatfsClient.fsres;
		goto exit;
	}
	
	SDCardStatus = SDCARD_OK;
	
exit:
	return SDCardStatus;
}

/**********************************************************************************************************
 @Function			static SDCard_StatusTypeDef SDCard_Client_CheckRegistersFilesystem(SDCard_ClientsTypeDef* pClient)
 @Description			SDCard_Client_CheckRegistersFilesystem		: SDCard检查文件系统是否存在
 @Input				pClient								: SDCard客户端实例
 @Return				SDCard_StatusTypeDef					: SDCard执行结果
**********************************************************************************************************/
static SDCard_StatusTypeDef SDCard_Client_CheckRegistersFilesystem(SDCard_ClientsTypeDef* pClient)
{
	SDCard_StatusTypeDef SDCardStatus = SDCARD_OK;
	
	if (pClient->MountState != Ismount) {
		SDCardStatus = SDCard_Client_RegistersFilesystem(pClient);
		if (SDCardStatus != SDCARD_OK) {
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
			SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Is Unmount Filesystem >\r\n");
#endif
		}
	}
	
	return SDCardStatus;
}

/**********************************************************************************************************
 @Function			SDCard_StatusTypeDef SDCard_Client_CreateDirectory(SDCard_ClientsTypeDef* pClient, FATFS_DirectoryTypeDef* Dir)
 @Description			SDCard_Client_CreateDirectory				: SDCard创建文件夹
 @Input				pClient								: SDCard客户端实例
					Dir									: SDCard文件夹实例
 @Return				SDCard_StatusTypeDef					: SDCard执行结果
**********************************************************************************************************/
SDCard_StatusTypeDef SDCard_Client_CreateDirectory(SDCard_ClientsTypeDef* pClient, FATFS_DirectoryTypeDef* Dir)
{
	SDCard_StatusTypeDef SDCardStatus = SDCARD_OK;
	
	if ((SDCardStatus = SDCard_Client_CheckRegistersFilesystem(pClient)) != SDCARD_OK) goto exit;
	
	/* SDCard创建文件夹目录 */
	pClient->FatfsClient.fsres = f_mkdir(Dir->path);
	
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
	SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Create Directory %s: %d >\r\n", Dir->path, pClient->FatfsClient.fsres);
#endif
	
	SDCardStatus = SDCard_Client_FileAndDirectoryErrorReport(pClient);
	
	if (FR_OK != pClient->FatfsClient.fsres) {
		/* SDCard目录文件夹创建失败 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Create Directory Fail >\r\n");
#endif
		SDCardStatus = (SDCard_StatusTypeDef)pClient->FatfsClient.fsres;
		goto exit;
	}
	else {
		/* SDCard目录文件夹创建成功 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Create Directory OK >\r\n");
#endif
	}
	
exit:
	return SDCardStatus;
}

/**********************************************************************************************************
 @Function			SDCard_StatusTypeDef SDCard_Client_ChecksDirectoryorFile(SDCard_ClientsTypeDef* pClient, FATFS_DirectoryTypeDef* Dir)
 @Description			SDCard_Client_ChecksDirectoryorFile		: SDCard获取文件夹或文件状态
 @Input				pClient								: SDCard客户端实例
					Dir									: SDCard文件夹实例
 @Return				SDCard_StatusTypeDef					: SDCard执行结果
**********************************************************************************************************/
SDCard_StatusTypeDef SDCard_Client_ChecksDirectoryorFile(SDCard_ClientsTypeDef* pClient, FATFS_DirectoryTypeDef* Dir)
{
	SDCard_StatusTypeDef SDCardStatus = SDCARD_OK;
	
	if ((SDCardStatus = SDCard_Client_CheckRegistersFilesystem(pClient)) != SDCARD_OK) goto exit;
	
	/* SDCard检查文件目录 */
	pClient->FatfsClient.fsres = f_stat(Dir->path, &Dir->finfo);
	
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
	SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Check Directory or File %s: %d >\r\n", Dir->path, pClient->FatfsClient.fsres);
#endif
	
	SDCardStatus = SDCard_Client_FileAndDirectoryErrorReport(pClient);
	
	if (FR_OK != pClient->FatfsClient.fsres) {
		/* SDCard目录文件夹检查失败 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Check Directory or File Fail >\r\n");
#endif
		SDCardStatus = (SDCard_StatusTypeDef)pClient->FatfsClient.fsres;
		goto exit;
	}
	else {
		/* SDCard目录文件夹检查成功 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Check Directory or File OK >\r\n");
#endif
	}
	
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
	SDCARD_DEBUG_LOG_PRINTF("\r\n");
	SDCARD_DEBUG_LOG_PRINTF("[SD Card Info:]<! Card FName: %s >\r\n", Dir->finfo.fname);
	SDCARD_DEBUG_LOG_PRINTF("[SD Card Info:]<! Card FSize: %d >\r\n", (int)Dir->finfo.fsize);
	SDCARD_DEBUG_LOG_PRINTF("[SD Card Info:]<! Card Timestamp: %u/%02u/%02u, %02u:%02u >\r\n", (Dir->finfo.fdate >> 9) + 1980, Dir->finfo.fdate >> 5 & 15, Dir->finfo.fdate & 31, Dir->finfo.ftime >> 11, Dir->finfo.ftime >> 5 & 63);
	SDCARD_DEBUG_LOG_PRINTF("[SD Card Info:]<! Card Attributes: %c%c%c%c%c >\r\n", (Dir->finfo.fattrib & AM_DIR) ? 'D' : '-', (Dir->finfo.fattrib & AM_RDO) ? 'R' : '-', (Dir->finfo.fattrib & AM_HID) ? 'H' : '-', (Dir->finfo.fattrib & AM_SYS) ? 'S' : '-', (Dir->finfo.fattrib & AM_ARC) ? 'A' : '-');
#endif
	
exit:
	return SDCardStatus;
}

/**********************************************************************************************************
 @Function			SDCard_StatusTypeDef SDCard_Client_ChangeTimestamp(SDCard_ClientsTypeDef* pClient, FATFS_DirectoryTypeDef* Dir)
 @Description			SDCard_Client_ChangeTimestamp				: SDCard更改文件或文件夹时间
 @Input				pClient								: SDCard客户端实例
					Dir									: SDCard文件夹实例
 @Return				SDCard_StatusTypeDef					: SDCard执行结果
**********************************************************************************************************/
SDCard_StatusTypeDef SDCard_Client_ChangeTimestamp(SDCard_ClientsTypeDef* pClient, FATFS_DirectoryTypeDef* Dir)
{
	SDCard_StatusTypeDef SDCardStatus = SDCARD_OK;
	
	if ((SDCardStatus = SDCard_Client_CheckRegistersFilesystem(pClient)) != SDCARD_OK) goto exit;
	
	/* SDCard更改文件或文件夹时间 */
	pClient->FatfsClient.fsres = f_utime(Dir->path, &Dir->finfo);
	
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
	SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Change Timestamp Directory or File %s: %d >\r\n", Dir->path, pClient->FatfsClient.fsres);
#endif
	
	SDCardStatus = SDCard_Client_FileAndDirectoryErrorReport(pClient);
	
	if (FR_OK != pClient->FatfsClient.fsres) {
		/* SDCard目录文件夹时间修改失败 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Change Timestamp Directory or File Fail >\r\n");
#endif
		SDCardStatus = (SDCard_StatusTypeDef)pClient->FatfsClient.fsres;
		goto exit;
	}
	else {
		/* SDCard目录文件夹时间修改成功 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Change Timestamp Directory or File OK >\r\n");
#endif
	}
	
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
	SDCARD_DEBUG_LOG_PRINTF("\r\n");
	SDCARD_DEBUG_LOG_PRINTF("[SD Card Info:]<! Card FName: %s >\r\n", Dir->finfo.fname);
	SDCARD_DEBUG_LOG_PRINTF("[SD Card Info:]<! Card FSize: %d >\r\n", (int)Dir->finfo.fsize);
	SDCARD_DEBUG_LOG_PRINTF("[SD Card Info:]<! Card Timestamp: %u/%02u/%02u, %02u:%02u >\r\n", (Dir->finfo.fdate >> 9) + 1980, Dir->finfo.fdate >> 5 & 15, Dir->finfo.fdate & 31, Dir->finfo.ftime >> 11, Dir->finfo.ftime >> 5 & 63);
#endif
	
exit:
	return SDCardStatus;
}

/**********************************************************************************************************
 @Function			SDCard_StatusTypeDef SDCard_Client_RemoveDirectoryorFile(SDCard_ClientsTypeDef* pClient, FATFS_DirectoryTypeDef* Dir)
 @Description			SDCard_Client_RemoveDirectoryorFile		: SDCard删除文件或文件夹
 @Input				pClient								: SDCard客户端实例
					Dir									: SDCard文件夹实例
 @Return				SDCard_StatusTypeDef					: SDCard执行结果
**********************************************************************************************************/
SDCard_StatusTypeDef SDCard_Client_RemoveDirectoryorFile(SDCard_ClientsTypeDef* pClient, FATFS_DirectoryTypeDef* Dir)
{
	SDCard_StatusTypeDef SDCardStatus = SDCARD_OK;
	
	if ((SDCardStatus = SDCard_Client_CheckRegistersFilesystem(pClient)) != SDCARD_OK) goto exit;
	
	/* SDCard删除文件或文件夹 */
	pClient->FatfsClient.fsres = f_unlink(Dir->path);
	
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
	SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Change Remove Directory or File %s: %d >\r\n", Dir->path, pClient->FatfsClient.fsres);
#endif
	
	SDCardStatus = SDCard_Client_FileAndDirectoryErrorReport(pClient);
	
	if (FR_OK != pClient->FatfsClient.fsres) {
		/* SDCard目录文件夹删除失败 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Remove Directory or File Fail >\r\n");
#endif
		SDCardStatus = (SDCard_StatusTypeDef)pClient->FatfsClient.fsres;
		goto exit;
	}
	else {
		/* SDCard目录文件夹删除成功 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Remove Directory or File OK >\r\n");
#endif
	}
	
exit:
	return SDCardStatus;
}

/**********************************************************************************************************
 @Function			SDCard_StatusTypeDef SDCard_Client_OpensFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File)
 @Description			SDCard_Client_OpensFile					: SDCard打开文件
 @Input				pClient								: SDCard客户端实例
					File									: SDCard文件实例
 @Return				SDCard_StatusTypeDef					: SDCard执行结果
**********************************************************************************************************/
SDCard_StatusTypeDef SDCard_Client_OpensFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File)
{
	SDCard_StatusTypeDef SDCardStatus = SDCARD_OK;
	
	if ((SDCardStatus = SDCard_Client_CheckRegistersFilesystem(pClient)) != SDCARD_OK) goto exit;
	
	/* SDCard打开/创建文件 */
	pClient->FatfsClient.fsres = f_open(&File->fp, File->path, File->fmode);
	
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
	SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Open File %s: %d >\r\n", File->path, pClient->FatfsClient.fsres);
#endif
	
	SDCardStatus = SDCard_Client_FileAndDirectoryErrorReport(pClient);
	
	if (FR_OK != pClient->FatfsClient.fsres) {
		/* SDCard打开/创建文件失败 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Open File Fail >\r\n");
#endif
		SDCardStatus = (SDCard_StatusTypeDef)pClient->FatfsClient.fsres;
		goto exit;
	}
	else {
		/* SDCard打开/创建文件成功 */
		File->fsize = f_size(&File->fp);
		
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Open File OK >\r\n");
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card File Size %10llu Byte >\r\n", File->fsize);
#endif
	}
	
exit:
	return SDCardStatus;
}

/**********************************************************************************************************
 @Function			SDCard_StatusTypeDef SDCard_Client_ClosesFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File)
 @Description			SDCard_Client_ClosesFile					: SDCard关闭文件
 @Input				pClient								: SDCard客户端实例
					File									: SDCard文件实例
 @Return				SDCard_StatusTypeDef					: SDCard执行结果
**********************************************************************************************************/
SDCard_StatusTypeDef SDCard_Client_ClosesFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File)
{
	SDCard_StatusTypeDef SDCardStatus = SDCARD_OK;
	
	if ((SDCardStatus = SDCard_Client_CheckRegistersFilesystem(pClient)) != SDCARD_OK) goto exit;
	
	/* SDCard关闭文件 */
	pClient->FatfsClient.fsres = f_close(&File->fp);
	
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
	SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Close File %s: %d >\r\n", File->path, pClient->FatfsClient.fsres);
#endif
	
	SDCardStatus = SDCard_Client_FileAndDirectoryErrorReport(pClient);
	
	if (FR_OK != pClient->FatfsClient.fsres) {
		/* SDCard关闭文件失败 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Close File Fail >\r\n");
#endif
		SDCardStatus = (SDCard_StatusTypeDef)pClient->FatfsClient.fsres;
		goto exit;
	}
	else {
		/* SDCard关闭文件成功 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Close File OK >\r\n");
#endif
	}
	
exit:
	return SDCardStatus;
}

/**********************************************************************************************************
 @Function			SDCard_StatusTypeDef SDCard_Client_WriteFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File)
 @Description			SDCard_Client_WriteFile					: SDCard写数据到文件
 @Input				pClient								: SDCard客户端实例
					File									: SDCard文件实例
 @Return				SDCard_StatusTypeDef					: SDCard执行结果
**********************************************************************************************************/
SDCard_StatusTypeDef SDCard_Client_WriteFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File)
{
	SDCard_StatusTypeDef SDCardStatus = SDCARD_OK;
	
	if ((SDCardStatus = SDCard_Client_CheckRegistersFilesystem(pClient)) != SDCARD_OK) goto exit;
	
	if ((File->fbuf == NULL) || (File->flen == 0)) {
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Write File Paramete Fail >\r\n");
#endif
		SDCardStatus = SDCARD_ERROR;
		goto exit;
	}
	
	/* SDCard写数据到文件 */
	pClient->FatfsClient.fsres = f_write(&File->fp, (const void*)File->fbuf, File->flen, &File->fcont);
	
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
	SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Write File %s: %d, fwrite:%d, fcont: %d >\r\n", File->path, pClient->FatfsClient.fsres, File->flen, File->fcont);
#endif
	
	SDCardStatus = SDCard_Client_FileAndDirectoryErrorReport(pClient);
	
	if (FR_OK != pClient->FatfsClient.fsres) {
		/* SDCard写入文件失败 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Write File Fail >\r\n");
#endif
		SDCardStatus = (SDCard_StatusTypeDef)pClient->FatfsClient.fsres;
		goto exit;
	}
	else {
		/* SDCard写入文件成功 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Write File OK >\r\n");
#endif
	}
	
exit:
	return SDCardStatus;
}

/**********************************************************************************************************
 @Function			SDCard_StatusTypeDef SDCard_Client_ReadFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File)
 @Description			SDCard_Client_ReadFile					: SDCard读数据从文件
 @Input				pClient								: SDCard客户端实例
					File									: SDCard文件实例
 @Return				SDCard_StatusTypeDef					: SDCard执行结果
**********************************************************************************************************/
SDCard_StatusTypeDef SDCard_Client_ReadFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File)
{
	SDCard_StatusTypeDef SDCardStatus = SDCARD_OK;
	
	if ((SDCardStatus = SDCard_Client_CheckRegistersFilesystem(pClient)) != SDCARD_OK) goto exit;
	
	if ((File->fbuf == NULL) || (File->flen == 0)) {
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Read File Paramete Fail >\r\n");
#endif
		SDCardStatus = SDCARD_ERROR;
		goto exit;
	}
	
	/* SDCard读数据从文件 */
	pClient->FatfsClient.fsres = f_read(&File->fp, (void*)File->fbuf, File->flen, &File->fcont);
	
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
	SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Read File %s: %d, fread:%d, fcont: %d >\r\n", File->path, pClient->FatfsClient.fsres, File->flen, File->fcont);
#endif
	
	SDCardStatus = SDCard_Client_FileAndDirectoryErrorReport(pClient);
	
	if (FR_OK != pClient->FatfsClient.fsres) {
		/* SDCard读取文件失败 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Read File Fail >\r\n");
#endif
		SDCardStatus = (SDCard_StatusTypeDef)pClient->FatfsClient.fsres;
		goto exit;
	}
	else {
		/* SDCard读取文件成功 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Read File OK >\r\n");
#endif
	}
	
exit:
	return SDCardStatus;
}

/**********************************************************************************************************
 @Function			SDCard_StatusTypeDef SDCard_Client_FlushesFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File)
 @Description			SDCard_Client_FlushesFile				: SDCard刷新文件缓冲区
 @Input				pClient								: SDCard客户端实例
					File									: SDCard文件实例
 @Return				SDCard_StatusTypeDef					: SDCard执行结果
**********************************************************************************************************/
SDCard_StatusTypeDef SDCard_Client_FlushesFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File)
{
	SDCard_StatusTypeDef SDCardStatus = SDCARD_OK;
	
	if ((SDCardStatus = SDCard_Client_CheckRegistersFilesystem(pClient)) != SDCARD_OK) goto exit;
	
	/* SDCard刷新文件缓冲区 */
	pClient->FatfsClient.fsres = f_sync(&File->fp);
	
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
	SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Flushes File %s: %d >\r\n", File->path, pClient->FatfsClient.fsres);
#endif
	
	SDCardStatus = SDCard_Client_FileAndDirectoryErrorReport(pClient);
	
	if (FR_OK != pClient->FatfsClient.fsres) {
		/* SDCard刷新文件缓冲区失败 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Flushes File Fail >\r\n");
#endif
		SDCardStatus = (SDCard_StatusTypeDef)pClient->FatfsClient.fsres;
		goto exit;
	}
	else {
		/* SDCard刷新文件缓冲区成功 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Flushes File OK >\r\n");
#endif
	}
	
exit:
	return SDCardStatus;
}

/**********************************************************************************************************
 @Function			SDCard_StatusTypeDef SDCard_Client_TruncateFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File)
 @Description			SDCard_Client_TruncateFile				: SDCard截断文件
 @Input				pClient								: SDCard客户端实例
					File									: SDCard文件实例
 @Return				SDCard_StatusTypeDef					: SDCard执行结果
**********************************************************************************************************/
SDCard_StatusTypeDef SDCard_Client_TruncateFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File)
{
	SDCard_StatusTypeDef SDCardStatus = SDCARD_OK;
	
	if ((SDCardStatus = SDCard_Client_CheckRegistersFilesystem(pClient)) != SDCARD_OK) goto exit;
	
	/* SDCard截断文件 */
	pClient->FatfsClient.fsres = f_truncate(&File->fp);
	
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
	SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Truncate File %s: %d >\r\n", File->path, pClient->FatfsClient.fsres);
#endif
	
	if (FR_OK != pClient->FatfsClient.fsres) {
		/* SDCard截断文件失败 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Truncate File Fail >\r\n");
#endif
		SDCardStatus = (SDCard_StatusTypeDef)pClient->FatfsClient.fsres;
		goto exit;
	}
	else {
		/* SDCard截断文件成功 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Truncate File OK >\r\n");
#endif
	}
	
exit:
	return SDCardStatus;
}

/**********************************************************************************************************
 @Function			SDCard_StatusTypeDef SDCard_Client_LseekFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File)
 @Description			SDCard_Client_LseekFile					: SDCard移动文件读写指针
 @Input				pClient								: SDCard客户端实例
					File									: SDCard文件实例
 @Return				SDCard_StatusTypeDef					: SDCard执行结果
**********************************************************************************************************/
SDCard_StatusTypeDef SDCard_Client_LseekFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File)
{
	SDCard_StatusTypeDef SDCardStatus = SDCARD_OK;
	
	if ((SDCardStatus = SDCard_Client_CheckRegistersFilesystem(pClient)) != SDCARD_OK) goto exit;
	
	/* SDCard移动文件读写指针 */
	pClient->FatfsClient.fsres = f_lseek(&File->fp, File->fnofs);
	
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
	SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Lseek File %s: %d, lseek: %lld >\r\n", File->path, pClient->FatfsClient.fsres, File->fnofs);
#endif
	
	SDCardStatus = SDCard_Client_FileAndDirectoryErrorReport(pClient);
	
	if (FR_OK != pClient->FatfsClient.fsres) {
		/* SDCard移动文件读写指针失败 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Lseek File Fail >\r\n");
#endif
		SDCardStatus = (SDCard_StatusTypeDef)pClient->FatfsClient.fsres;
		goto exit;
	}
	else {
		/* SDCard移动文件读写指针成功 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Lseek File OK >\r\n");
#endif
	}
	
exit:
	return SDCardStatus;
}

/**********************************************************************************************************
 @Function			SDCard_StatusTypeDef SDCard_Client_ParameterFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File)
 @Description			SDCard_Client_ParameterFile				: SDCard获取文件参数
 @Input				pClient								: SDCard客户端实例
					File									: SDCard文件实例
 @Return				SDCard_StatusTypeDef					: SDCard执行结果
**********************************************************************************************************/
SDCard_StatusTypeDef SDCard_Client_ParameterFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File)
{
	SDCard_StatusTypeDef SDCardStatus = SDCARD_OK;
	
	if ((SDCardStatus = SDCard_Client_CheckRegistersFilesystem(pClient)) != SDCARD_OK) goto exit;
	
	/* SDCard获取文件参数 */
	File->fsize = f_size(&File->fp);
	File->ftell = f_tell(&File->fp);
	
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
	SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card File Parameter Size: %lld, Tell: %lld >\r\n", File->fsize, File->ftell);
#endif
	
exit:
	return SDCardStatus;
}

/**********************************************************************************************************
 @Function			SDCard_StatusTypeDef SDCard_Client_PutchrFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File)
 @Description			SDCard_Client_PutchrFile					: SDCard写入字符
 @Input				pClient								: SDCard客户端实例
					File									: SDCard文件实例
 @Return				SDCard_StatusTypeDef					: SDCard执行结果
**********************************************************************************************************/
SDCard_StatusTypeDef SDCard_Client_PutchrFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File)
{
	SDCard_StatusTypeDef SDCardStatus = SDCARD_OK;
	int rc;
	
	if ((SDCardStatus = SDCard_Client_CheckRegistersFilesystem(pClient)) != SDCARD_OK) goto exit;
	
	/* SDCard写入字符 */
	rc = f_putc(File->fchr, &File->fp);
	
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
	SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Putc File %s, f_putc:%d >\r\n", File->path, rc);
#endif
	
	if (rc == -1) {
		/* SDCard写入字符失败 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Putc File Fail >\r\n");
#endif
		SDCardStatus = SDCARD_ERROR;
		goto exit;
	}
	else {
		/* SDCard写入字符成功 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Putc File OK >\r\n");
#endif
		File->fcont = rc;
	}
	
exit:
	return SDCardStatus;
}

/**********************************************************************************************************
 @Function			SDCard_StatusTypeDef SDCard_Client_PutstrFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File)
 @Description			SDCard_Client_PutstrFile					: SDCard写入字符串
 @Input				pClient								: SDCard客户端实例
					File									: SDCard文件实例
 @Return				SDCard_StatusTypeDef					: SDCard执行结果
**********************************************************************************************************/
SDCard_StatusTypeDef SDCard_Client_PutstrFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File)
{
	SDCard_StatusTypeDef SDCardStatus = SDCARD_OK;
	int rc;
	
	if ((SDCardStatus = SDCard_Client_CheckRegistersFilesystem(pClient)) != SDCARD_OK) goto exit;
	
	if ((File->fbuf == NULL) || (File->flen == 0)) {
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Puts File Paramete Fail >\r\n");
#endif
		SDCardStatus = SDCARD_ERROR;
		goto exit;
	}
	
	/* SDCard写入字符串 */
	rc = f_puts(File->fbuf, &File->fp);
	
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
	SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Puts File %s, f_puts:%d >\r\n", File->path, rc);
#endif
	
	if (rc == -1) {
		/* SDCard写入字符串失败 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Puts File Fail >\r\n");
#endif
		SDCardStatus = SDCARD_ERROR;
		goto exit;
	}
	else {
		/* SDCard写入字符串成功 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Puts File OK >\r\n");
#endif
		File->fcont = rc;
	}
	
exit:
	return SDCardStatus;
}

/**********************************************************************************************************
 @Function			SDCard_StatusTypeDef SDCard_Client_GetstrFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File)
 @Description			SDCard_Client_GetstrFile					: SDCard读出字符串
 @Input				pClient								: SDCard客户端实例
					File									: SDCard文件实例
 @Return				SDCard_StatusTypeDef					: SDCard执行结果
**********************************************************************************************************/
SDCard_StatusTypeDef SDCard_Client_GetstrFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File)
{
	SDCard_StatusTypeDef SDCardStatus = SDCARD_OK;
	int rc = 0;
	
	if ((SDCardStatus = SDCard_Client_CheckRegistersFilesystem(pClient)) != SDCARD_OK) goto exit;
	
	if ((File->fbuf == NULL) || (File->flen == 0)) {
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Puts File Paramete Fail >\r\n");
#endif
		SDCardStatus = SDCARD_ERROR;
		goto exit;
	}
	
	/* SDCard读出字符串 */
	f_gets(File->fbuf, File->flen, &File->fp);
	
	rc = strlen(File->fbuf);
	
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
	SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Gets File %s, f_gets:%d >\r\n", File->path, rc);
#endif
	
	if (rc == 0) {
		/* SDCard读出字符串失败 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Gets File Fail >\r\n");
#endif
		SDCardStatus = SDCARD_ERROR;
		goto exit;
	}
	else {
		/* SDCard读出字符串成功 */
#ifdef SDCARD_DEBUG_LOG_RF_PRINT
		SDCARD_DEBUG_LOG_PRINTF("\r\n[SD Card Info:]<! Card Gets File OK >\r\n");
#endif
		File->fcont = rc;
	}
	
exit:
	return SDCardStatus;
}






/********************************************** END OF FLEE **********************************************/
