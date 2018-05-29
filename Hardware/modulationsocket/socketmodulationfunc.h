#ifndef __SOCKET_MODULATION_FUNC_H
#define   __SOCKET_MODULATION_FUNC_H

#include "stm32f10x_lib.h"
#include "socketmodulationconfig.h"

extern Socket_Modulation_Data_Frame			SocketModulationDataFrame;		//SocketModulation����֡
extern volatile u8 SocketModulationSendBuf[SOCKET_MODULATION_CACHE_SIZE];			//SocketModulation�������ݻ���
extern volatile u8 SocketModulationReceiveBuf[SOCKET_MODULATION_RECVIVE_SIZE];		//SocketModulation�������ݻ���


u16  SOCKET_Modulation_ObtainDataFrame(u8 *addr);								//��ȡSocketModulation����֡�����뻺��
void SOCKET_Modulation_ObtainDataFrameCheckCode(u8 *addr, u16 buflength);			//����У���벢����У����


u8   SOCKET_Modulation_Convert2Byte_Little_endian(u8 *addr, u16 data);			//��2Byte����ת�������뻺��(С��)
u8   SOCKET_Modulation_Convert2Byte_Big_endian(u8 *addr, u16 data);				//��2Byte����ת�������뻺��(���)
u8   SOCKET_Modulation_Convert4Byte_Little_endian(u8 *addr, u32 data);			//��4Byte����ת�������뻺��(С��)
u8   SOCKET_Modulation_Convert4Byte_Big_endian(u8 *addr, u32 data);				//��4Byte����ת�������뻺��(���)
u8   SOCKET_Modulation_ConvertNByte(u8 *addr, u8 *data, u16 len);				//��NByte�������뻺��
u16  SOCKET_Modulation_CRC16(unsigned char *puchMsg, unsigned short usDataLen);		//CRCУ��

void SOCKET_Modulation_GetOutputID(u16 *outputid);							//��ȡoutput_ID����˿ڵĲ�����SocketModulation�������ݰ�
u16  SOCKET_Modulation_PackagedStorage(void);								//SocketModulation�����ݴ�����뻺����

#endif
