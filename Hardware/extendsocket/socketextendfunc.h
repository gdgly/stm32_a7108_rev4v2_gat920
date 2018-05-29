#ifndef __SOCKET_EXTEND_FUNC_H
#define   __SOCKET_EXTEND_FUNC_H

#include "stm32f10x_lib.h"
#include "socketextendconfig.h"

extern Socket_Extend_Packet_Head			SocketExtendHeadPacket;				//SocketExtend数据包头
extern Socket_Extend_Packet_Data			SocketExtendDataPacket[OUTPUT_MAX];	//SocketExtend流量数据包集
extern volatile u8 SocketExtendSendBuf[SOCKET_EXTEND_CACHE_SIZE];				//SocketExtend发送数据缓存
extern volatile u8 SocketExtendReceiveBuf[SOCKET_EXTEND_RECVIVE_SIZE];			//SocketExtend接收数据缓存


u16  SOCKET_Extend_ObtainPacketHead(u8 *addr);								//读取SocketExtend包头数据并填入缓存
u16  SOCKET_Extend_ObtainPacketData(u8 *addr);								//读取SocketExtend流量数据包并填入缓存
void SOCKET_Extend_ObtainPacketCheckCode(u8 *addr, u16 buflength);				//计算校验码并填入校验码


u8   SOCKET_Extend_ConvertCrossID(u8 *addr, u32 crossid);						//将crossid转换为字符格式并填入缓存
u8   SOCKET_Extend_ConvertDirverWayCode(u8 *addr, u32 dirverWayCode);				//将dirverWayCode转换为字符格式并填入缓存
u8   SOCKET_Extend_Convert2Byte(u8 *addr, u16 data);							//将2Byte数据转换并填入缓存
u8   SOCKET_Extend_Convert4Byte(u8 *addr, float data);							//将float数据转换并填入缓存
u8   SOCKET_Extend_Convert4ByteInt(u8 *addr, u32 data);						//将Int数据转换并填入缓存

void SOCKET_Extend_GetOutputID(u16 *outputid);								//读取output_ID输出端口的参数到SocketExtend流量数据包
u16  SOCKET_Extend_PackagedStorage(void);									//SocketExtend将数据打包存入缓存区
u16  SOCKET_Extend_RtcCheckPackagedStorage(void);								//SocketExtend将对时数据打包存入缓存区

#endif
