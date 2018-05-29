/**
  *********************************************************************************************************
  * @file    socketmodulationconfig.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   ���������ⲿ�����ӿ�
  *********************************************************************************************************
  * @attention
  *		
  *			
  *********************************************************************************************************
  */

#include "socketmodulationconfig.h"
#include "socketmodulationfunc.h"
#include "socketmodulationinstantia.h"
#include "socketmodulationinitialization.h"

/* SocketModulation�ⲿ�ӿ����ݶ�ȡ���������� */
struct _m_socket_modulation_dev socket_modulation_dev = 
{
	SOCKET_Modulation_GetHeadFrame,										//������ȡHeadFrame֡ͷ,ֱ����ֵ(0xAAAA)
	SOCKET_Modulation_GetTailFrame,										//������ȡTailFrame֡β,ֱ����ֵ(0xEEEE)
	SOCKET_Modulation_GetCRCCheckCode,										//������ȡCRCCheckCodeУ����
	SOCKET_Modulation_GetPacketLength,										//������ȡPacketLength����
	
	SOCKET_Modulation_GetPacketType,										//������ȡPacketType���ݰ�����
	SOCKET_Modulation_GetDeviceID,										//������ȡDeviceID�豸���ʶ����
	SOCKET_Modulation_GetStationID,										//������ȡStationIDվ�����
	SOCKET_Modulation_GetDeviceErrCode,									//������ȡDeviceErrCode�豸Ӳ��������
	SOCKET_Modulation_GetSurveyContent,									//������ȡSurveyContent��������
	SOCKET_Modulation_GetDateYear,										//������ȡDateYear��
	SOCKET_Modulation_GetDateMon,											//������ȡDateMon��
	SOCKET_Modulation_GetDateDay,											//������ȡDateDay��
	SOCKET_Modulation_GetProcessCycle,										//������ȡProcessCycle��ͨ���ݴ�������
	SOCKET_Modulation_GetTimeSerial,										//������ȡTimeSerialʱ�����
	SOCKET_Modulation_GetLaneNum,											//������ȡLaneNum������
	
	SOCKET_Modulation_GetLaneNo,											//������ȡLaneNo������
	SOCKET_Modulation_GetFollowCar,										//������ȡFollowCar�����ٷֱ�
	SOCKET_Modulation_GetAvgHeadDistance,									//������ȡAvgHeadDistanceƽ����ͷ���
	SOCKET_Modulation_GetTimeOccupancy,									//������ȡTimeOccupancyʱ��ռ����
	
	SOCKET_Modulation_GetSmallVehilceCount,									//������ȡSmallVehilceCountС�ͳ���ͨ����
	SOCKET_Modulation_GetSmallVehilceAvgSpeed,								//������ȡSmallVehilceAvgSpeedС�ͳ�ƽ���ص㳵��
	
	SOCKET_Modulation_FillData,											//����������������ݰ�
	SOCKET_Modulation_ObtainDataFrame,										//��ȡSocketModulation����֡�����뻺��
	SOCKET_Modulation_ObtainDataFrameCheckCode,								//����У���벢����У����
	
	SOCKET_Modulation_GetOutputID,										//��ȡoutput_ID����˿ڵĲ�����SocketModulation�������ݰ�
	SOCKET_Modulation_PackagedStorage,										//SocketModulation�����ݴ�����뻺����
	
	SOCKET_Modulation_PortSerialInit,										//��ʼ��SocketModulation����
	SOCKET_Modulation_UARTx_IRQ,											//SocketModulationЭ�鴮���жϴ�����
	SOCKET_Modulation_Init,												//��ʼ��SocketModulationЭ��
	SOCKET_Modulation_Implement,											//SocketModulationЭ�鴦��
};

/********************************************** END OF FLEE **********************************************/
