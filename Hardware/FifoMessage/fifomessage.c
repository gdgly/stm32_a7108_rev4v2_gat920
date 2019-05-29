/**
  *********************************************************************************************************
  * @file    fifomessage.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2018-12-14
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "fifomessage.h"

/**********************************************************************************************************
 @Function			void Fifo_init(struct DataFIFO *fifo, int size, unsigned char *buf)
 @Description			Fifo_init					: 初始化Fifo
 @Input				fifo						: Fifo结构体地址
					size						: Fifo内存大小
					buf						: Fifo内存地址
 @Return				void
**********************************************************************************************************/
void Fifo_init(struct DataFIFO *fifo, int size, unsigned char *buf)
{
	fifo->buf = buf;
	fifo->free = size;
	fifo->size = size;
	fifo->putP = 0;
	fifo->getP = 0;
	
	return;
}

/**********************************************************************************************************
 @Function			int Fifo_putPut(struct DataFIFO *fifo, unsigned char data)
 @Description			Fifo_putPut				: 向Fifo中写入1个字节
 @Input				fifo						: Fifo结构体地址
					data						: 写入字节
 @Return				0						: 成功
					-1						: 失败
**********************************************************************************************************/
int Fifo_putPut(struct DataFIFO *fifo, unsigned char data)
{
	if (fifo->free == 0) {
		return -1;
	}
	
	fifo->buf[fifo->putP] = data;
	fifo->putP++;
	if (fifo->putP == fifo->size) {
		fifo->putP = 0;
	}
	fifo->free--;
	
	return 0;
}

/**********************************************************************************************************
 @Function			int Fifo_get(struct DataFIFO *fifo)
 @Description			Fifo_get					: 从Fifo中读取1个字节
 @Input				fifo						: Fifo结构体地址
 @Return				data						: 读取字节
					-1						: 失败
**********************************************************************************************************/
int Fifo_get(struct DataFIFO *fifo)
{
	unsigned char data;
	
	if (fifo->free == fifo->size) {
		return -1;
	}
	
	data = fifo->buf[fifo->getP];
	fifo->getP++;
	if (fifo->getP == fifo->size) {
		fifo->getP = 0;
	}
	fifo->free++;
	
	return data;
}

/**********************************************************************************************************
 @Function			void Fifo_WriteBuffer(struct DataFIFO *fifo, unsigned char* buf, unsigned int len)
 @Description			Fifo_WriteBuffer			: 向Fifo中写入数据
 @Input				fifo						: Fifo结构体地址
					buf						: 写入数据地址
					len						: 写入数据长度
 @Return				void
**********************************************************************************************************/
void Fifo_WriteBuffer(struct DataFIFO *fifo, unsigned char* buf, unsigned int len)
{
	while (len--) {
		Fifo_putPut(fifo, *buf++);
	}
}

/**********************************************************************************************************
 @Function			void Fifo_ReadBuffer(struct DataFIFO *fifo, unsigned char* buf, unsigned int len)
 @Description			Fifo_ReadBuffer			: 从Fifo中读取数据
 @Input				fifo						: Fifo结构体地址
					buf						: 读取数据地址
					len						: 读取数据长度
 @Return				void
**********************************************************************************************************/
void Fifo_ReadBuffer(struct DataFIFO *fifo, unsigned char* buf, unsigned int len)
{
	while (len--) {
		*buf++ = Fifo_get(fifo);
	}
}

/**********************************************************************************************************
 @Function			void Fifo_ReadOnlyBuffer(struct DataFIFO *fifo, unsigned char* buf, unsigned int offset, unsigned int len)
 @Description			Fifo_ReadOnlyBuffer			: 从Fifo中读取数据(只读)
 @Input				fifo						: Fifo结构体地址
					buf						: 读取数据地址
					offset					: 数据偏移地址
					len						: 读取数据长度
 @Return				void
**********************************************************************************************************/
void Fifo_ReadOnlyBuffer(struct DataFIFO *fifo, unsigned char* buf, unsigned int offset, unsigned int len)
{
	int getP = fifo->getP;
	
	while(offset--) {
		getP++;
		if (getP == fifo->size) {
			getP = 0;
		}
	}

	while (len--) {
		*buf++ = fifo->buf[getP];
		getP++;
		if (getP == fifo->size) {
			getP = 0;
		}
	}
}

/**********************************************************************************************************
 @Function			int Fifo_status(struct DataFIFO *fifo)
 @Description			Fifo_status				: Fifo已用大小
 @Input				fifo						: Fifo结构体地址
 @Return				int						: Fifo已用大小
**********************************************************************************************************/
int Fifo_status(struct DataFIFO *fifo)
{
	return fifo->size-fifo->free;
}

/**********************************************************************************************************
 @Function			int Fifo_free(struct DataFIFO *fifo)
 @Description			Fifo_free					: Fifo剩余大小
 @Input				fifo						: Fifo结构体地址
 @Return				int						: Fifo剩余大小
**********************************************************************************************************/
int Fifo_free(struct DataFIFO *fifo)
{
	return fifo->free;
}

/**********************************************************************************************************
 @Function			void netMessageFifoInit(MessageFifoTypeDef *pMessageFifo, unsigned char *buf, int size, unsigned char parkNum)
 @Description			netMessageFifoInit				: 初始化消息队列
 @Input				pMessageFifo					: FiFo队列地址
					buf							: FiFo队列数据存储地址
					size							: FiFo队列数据存储大小
					parkNum						: 消息存储最大包数
 @Return				void
**********************************************************************************************************/
void netMessageFifoInit(MessageFifoTypeDef *pMessageFifo, unsigned char *buf, int size, unsigned char parkNum)
{
	Fifo_init(&pMessageFifo->dataFifo, size, buf);

	pMessageFifo->Front = 0;
	pMessageFifo->Rear = 0;
	pMessageFifo->ParkNum = parkNum;
	pMessageFifo->ParkFree = parkNum;
}

/**********************************************************************************************************
 @Function			bool netMessageFifoisFull(MessageFifoTypeDef *pMessageFifo, int writtenLen)
 @Description			netMessageFifoisFull			: 消息队列是否已满
 @Input				pMessageFifo					: FiFo队列地址
					writtenLen					: 将要写入FiFo数据大小
 @Return				true							: 已满
					false						: 未满
**********************************************************************************************************/
bool netMessageFifoisFull(MessageFifoTypeDef *pMessageFifo, int writtenLen)
{
	if ((writtenLen + 2) > Fifo_free(&pMessageFifo->dataFifo)) {
		return true;
	}

	if (((pMessageFifo->Rear + 1) % pMessageFifo->ParkNum) == pMessageFifo->Front) {
		return true;
	}

	return false;
}

/**********************************************************************************************************
 @Function			bool netMessageFifoisEmpty(MessageFifoTypeDef *pMessageFifo)
 @Description			netMessageFifoisEmpty			: 消息队列是否已空
 @Input				pMessageFifo					: FiFo队列地址
 @Return				true							: 已空
					false						: 未空
**********************************************************************************************************/
bool netMessageFifoisEmpty(MessageFifoTypeDef *pMessageFifo)
{
	if (Fifo_status(&pMessageFifo->dataFifo) == 0) {
		return true;
	}

	if (pMessageFifo->Front == pMessageFifo->Rear) {
		return true;
	}

	return false;
}

/**********************************************************************************************************
 @Function			bool netMessageFifoDiscard(MessageFifoTypeDef *pMessageFifo)
 @Description			netMessageFifoDiscard			: 丢弃最早一包消息队列数据
 @Input				pMessageFifo					: FiFo队列地址
 @Return				true							: 丢弃成功
					false						: 丢弃失败
**********************************************************************************************************/
bool netMessageFifoDiscard(MessageFifoTypeDef *pMessageFifo)
{
	unsigned char lenbuf[2];
	unsigned short len = 0;

	if (netMessageFifoisEmpty(pMessageFifo) == true) {
		return false;
	}

	/* step1 : 读取消息数据前2字节的数据长度 */
	Fifo_ReadBuffer(&pMessageFifo->dataFifo, lenbuf, 2);
	len = lenbuf[0];
	len |= lenbuf[1] << 8;

	/* step2 : 丢弃队列有效消息数据 */
	while (len--) {
		Fifo_get(&pMessageFifo->dataFifo);
	}

	/* step3 : 数据包处理 */
	pMessageFifo->Front = (pMessageFifo->Front + 1) % pMessageFifo->ParkNum;
	pMessageFifo->ParkFree++;

	return true;
}

/**********************************************************************************************************
 @Function			bool netMessageFifoEnqueue(MessageFifoTypeDef *pMessageFifo, unsigned char* buf, unsigned short len)
 @Description			netMessageFifoEnqueue			: 数据写入消息队列
 @Input				pMessageFifo					: FiFo队列地址
					buf							: 需写入数据地址
					len							: 需写入数据长度
 @Return				true							: 写入成功
					false						: 写入失败
**********************************************************************************************************/
bool netMessageFifoEnqueue(MessageFifoTypeDef *pMessageFifo, unsigned char* buf, unsigned short len)
{
	unsigned char lenbuf[2];

	if ((len + 2) > pMessageFifo->dataFifo.size) {
		return false;
	}

	/* step1 : 腾出需写入数据空间 */
	while (netMessageFifoisFull(pMessageFifo, len) == true) {
		netMessageFifoDiscard(pMessageFifo);
	}

	/* step2 : 写入消息数据前2字节的数据长度 */
	lenbuf[0] = len & 0xFF;
	lenbuf[1] = (len >> 8) & 0xFF;
	Fifo_WriteBuffer(&pMessageFifo->dataFifo, lenbuf, 2);

	/* step3 : 写入队列有效消息数据 */
	Fifo_WriteBuffer(&pMessageFifo->dataFifo, buf, len);

	/* step4 : 数据包处理 */
	pMessageFifo->Rear = (pMessageFifo->Rear + 1) % pMessageFifo->ParkNum;
	pMessageFifo->ParkFree--;

	return true;
}

/**********************************************************************************************************
 @Function			bool netMessageFifoDequeue(MessageFifoTypeDef *pMessageFifo, unsigned char* buf, unsigned short* len)
 @Description			netMessageFifoDequeue			: 数据读出消息队列
 @Input				pMessageFifo					: FiFo队列地址
					buf							: 需读出数据地址
					len							: 需读出数据长度地址
 @Return				true							: 读取成功
					false						: 读取失败
**********************************************************************************************************/
bool netMessageFifoDequeue(MessageFifoTypeDef *pMessageFifo, unsigned char* buf, unsigned short* len)
{
	unsigned char lenbuf[2];

	if (netMessageFifoisEmpty(pMessageFifo) == true) {
		return false;
	}

	/* step1 : 读取消息数据前2字节的数据长度 */
	Fifo_ReadOnlyBuffer(&pMessageFifo->dataFifo, lenbuf, 0, 2);
	*len = lenbuf[0];
	*len |= lenbuf[1] << 8;

	/* step2 : 读取队列有效消息数据 */
	Fifo_ReadOnlyBuffer(&pMessageFifo->dataFifo, buf, 2, *len);

	return true;
}

/**********************************************************************************************************
 @Function			unsigned char netMessageFifoFront(MessageFifoTypeDef *pMessageFifo)
 @Description			netMessageFifoFront				: 获取消息队列队头值
 @Input				pMessageFifo					: FiFo队列地址
 @Return				消息队列队头值
**********************************************************************************************************/
unsigned char netMessageFifoFront(MessageFifoTypeDef *pMessageFifo)
{
	return pMessageFifo->Front;
}

/**********************************************************************************************************
 @Function			unsigned char netMessageFifoRear(MessageFifoTypeDef *pMessageFifo)
 @Description			netMessageFifoRear				: 获取消息队列队尾值
 @Input				pMessageFifo					: FiFo队列地址
 @Return				消息队列队尾值
**********************************************************************************************************/
unsigned char netMessageFifoRear(MessageFifoTypeDef *pMessageFifo)
{
	return pMessageFifo->Rear;
}

/********************************************** END OF FLEE **********************************************/
