/**
  *********************************************************************************************************
  * @file    socketextendconfig.c
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

#include "socketextendconfig.h"
#include "socketextendfunc.h"
#include "socketextendinstantia.h"
#include "socketextendinitialization.h"

/* SocketExtend�ⲿ�ӿ����ݶ�ȡ���������� */
struct _m_socket_extend_dev socket_extend_dev = 
{
	SOCKET_Extend_GetStartX,												//��ȡStartX��ʼ��
	SOCKET_Extend_GetExitX,												//��ȡExitX������
	SOCKET_Extend_GetServerType,											//��ȡServerTypeҵ������
	SOCKET_Extend_GetCrossID,											//��ȡCrossID·�ڴ���
	SOCKET_Extend_GetDataLength,											//��ȡDataLength���ݳ���
	SOCKET_Extend_GetChannel,											//��ȡChannel�������
	SOCKET_Extend_GetCheckCode,											//��ȡCheckCodeУ����
	SOCKET_Extend_GetDataCount,											//��ȡDataCount��������
	
	SOCKET_Extend_GetRecordNo,											//��ȡRecordNo��ˮ��
	SOCKET_Extend_GetSendTime,											//��ȡSendTime�ϱ�ʱ��
	SOCKET_Extend_GetVehicleCount,										//������ȡVehicleCount������
	SOCKET_Extend_GetLargerVehicleCount,									//������ȡLargerVehicleCount������
	SOCKET_Extend_GetMidVehicleCount,										//������ȡMidVehicleCount�г�����
	SOCKET_Extend_GetSmallVehilceCount,									//������ȡSmallVehilceCountС������
	SOCKET_Extend_GetMiniVehicleCount,										//������ȡMiniVehicleCount΢�ͳ�����
	SOCKET_Extend_GetMotoVehilceCount,										//������ȡMotoVehilceCountĦ�г�����
	SOCKET_Extend_GetReserverType1,										//������ȡReserverType1Ԥ������1
	SOCKET_Extend_GetReserverType2,										//������ȡReserverType2Ԥ������2
	SOCKET_Extend_GetReserverType3,										//������ȡReserverType3Ԥ������3
	SOCKET_Extend_GetReserverType4,										//������ȡReserverType4Ԥ������4
	SOCKET_Extend_GetReserverType5,										//������ȡReserverType5Ԥ������5
	SOCKET_Extend_GetReserverType6,										//������ȡReserverType6Ԥ������6
	SOCKET_Extend_GetMaxHeadInterval,										//������ȡMaxHeadInterval���ͷʱ��(MS)
	SOCKET_Extend_GetMinHeadInterval,										//������ȡMinHeadInterval��С��ͷʱ��(MS)
	SOCKET_Extend_GetAddUpHeadInterval,									//������ȡAddUpHeadInterval�ۼƳ�ͷʱ��(MS)
	SOCKET_Extend_GetMaxSpeed,											//������ȡMaxSpeed�����(Km/h)
	SOCKET_Extend_GetMinSpeed,											//������ȡMinSpeed��С����(Km/h)
	SOCKET_Extend_GetAddUpSpeed,											//������ȡAddUpSpeed�ۼƳ���(Km/h)
	SOCKET_Extend_GetMaxOccupancy,										//������ȡMaxOccupancy���ռ��ʱ��(MS)
	SOCKET_Extend_GetMinOccupancy,										//������ȡMinOccupancy��Сռ��ʱ��(MS)
	SOCKET_Extend_GetAddUpOccupancy,										//������ȡAddUpOccupancy�ۼ�ռ��ʱ��(MS)
	SOCKET_Extend_GetMaxInterval,											//������ȡMaxInterval�����ʱ��(MS)
	SOCKET_Extend_GetMinInterval,											//������ȡMinInterval��С���ʱ��(MS)
	SOCKET_Extend_GetAddUpInterval,										//������ȡAddUpInterval�ۼƼ��ʱ��(MS)
	SOCKET_Extend_GetMaxVehicleLength,										//������ȡMaxVehicleLength��󳵳�(��)
	SOCKET_Extend_GetMinVehicleLength,										//������ȡMinVehicleLength��С����(��)
	SOCKET_Extend_GetAddUpVehilcleLength,									//������ȡAddUpVehilcleLength�ۼƳ���(��)
	SOCKET_Extend_GetMaxQueueLength,										//������ȡMaxQueueLength����Ŷӳ���(��)
	SOCKET_Extend_GetMinQueueLength,										//������ȡMinQueueLength��С�Ŷӳ���(��)
	SOCKET_Extend_GetAddUpQueueLength,										//������ȡAddUpQueueLength�ۼ��Ŷӳ���(��)
	SOCKET_Extend_GetAddUpQueueTime,										//������ȡAddUpQueueTime�ۼ��Ŷ�ʱ��(��)
	SOCKET_Extend_GetRedLightCount,										//������ȡRedLightCount����ƴ�������
	SOCKET_Extend_GetPrecedingWayFullTime,									//������ȡPrecedingWayFullTimeǰ���������ۼ�ʱ��
	SOCKET_Extend_GetDirverWayCode,										//������ȡDirverWayCode��������
	SOCKET_Extend_GetStatus,												//������ȡStatus״̬(0δ����1����)
	SOCKET_Extend_GetReserver1,											//������ȡReserver1Ԥ��
	SOCKET_Extend_GetReserver2,											//������ȡReserver2Ԥ��
	
	SOCKET_Extend_FillData,												//����������������ݰ�
	SOCKET_Extend_RtcCheckFillData,										//����ʱ�����������ݰ�
	SOCKET_Extend_ObtainPacketHead,										//��ȡSocketExtend��ͷ���ݲ����뻺��
	SOCKET_Extend_ObtainPacketData,										//��ȡSocketExtend�������ݰ������뻺��
	SOCKET_Extend_ObtainPacketCheckCode,									//����У���벢����У����
	
	SOCKET_Extend_GetOutputID,											//��ȡoutput_ID����˿ڵĲ�����SocketExtend�������ݰ�
	SOCKET_Extend_PackagedStorage,										//SocketExtend�����ݴ�����뻺����
	SOCKET_Extend_RtcCheckPackagedStorage,									//SocketExtend����ʱ���ݴ�����뻺����
	
	SOCKET_Extend_PortSerialInit,											//��ʼ��SocketExtend����
	SOCKET_Extend_UARTx_IRQ,												//SocketExtendЭ�鴮���жϴ�����
	SOCKET_Extend_Init,													//��ʼ��SocketExtendЭ��
	SOCKET_Extend_Implement,												//SocketExtendЭ�鴦��
};

/********************************************** END OF FLEE **********************************************/
