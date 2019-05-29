#ifndef __FIFO_MESSAGE_H
#define   __FIFO_MESSAGE_H

#include "stm32f10x_lib.h"
#include "platform_config.h"
#include "stdbool.h"

struct DataFIFO
{
	unsigned char* buf;
	int putP, getP, size, free;
};

typedef struct
{
	struct DataFIFO					dataFifo;
	unsigned char						Front;
	unsigned char						Rear;
	unsigned char						ParkNum;
	unsigned char						ParkFree;
}MessageFifoTypeDef;

void Fifo_init(struct DataFIFO *fifo, int size, unsigned char *buf);										//初始化Fifo
int  Fifo_putPut(struct DataFIFO *fifo, unsigned char data);											//向Fifo中写入1个字节
int  Fifo_get(struct DataFIFO *fifo);																//从Fifo中读取1个字节
void Fifo_WriteBuffer(struct DataFIFO *fifo, unsigned char* buf, unsigned int len);							//向Fifo中写入数据
void Fifo_ReadBuffer(struct DataFIFO *fifo, unsigned char* buf, unsigned int len);							//从Fifo中读取数据
void Fifo_ReadOnlyBuffer(struct DataFIFO *fifo, unsigned char* buf, unsigned int offset, unsigned int len);		//从Fifo中读取数据(只读)
int  Fifo_status(struct DataFIFO *fifo);															//Fifo已用大小
int  Fifo_free(struct DataFIFO *fifo);																//Fifo剩余大小

void netMessageFifoInit(MessageFifoTypeDef *pMessageFifo, unsigned char *buf, int size, unsigned char parkNum);	//初始化消息队列
bool netMessageFifoisFull(MessageFifoTypeDef *pMessageFifo, int writtenLen);								//消息队列是否已满
bool netMessageFifoisEmpty(MessageFifoTypeDef *pMessageFifo);											//消息队列是否已空
bool netMessageFifoDiscard(MessageFifoTypeDef *pMessageFifo);											//丢弃最早一包消息队列数据
bool netMessageFifoEnqueue(MessageFifoTypeDef *pMessageFifo, unsigned char* buf, unsigned short len);			//数据写入消息队列
bool netMessageFifoDequeue(MessageFifoTypeDef *pMessageFifo, unsigned char* buf, unsigned short* len);			//数据读出消息队列
unsigned char netMessageFifoFront(MessageFifoTypeDef *pMessageFifo);										//获取消息队列队头值
unsigned char netMessageFifoRear(MessageFifoTypeDef *pMessageFifo);										//获取消息队列队尾值

#endif /* __FIFO_MESSAGE_H */
