#ifndef __SDIO_SDCARD_USER_APP_H
#define   __SDIO_SDCARD_USER_APP_H

#include "stm32f10x_lib.h"
#include "platform_config.h"
#include "stdbool.h"
#include "ff.h"

#define FATFS_DIRECTORY					2
#define FATFS_FILE						2

#define SDCARD_DATASTACK_SIZE				512
#define SDCARD_WRITESTACK_SIZE			1024

#define FATFS_DIRECTORY_LOGDIR_INDEX		0
#define FATFS_DIRECTORY_LOGDIR_PATH		"LOG"

#define FATFS_DIRECTORY_DATDIR_INDEX		1
#define FATFS_DIRECTORY_DATDIR_PATH		"DAT"

#define FATFS_FILE_LOGDIR_INDEX			0
#define FATFS_FILE_LOGDIR_PATH			"LOG/LOGNormal.log"

#define FATFS_FILE_DATDIR_INDEX			1
#define FATFS_FILE_DATDIR_PATH			"DAT/DATNormal.dat"

typedef struct FATFS_ClientsTypeDef		FATFS_ClientsTypeDef;
typedef struct SDCard_ClientsTypeDef		SDCard_ClientsTypeDef;

/* SDCard Status */
typedef enum
{
	SDCARD_OK							= 0x00,
	SDCARD_ERROR						= 0x01,
	SDCARD_INT_ERR						= 0x02,
	SDCARD_NOT_READY					= 0x03,
	SDCARD_NO_FILE						= 0x04,
	SDCARD_NO_PATH						= 0x05,
	SDCARD_INVALID_NAME					= 0x06,
	SDCARD_DENIED						= 0x07,
	SDCARD_EXIST						= 0x08,
	SDCARD_INVALID_OBJECT				= 0x09,
	SDCARD_WRITE_PROTECTED				= 0x0A,
	SDCARD_INVALID_DRIVE				= 0x0B,
	SDCARD_NOT_ENABLED					= 0x0C,
	SDCARD_NO_FILESYSTEM				= 0x0D,
	SDCARD_MKFS_ABORTED					= 0x0E,
	SDCARD_TIMEOUT						= 0x0F,
	SDCARD_LOCKED						= 0x10,
	SDCARD_NOT_ENOUGH_CORE				= 0x11,
	SDCARD_TOO_MANY_OPEN_FILES			= 0x12,
	SDCARD_INVALID_PARAMETER				= 0x13
	
	
	
	
	
}SDCard_StatusTypeDef;

/* SDCard Mount */
typedef enum
{
	Unmount							= 0x00,
	Ismount							= 0x01
}SDCard_MountStatusTypeDef;













/* FATFS Parameter */
typedef struct
{
	DWORD							fre_clust;
	DWORD							tot_sect;
	DWORD							fre_sect;
}FATFS_ParameterTypeDef;

/* FATFS Directory */
typedef struct
{
	TCHAR*							path;
	FILINFO							finfo;
}FATFS_DirectoryTypeDef;

/* FATFS File */
typedef struct
{
	TCHAR*							path;
	FILINFO							finfo;
	FIL								fp;
	BYTE								fmode;
	TCHAR							fchr;
	TCHAR*							fbuf;
	UINT								flen;
	UINT								fcont;
	FSIZE_t							fnofs;
	FSIZE_t							fsize;
	FSIZE_t							ftell;
}FATFS_FileTypeDef;







/* FATFS Clients */
struct FATFS_ClientsTypeDef
{
	FATFS							fs;
	FATFS*							pfs;
	BYTE								work[FF_MAX_SS];
	FRESULT							fsres;
};

/* SDCard Clients */
struct SDCard_ClientsTypeDef
{
	SDCard_MountStatusTypeDef			MountState;
	
	unsigned char*						DataProcessStack;
	unsigned short						DataProcessStack_len;
	unsigned int						DataProcessStack_size;
	
	unsigned char*						WriteProcessStack;
	unsigned short						WriteProcessStack_len;
	unsigned int						WriteProcessStack_size;
	
	FATFS_ClientsTypeDef				FatfsClient;
	FATFS_ParameterTypeDef				FatfsParameter;
	FATFS_DirectoryTypeDef				FatfsDirectory[FATFS_DIRECTORY];
	FATFS_FileTypeDef					FatfsFile[FATFS_FILE];
	
	
	
	
	
	/* SDCard运行控制器 */
	struct SDCARDDictateRuningCtlTypeDef
	{
		bool							dictateRegistersFilesystem;
		bool							dictateCreateDirectoryLOGDIR;
		bool							dictateCreateDirectoryDATDIR;
		
		
		
	}DictateRunCtl;
	
	
	
	
	
};







extern SDCard_ClientsTypeDef				SDCardClient;

void SDCard_Client_Init(SDCard_ClientsTypeDef* pClient);

SDCard_StatusTypeDef SDCard_Client_RegistersFilesystem(SDCard_ClientsTypeDef* pClient);												//挂载文件系统
SDCard_StatusTypeDef SDCard_Client_UnregistersFilesystem(SDCard_ClientsTypeDef* pClient);												//卸载文件系统
SDCard_StatusTypeDef SDCard_Client_GetFreeClusters(SDCard_ClientsTypeDef* pClient);													//获取文件系统空间信息

SDCard_StatusTypeDef SDCard_Client_CreateDirectory(SDCard_ClientsTypeDef* pClient, FATFS_DirectoryTypeDef* Dir);							//创建文件夹
SDCard_StatusTypeDef SDCard_Client_ChecksDirectoryorFile(SDCard_ClientsTypeDef* pClient, FATFS_DirectoryTypeDef* Dir);						//读取文件或文件夹信息
SDCard_StatusTypeDef SDCard_Client_ChangeTimestamp(SDCard_ClientsTypeDef* pClient, FATFS_DirectoryTypeDef* Dir);							//改变文件或文件夹时间
SDCard_StatusTypeDef SDCard_Client_RemoveDirectoryorFile(SDCard_ClientsTypeDef* pClient, FATFS_DirectoryTypeDef* Dir);						//删除文件或文件夹

SDCard_StatusTypeDef SDCard_Client_OpensFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File);									//打开文件
SDCard_StatusTypeDef SDCard_Client_ClosesFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File);									//关闭文件

SDCard_StatusTypeDef SDCard_Client_WriteFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File);									//写数据到文件
SDCard_StatusTypeDef SDCard_Client_ReadFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File);									//读数据从文件
SDCard_StatusTypeDef SDCard_Client_FlushesFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File);									//刷新文件缓冲区
SDCard_StatusTypeDef SDCard_Client_TruncateFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File);								//截断文件
SDCard_StatusTypeDef SDCard_Client_LseekFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File);									//移动文件读写指针
SDCard_StatusTypeDef SDCard_Client_ParameterFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File);								//获取文件参数

SDCard_StatusTypeDef SDCard_Client_PutchrFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File);									//写入字符
SDCard_StatusTypeDef SDCard_Client_PutstrFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File);									//写入字符串
SDCard_StatusTypeDef SDCard_Client_GetstrFile(SDCard_ClientsTypeDef* pClient, FATFS_FileTypeDef* File);									//读出字符串





#endif
