#ifndef __SOCKET_EXTEND_FUNC_H
#define   __SOCKET_EXTEND_FUNC_H

#include "stm32f10x_lib.h"
#include "socketextendconfig.h"

extern Socket_Extend_Packet_Head			SocketExtendHeadPacket;				//SocketExtend���ݰ�ͷ
extern Socket_Extend_Packet_Data			SocketExtendDataPacket[OUTPUT_MAX];	//SocketExtend�������ݰ���
extern volatile u8 SocketExtendSendBuf[SOCKET_EXTEND_CACHE_SIZE];				//SocketExtend�������ݻ���
extern volatile u8 SocketExtendReceiveBuf[SOCKET_EXTEND_RECVIVE_SIZE];			//SocketExtend�������ݻ���


u16  SOCKET_Extend_ObtainPacketHead(u8 *addr);								//��ȡSocketExtend��ͷ���ݲ����뻺��
u16  SOCKET_Extend_ObtainPacketData(u8 *addr);								//��ȡSocketExtend�������ݰ������뻺��
void SOCKET_Extend_ObtainPacketCheckCode(u8 *addr, u16 buflength);				//����У���벢����У����


u8   SOCKET_Extend_ConvertCrossID(u8 *addr, u32 crossid);						//��crossidת��Ϊ�ַ���ʽ�����뻺��
u8   SOCKET_Extend_ConvertDirverWayCode(u8 *addr, u32 dirverWayCode);				//��dirverWayCodeת��Ϊ�ַ���ʽ�����뻺��
u8   SOCKET_Extend_Convert2Byte(u8 *addr, u16 data);							//��2Byte����ת�������뻺��
u8   SOCKET_Extend_Convert4Byte(u8 *addr, float data);							//��float����ת�������뻺��
u8   SOCKET_Extend_Convert4ByteInt(u8 *addr, u32 data);						//��Int����ת�������뻺��

void SOCKET_Extend_GetOutputID(u16 *outputid);								//��ȡoutput_ID����˿ڵĲ�����SocketExtend�������ݰ�
u16  SOCKET_Extend_PackagedStorage(void);									//SocketExtend�����ݴ�����뻺����
u16  SOCKET_Extend_RtcCheckPackagedStorage(void);								//SocketExtend����ʱ���ݴ�����뻺����

#endif
