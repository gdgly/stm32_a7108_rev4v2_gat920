/**
  *********************************************************************************************************
  * @file    socketconfig.c
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

#include "socketconfig.h"
#include "socketfunc.h"
#include "socketinstantia.h"
#include "socketinitialization.h"

/* socket�ⲿ�ӿ����ݶ�ȡ���������� */
struct _m_socket_dev socket_dev = 
{
	SOCKET_GetCrossID,													//��ȡCrossID·�ڴ���
	SOCKET_GetPacketType,												//��ȡPacketType��ˮ������
	SOCKET_GetPacketInfo,												//��ȡPacketInfo����Ϣ (����ʱ:���ݰ��ڵļ�¼��)
	
	SOCKET_GetDeviceType,												//��ȡDeviceType�豸����ֵ,�شż����,ֱ����ֵB(0x42)
	SOCKET_GetInterval,													//��ȡIntervalͳ��ʱ��
	SOCKET_GetLaneNo,													//��ȡLaneNo������
	SOCKET_GetDateTime,													//��ȡDateTime��ˮ������ʱ��
	SOCKET_GetVolume,													//��ȡVolumeһ������ͨ���ĳ���
	SOCKET_GetVolume1,													//��ȡVolume1΢С��������
	SOCKET_GetVolume2,													//��ȡVolume2С������
	SOCKET_GetVolume3,													//��ȡVolume3�г�����
	SOCKET_GetVolume4,													//��ȡVolume4������
	SOCKET_GetVolume5,													//��ȡVolume5��������
	SOCKET_GetAvgOccupancy,												//��ȡAvgOccupancyƽ��ռ��ʱ��
	SOCKET_GetAvgHeadTime,												//��ȡAvgHeadTimeƽ����ͷʱ��
	SOCKET_GetAvgLength,												//��ȡAvgLengthƽ������
	SOCKET_GetAvgSpeed,													//��ȡAvgSpeedƽ���ٶ�
	SOCKET_GetSaturation,												//��ȡSaturation���Ͷ�
	SOCKET_GetDensity,													//��ȡDensity�ܶ�
	SOCKET_GetVoltage,													//��ȡVoltage
	SOCKET_GetAvgQueueLength,											//��ȡAvgQueueLength�Ŷӳ���
	
	SOCKET_FillData,													//����������������ݰ�
	SOCKET_ObtainPacketHead,												//��ȡSocket��ͷ���ݲ����뻺��
	SOCKET_ObtainPacketData,												//��ȡSocket�������ݰ������뻺��
	SOCKET_ObtainPacketManuCheck,											//����У���벢���������̱����У����
	
	SOCKET_GetOutputID,													//��ȡoutput_ID����˿ڵĲ�����Socket�������ݰ�
	SOCKET_PackagedStorage,												//Socket�����ݴ�����뻺����
	
	SOCKET_PortSerialInit,												//��ʼ��Socket����
	SOCKET_UARTx_IRQ,													//SocketЭ�鴮���жϴ�����
	SOCKET_Init,														//��ʼ��SocketЭ��
	SOCKET_Implement,													//SocketЭ�鴦��
};

/********************************************** END OF FLEE **********************************************/
