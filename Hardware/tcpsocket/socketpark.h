#ifndef __SOCKET_PARK_H
#define   __SOCKET_PARK_H

#include "stm32f10x_lib.h"
#include "platform_config.h"


u16 SOCKET_ParkObtainPacketHead(u8 *addr);									//��ȡSocketPark��ͷ���ݲ����뻺��
u16 SOCKET_ParkObtainPacketData(u8 *addr);									//��ȡSocketParkͣ�������ݰ������뻺��

void SOCKET_ParkImplement(u8 laneNo, u16 CarNum, u8 CarInorOut);					//Socket����ʵʱ�ϴ�
void SOCKET_ParkImplementHeartbeatSend(u8 laneNo, u16 CarNum);					//Socket���������ϴ�
void SOCKET_ParkImplementHeartbeat(u8 *buf);									//Socket����ʵʱ�ϴ�(�������ϴ�����)

#endif
