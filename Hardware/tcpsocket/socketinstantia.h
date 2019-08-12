#ifndef __SOCKET_TINSTANTIA_H
#define   __SOCKET_TINSTANTIA_H

#include "stm32f10x_lib.h"
#include "socketconfig.h"

#define	AVGLENGTHMIN		3											//ƽ��������Сֵ
#define	AVGLENGTHSINGLE	5											//���شŲ���Ĭ�ϳ���


u32   SOCKET_GetCrossID(void);											//������ȡCrossID·�ڴ���
u8    SOCKET_GetPacketType(void);											//������ȡPacketType��ˮ������
u32   SOCKET_GetPacketInfo(Socket_Packet_Data *addr);							//������ȡPacketInfo����Ϣ (����ʱ:���ݰ��ڵļ�¼��)
u8    SOCKET_GetDeviceType(u16 outputid);									//������ȡDeviceType�豸����ֵ,�شż����,ֱ����ֵB(0x42)
u16   SOCKET_GetInterval(u16 outputid);										//������ȡIntervalͳ��ʱ��
u16   SOCKET_GetLaneNo(u16 outputid);										//������ȡLaneNo������
u32   SOCKET_GetDateTime(u16 outputid);										//������ȡDateTime��ˮ������ʱ��
u16   SOCKET_GetVolume(u16 outputid);										//������ȡVolumeһ������ͨ���ĳ���
u16   SOCKET_GetVolume1(u16 outputid);										//������ȡVolume1΢С��������
u16   SOCKET_GetVolume2(u16 outputid);										//������ȡVolume2С������
u16   SOCKET_GetVolume3(u16 outputid);										//������ȡVolume3�г�����
u16   SOCKET_GetVolume4(u16 outputid);										//������ȡVolume4������
u16   SOCKET_GetVolume5(u16 outputid);										//������ȡVolume5��������
u16 	 SOCKET_GetAvgOccupancy(u16 outputid);									//������ȡAvgOccupancyƽ��ռ��ʱ��
u16   SOCKET_GetAvgHeadTime(u16 outputid);									//������ȡAvgHeadTimeƽ����ͷʱ��
float SOCKET_GetAvgLength(u16 outputid);									//������ȡAvgLengthƽ������
float SOCKET_GetAvgSpeed(u16 outputid);										//������ȡAvgSpeedƽ���ٶ�
u8    SOCKET_GetSaturation(u16 outputid);									//������ȡSaturation���Ͷ�
u16   SOCKET_GetDensity(u16 outputid);										//������ȡDensity�ܶ�
u16   SOCKET_GetVoltage(u16 outputid);										//������ȡVoltage
u16   SOCKET_GetPcu(u16 outputid);											//������ȡPcu����С����
float SOCKET_GetAvgQueueLength(u16 outputid);								//������ȡAvgQueueLength�Ŷӳ���

void  SOCKET_FillData(void);												//����������������ݰ�


#endif
