#ifndef __SOCKET_MODULATION_FUNC_H
#define   __SOCKET_MODULATION_FUNC_H

#include "stm32f10x_lib.h"
#include "socketmodulationconfig.h"

extern Socket_Modulation_Data_Frame			SocketModulationDataFrame;		//SocketModulation数据帧
extern volatile u8 SocketModulationSendBuf[SOCKET_MODULATION_CACHE_SIZE];			//SocketModulation发送数据缓存
extern volatile u8 SocketModulationReceiveBuf[SOCKET_MODULATION_RECVIVE_SIZE];		//SocketModulation接收数据缓存


u16  SOCKET_Modulation_ObtainDataFrame(u8 *addr);								//读取SocketModulation数据帧并填入缓存
void SOCKET_Modulation_ObtainDataFrameCheckCode(u8 *addr, u16 buflength);			//计算校验码并填入校验码


u8   SOCKET_Modulation_Convert2Byte_Little_endian(u8 *addr, u16 data);			//将2Byte数据转换并填入缓存(小端)
u8   SOCKET_Modulation_Convert2Byte_Big_endian(u8 *addr, u16 data);				//将2Byte数据转换并填入缓存(大端)
u8   SOCKET_Modulation_Convert4Byte_Little_endian(u8 *addr, u32 data);			//将4Byte数据转换并填入缓存(小端)
u8   SOCKET_Modulation_Convert4Byte_Big_endian(u8 *addr, u32 data);				//将4Byte数据转换并填入缓存(大端)
u8   SOCKET_Modulation_ConvertNByte(u8 *addr, u8 *data, u16 len);				//将NByte数据填入缓存
u16  SOCKET_Modulation_CRC16(unsigned char *puchMsg, unsigned short usDataLen);		//CRC校验

void SOCKET_Modulation_GetOutputID(u16 *outputid);							//读取output_ID输出端口的参数到SocketModulation车道数据包
u16  SOCKET_Modulation_PackagedStorage(void);								//SocketModulation将数据打包存入缓存区

#endif
