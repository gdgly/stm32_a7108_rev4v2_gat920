#ifndef __SOCKET_EXTEND_TINSTANTIA_H
#define   __SOCKET_EXTEND_TINSTANTIA_H

#include "stm32f10x_lib.h"
#include "socketextendconfig.h"

#define	SOCKET_EXTEND_AVGLENGTHMIN		3								//ƽ��������Сֵ
#define	SOCKET_EXTEND_AVGLENGTHSINGLE		5								//���شŲ���Ĭ�ϳ���

u8    SOCKET_Extend_GetStartX(void);										//������ȡStartX��ʼ��ֵ,ֱ����ֵ(0x88)
u8    SOCKET_Extend_GetExitX(void);										//������ȡExitX������ֵ,ֱ����ֵ(0x99)
u8    SOCKET_Extend_GetServerType(void);									//������ȡServerTypeҵ������ֵ
u32   SOCKET_Extend_GetCrossID(void);										//������ȡCrossID·�ڴ���
u16   SOCKET_Extend_GetDataLength(Socket_Extend_Packet_Data *addr);				//������ȡDataLength���ݳ���
u16   SOCKET_Extend_GetChannel(void);										//������ȡChannel�������ֵ
u8    SOCKET_Extend_GetCheckCode(void);										//������ȡCheckCodeУ����ֵ
u8    SOCKET_Extend_GetDataCount(Socket_Extend_Packet_Data *addr);				//������ȡDataCount��������

u32   SOCKET_Extend_GetRecordNo(u16 outputid);								//������ȡRecordNo��ˮ��
u32   SOCKET_Extend_GetSendTime(u16 outputid);								//������ȡSendTime�ϱ�ʱ��
u16   SOCKET_Extend_GetVehicleCount(u16 outputid);							//������ȡVehicleCount������
u16   SOCKET_Extend_GetLargerVehicleCount(u16 outputid);						//������ȡLargerVehicleCount������
u16   SOCKET_Extend_GetMidVehicleCount(u16 outputid);							//������ȡMidVehicleCount�г�����
u16   SOCKET_Extend_GetSmallVehilceCount(u16 outputid);						//������ȡSmallVehilceCountС������
u16   SOCKET_Extend_GetMiniVehicleCount(u16 outputid);							//������ȡMiniVehicleCount΢�ͳ�����
u16   SOCKET_Extend_GetMotoVehilceCount(u16 outputid);							//������ȡMotoVehilceCountĦ�г�����
u16   SOCKET_Extend_GetReserverType1(u16 outputid);							//������ȡReserverType1Ԥ������1
u16   SOCKET_Extend_GetReserverType2(u16 outputid);							//������ȡReserverType2Ԥ������2
u16   SOCKET_Extend_GetReserverType3(u16 outputid);							//������ȡReserverType3Ԥ������3
u16   SOCKET_Extend_GetReserverType4(u16 outputid);							//������ȡReserverType4Ԥ������4
u16   SOCKET_Extend_GetReserverType5(u16 outputid);							//������ȡReserverType5Ԥ������5
u16   SOCKET_Extend_GetReserverType6(u16 outputid);							//������ȡReserverType6Ԥ������6
u32   SOCKET_Extend_GetMaxHeadInterval(u16 outputid);							//������ȡMaxHeadInterval���ͷʱ��(MS)
u32   SOCKET_Extend_GetMinHeadInterval(u16 outputid);							//������ȡMinHeadInterval��С��ͷʱ��(MS)
u32   SOCKET_Extend_GetAddUpHeadInterval(u16 outputid);						//������ȡAddUpHeadInterval�ۼƳ�ͷʱ��(MS)
float SOCKET_Extend_GetMaxSpeed(u16 outputid);								//������ȡMaxSpeed�����(Km/h)
float SOCKET_Extend_GetMinSpeed(u16 outputid);								//������ȡMinSpeed��С����(Km/h)
float SOCKET_Extend_GetAddUpSpeed(u16 outputid);								//������ȡAddUpSpeed�ۼƳ���(Km/h)
u32   SOCKET_Extend_GetMaxOccupancy(u16 outputid);							//������ȡMaxOccupancy���ռ��ʱ��(MS)
u32   SOCKET_Extend_GetMinOccupancy(u16 outputid);							//������ȡMinOccupancy��Сռ��ʱ��(MS)
u32   SOCKET_Extend_GetAddUpOccupancy(u16 outputid);							//������ȡAddUpOccupancy�ۼ�ռ��ʱ��(MS)
u32   SOCKET_Extend_GetMaxInterval(u16 outputid);								//������ȡMaxInterval�����ʱ��(MS)
u32   SOCKET_Extend_GetMinInterval(u16 outputid);								//������ȡMinInterval��С���ʱ��(MS)
u32   SOCKET_Extend_GetAddUpInterval(u16 outputid);							//������ȡAddUpInterval�ۼƼ��ʱ��(MS)
float SOCKET_Extend_GetMaxVehicleLength(u16 outputid);							//������ȡMaxVehicleLength��󳵳�(��)
float SOCKET_Extend_GetMinVehicleLength(u16 outputid);							//������ȡMinVehicleLength��С����(��)
float SOCKET_Extend_GetAddUpVehilcleLength(u16 outputid);						//������ȡAddUpVehilcleLength�ۼƳ���(��)
u16   SOCKET_Extend_GetMaxQueueLength(u16 outputid);							//������ȡMaxQueueLength����Ŷӳ���(��)
u16   SOCKET_Extend_GetMinQueueLength(u16 outputid);							//������ȡMinQueueLength��С�Ŷӳ���(��)
u32   SOCKET_Extend_GetAddUpQueueLength(u16 outputid);							//������ȡAddUpQueueLength�ۼ��Ŷӳ���(��)
u16   SOCKET_Extend_GetAddUpQueueTime(u16 outputid);							//������ȡAddUpQueueTime�ۼ��Ŷ�ʱ��(��)
u16   SOCKET_Extend_GetRedLightCount(u16 outputid);							//������ȡRedLightCount����ƴ�������
u16   SOCKET_Extend_GetPrecedingWayFullTime(u16 outputid);						//������ȡPrecedingWayFullTimeǰ���������ۼ�ʱ��
u32   SOCKET_Extend_GetDirverWayCode(u16 outputid);							//������ȡDirverWayCode��������
u8    SOCKET_Extend_GetStatus(u16 outputid);									//������ȡStatus״̬(0δ����1����)
u8    SOCKET_Extend_GetReserver1(u16 outputid);								//������ȡReserver1Ԥ��
u8    SOCKET_Extend_GetReserver2(u16 outputid);								//������ȡReserver2Ԥ��

void  SOCKET_Extend_FillData(void);										//����������������ݰ�
void  SOCKET_Extend_RtcCheckFillData(void);									//����ʱ�����������ݰ�

#endif
