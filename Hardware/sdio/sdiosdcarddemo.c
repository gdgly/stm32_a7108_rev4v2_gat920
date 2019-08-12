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

#include "sdiosdcarddemo.h"
#include "sdiosdcard.h"
#include "bsp_usart.h"

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

#define BLOCK_SIZE				512
#define NUMBER_OF_BLOCKS			4
#define MULTI_BUFFER_SIZE		(BLOCK_SIZE * NUMBER_OF_BLOCKS)

SD_Error Status = SD_OK;
volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;

uint8_t Buffer_MultiBlock_Tx[MULTI_BUFFER_SIZE], Buffer_MultiBlock_Rx[MULTI_BUFFER_SIZE];
uint8_t Buffer_Block_Tx[BLOCK_SIZE], Buffer_Block_Rx[BLOCK_SIZE];

void SD_EraseTest(void);
void SD_SingleBlockTest(void);
void SD_MultiBlockTest(void);

void Fill_Buffer(uint8_t* pBuffer, uint32_t BufferLength, uint32_t Offset);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);
TestStatus BuffercmpisZero(uint8_t* pBuffer, uint32_t BufferLength);

/**********************************************************************************************************
 @Function			void SD_SDIO_Demo(void)
 @Description			SD_SDIO_Demo										: SDIO SD Card 测试
 @Input				void
 @Return				void
**********************************************************************************************************/
void SD_SDIO_Demo(void)
{
	RS485_Mode_TX();
	
	printf("\r\nThis is SD Card 1bit dma Demo!\r\n");
	
	Status = SD_Init();
	
	if (Status == SD_OK) {
		printf("\r\nSD Card Init Success!\r\n");
	}
	else {
		printf("\r\nSD Card Init Fail! Status = %d.\r\n", Status);
	}
	
	printf("\r\nCardType is : %d ", SDCardInfo.CardType);
	printf("\r\nCardDeviceSize is : %d ", SDCardInfo.SD_csd.DeviceSize);
	printf("\r\nCardCapacity is : %lldM (%.2fG) ", SDCardInfo.CardCapacity / 1024 / 1024, (double)SDCardInfo.CardCapacity / 1024.0 / 1024 / 1024);
	printf("\r\nCardBlockSize is : %d ", SDCardInfo.CardBlockSize);
	printf("\r\nRCA is : %d ", SDCardInfo.RCA);
	printf("\r\nManufacturerID is : %d \r\n", SDCardInfo.SD_cid.ManufacturerID);
	
	SD_EraseTest();
	
	SD_SingleBlockTest();
	
	SD_MultiBlockTest();
}

/**********************************************************************************************************
 @Function			void SD_EraseTest(void)
 @Description			SD_EraseTest										: SD Card 擦除测试
 @Input				void
 @Return				void
**********************************************************************************************************/
void SD_EraseTest(void)
{
	if (Status == SD_OK) {
		Status = SD_Erase(0x00, (BLOCK_SIZE * NUMBER_OF_BLOCKS));
	}
	
	if (Status == SD_OK) {
		Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
		Status = SD_WaitReadOperation();
		while (SD_GetStatus() != SD_TRANSFER_OK);
	}
	
	if (Status == SD_OK) {
		EraseStatus = BuffercmpisZero(Buffer_MultiBlock_Rx, MULTI_BUFFER_SIZE);
	}
	
	if (EraseStatus == PASSED) {
		printf("\r\nSD Card Erase Success!\r\n");
	}
	else {
		printf("\r\nSD Card Erase Fail!\r\n");
	}
}

/**********************************************************************************************************
 @Function			void SD_SingleBlockTest(void)
 @Description			SD_SingleBlockTest									: SD Card 单数据块读写测试
 @Input				void
 @Return				void
**********************************************************************************************************/
void SD_SingleBlockTest(void)
{
	Fill_Buffer(Buffer_Block_Tx, BLOCK_SIZE, 0x320F);
	
	if (Status == SD_OK) {
		Status = SD_WriteBlock(Buffer_Block_Tx, 0x00, BLOCK_SIZE);
		
		Status = SD_WaitWriteOperation();
		
		while(SD_GetStatus() != SD_TRANSFER_OK);
	}
	
	if (Status == SD_OK) {
		Status = SD_ReadBlock(Buffer_Block_Rx, 0x00, BLOCK_SIZE);
		
		Status = SD_WaitReadOperation();
		
		while(SD_GetStatus() != SD_TRANSFER_OK);
	}
	
	if (Status == SD_OK) {
		TransferStatus1 = Buffercmp(Buffer_Block_Tx, Buffer_Block_Rx, BLOCK_SIZE);
	}
	
	if(TransferStatus1 == PASSED) {
		printf("\r\nSD Card Single Block Test Success!\r\n");
	}
	else {
		printf("\r\nSD Card Single Block Test Fail!\r\n");
	}
}

/**********************************************************************************************************
 @Function			void SD_MultiBlockTest(void)
 @Description			SD_MultiBlockTest									: SD Card 多数据块读写测试
 @Input				void
 @Return				void
**********************************************************************************************************/
void SD_MultiBlockTest(void)
{
	Fill_Buffer(Buffer_MultiBlock_Tx, MULTI_BUFFER_SIZE, 0x00);
	
	if (Status == SD_OK) {
		Status = SD_WriteMultiBlocks(Buffer_MultiBlock_Tx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
		
		Status = SD_WaitWriteOperation();
		
		while(SD_GetStatus() != SD_TRANSFER_OK);
	}
	
	if (Status == SD_OK) {
		Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
		
		Status = SD_WaitReadOperation();
		
		while(SD_GetStatus() != SD_TRANSFER_OK);
	}
	
	if (Status == SD_OK) {
		TransferStatus2 = Buffercmp(Buffer_MultiBlock_Tx, Buffer_MultiBlock_Rx, MULTI_BUFFER_SIZE);
	}
	
	if(TransferStatus1 == PASSED) {
		printf("\r\nSD Card Multi Block Test Success!\r\n");
	}
	else {
		printf("\r\nSD Card Multi Block Test Fail!\r\n");
	}
}

/**********************************************************************************************************
 @Function			void Fill_Buffer(uint8_t* pBuffer, uint32_t BufferLength, uint32_t Offset)
 @Description			Fill_Buffer										: 缓冲区填写数据
 @Input				void
 @Return				void
**********************************************************************************************************/
void Fill_Buffer(uint8_t* pBuffer, uint32_t BufferLength, uint32_t Offset)
{
	uint16_t index = 0;
	
	for (index = 0; index < BufferLength; index++) {
		pBuffer[index] = index + Offset;
	}
}

/**********************************************************************************************************
 @Function			TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
 @Description			Buffercmp											: 比较缓存区数据是否相等
 @Input				void
 @Return				void
**********************************************************************************************************/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
{
	while (BufferLength--) {
		if (*pBuffer1 != *pBuffer2) {
			return FAILED;
		}
		
		pBuffer1++;
		pBuffer2++;
	}
	
	return PASSED;
}

/**********************************************************************************************************
 @Function			TestStatus BuffercmpisZero(uint8_t* pBuffer, uint32_t BufferLength)
@Description			BuffercmpisZero									: 比较缓存区数据是为0
 @Input				void
 @Return				void
**********************************************************************************************************/
TestStatus BuffercmpisZero(uint8_t* pBuffer, uint32_t BufferLength)
{
	while (BufferLength--) {
		if ((*pBuffer != 0xFF) && (*pBuffer != 0x00)) {
			return FAILED;
		}
		
		pBuffer++;
	}
	
	return PASSED;
}

/********************************************** END OF FLEE **********************************************/
