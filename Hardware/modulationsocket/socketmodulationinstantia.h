#ifndef __SOCKET_MODULATION_TINSTANTIA_H
#define   __SOCKET_MODULATION_TINSTANTIA_H

#include "stm32f10x_lib.h"
#include "socketmodulationconfig.h"

#define	SOCKET_MODULATION_AVGLENGTHSINGLE		5											//���شŲ���Ĭ�ϳ���

u16   SOCKET_Modulation_GetHeadFrame(void);													//������ȡHeadFrame֡ͷ,ֱ����ֵ(0xAAAA)
u16   SOCKET_Modulation_GetTailFrame(void);													//������ȡTailFrame֡β,ֱ����ֵ(0xEEEE)
u16   SOCKET_Modulation_GetCRCCheckCode(void);												//������ȡCRCCheckCodeУ����
u16   SOCKET_Modulation_GetPacketLength(Socket_Modulation_Data_Frame *addr);						//������ȡPacketLength����

u8    SOCKET_Modulation_GetPacketType(void);													//������ȡPacketType���ݰ�����
void  SOCKET_Modulation_GetDeviceID(u8 *deviceID);											//������ȡDeviceID�豸���ʶ����
void  SOCKET_Modulation_GetStationID(u8 *stationID);											//������ȡStationIDվ�����
u8    SOCKET_Modulation_GetDeviceErrCode(void);												//������ȡDeviceErrCode�豸Ӳ��������
u8    SOCKET_Modulation_GetSurveyContent(void);												//������ȡSurveyContent��������
u16   SOCKET_Modulation_GetDateYear(void);													//������ȡDateYear��
u8    SOCKET_Modulation_GetDateMon(void);													//������ȡDateMon��
u8    SOCKET_Modulation_GetDateDay(void);													//������ȡDateDay��
u8    SOCKET_Modulation_GetProcessCycle(void);												//������ȡProcessCycle��ͨ���ݴ�������
u16   SOCKET_Modulation_GetTimeSerial(void);													//������ȡTimeSerialʱ�����
u8    SOCKET_Modulation_GetLaneNum(Socket_Modulation_Data_Frame *addr);							//������ȡLaneNum������

u8    SOCKET_Modulation_GetLaneNo(Socket_Modulation_Data_Frame *addr, u16 outputid);					//������ȡLaneNo������
u8    SOCKET_Modulation_GetFollowCar(Socket_Modulation_Data_Frame *addr, u16 outputid);				//������ȡFollowCar�����ٷֱ�
u16   SOCKET_Modulation_GetAvgHeadDistance(Socket_Modulation_Data_Frame *addr, u16 outputid);			//������ȡAvgHeadDistanceƽ����ͷ���
u8    SOCKET_Modulation_GetTimeOccupancy(Socket_Modulation_Data_Frame *addr, u16 outputid);			//������ȡTimeOccupancyʱ��ռ����

u16   SOCKET_Modulation_GetSmallVehilceCount(Socket_Modulation_Data_Frame *addr, u16 outputid);		//������ȡSmallVehilceCountС�ͳ���ͨ����
u8    SOCKET_Modulation_GetSmallVehilceAvgSpeed(Socket_Modulation_Data_Frame *addr, u16 outputid);		//������ȡSmallVehilceAvgSpeedС�ͳ�ƽ���ص㳵��

void  SOCKET_Modulation_FillData(void);														//����������������ݰ�

#endif
