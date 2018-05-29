/**
  *********************************************************************************************************
  * @file    socketextendfunc.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   socket Extend �ڲ�Э��
  *********************************************************************************************************
  * @attention
  *			���� : 
  *			1.  ��ȡSocketExtend��ͷ���ݲ����뻺��							(�ⲿ����)
  *			2.  ��ȡSocketExtend�������ݰ������뻺��						(�ⲿ����)
  *			3.  ����У���벢����У����									(�ⲿ����)
  *
  *			4.  ��crossidת��Ϊ�ַ���ʽ�����뻺��							(�ⲿ����)
  *			5.  ��2Byte����ת�������뻺��									(�ⲿ����)
  *			6.  ��float����ת�������뻺��									(�ⲿ����)
  *			7.  ��Int����ת�������뻺��									(�ⲿ����)
  *
  *			8.  ��ȡoutput_ID����˿ڵĲ�����SocketExtend�������ݰ�			(�ⲿ����)
  *			9.  SocketExtend�����ݴ�����뻺����							(�ⲿ����)
  *			10. SocketExtend����ʱ���ݴ�����뻺����						(�ⲿ����)
  *
  *********************************************************************************************************
  */

#include "socketextendfunc.h"

Socket_Extend_Packet_Head			SocketExtendHeadPacket;					//SocketExtend���ݰ�ͷ
Socket_Extend_Packet_Data			SocketExtendDataPacket[OUTPUT_MAX];		//SocketExtend�������ݰ���
volatile u8 SocketExtendSendBuf[SOCKET_EXTEND_CACHE_SIZE];						//SocketExtend�������ݻ���
volatile u8 SocketExtendReceiveBuf[SOCKET_EXTEND_RECVIVE_SIZE];					//SocketExtend�������ݻ���

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_ObtainPacketHead(u8 *addr)
 @Description			SOCKET_Extend_ObtainPacketHead	: ��ȡSocketExtend��ͷ���ݲ����뻺��
 @Input				*addr						: Socket�����ַ
 @Return				DataLength					: ���뻺�����ݳ���
					SOCKET_EXTEND_FALSE   			: ����
**********************************************************************************************************/
u16 SOCKET_Extend_ObtainPacketHead(u8 *addr)
{
	u16 headlength = 0;
	
	addr[headlength] = SocketExtendHeadPacket.StartX;															//������ʼ��
	headlength += 1;
	
	addr[headlength] = SocketExtendHeadPacket.ServerType;														//����ҵ������
	headlength += 1;
	
	if ((SOCKET_Extend_ConvertCrossID((u8 *)&addr[headlength], SocketExtendHeadPacket.CrossID)) == SOCKET_EXTEND_FALSE) {	//����CrossID
		return SOCKET_EXTEND_FALSE;
	}
	else {
		headlength += 8;
	}
	
	if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[headlength], SocketExtendHeadPacket.DataLength)) == SOCKET_EXTEND_FALSE) {	//�������ݳ���
		return SOCKET_EXTEND_FALSE;
	}
	else {
		headlength += 2;
	}
	
	if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[headlength], SocketExtendHeadPacket.Channel)) == SOCKET_EXTEND_FALSE) {	//���봫�����
		return SOCKET_EXTEND_FALSE;
	}
	else {
		headlength += 2;
	}
	
	addr[headlength] = SocketExtendHeadPacket.ExitX;															//���������
	headlength += 1;
	
	addr[headlength] = SocketExtendHeadPacket.CheckCode;														//����У����
	headlength += 1;
	
	addr[headlength] = SocketExtendHeadPacket.DataCount;														//������������
	headlength += 1;
	
	return headlength;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_ObtainPacketData(u8 *addr)
 @Description			SOCKET_Extend_ObtainPacketData	: ��ȡSocketExtend�������ݰ������뻺��
 @Input				*addr						: Socket�����ַ
 @Return				DataLength					: ���뻺�����ݳ���
					SOCKET_FALSE   				: ����
**********************************************************************************************************/
u16 SOCKET_Extend_ObtainPacketData(u8 *addr)
{
	u8 i = 0;
	u16 datalength = 0;
	
	datalength += 17;																				//��ַƫ�Ƴ����ݰ�ͷ
	
	for (i = 0; i < OUTPUT_MAX; i++) {																	//������Ѱ���ù��ĳ�����
		if (SocketExtendDataPacket[i].OutputID != 0) {
			
			/* ������ˮ�� */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].RecordNo)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* �����ϱ�ʱ�� */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].SendTime)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* ���복���� */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].VehicleCount)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* ��������� */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].LargerVehicleCount)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* �����г����� */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].MidVehicleCount)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* ����С������ */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].SmallVehilceCount)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* ����΢�ͳ����� */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].MiniVehicleCount)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* ����Ħ�г����� */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].Moto)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* ����Ԥ������1 */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].ReserverType1)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* ����Ԥ������2 */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].ReserverType2)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* ����Ԥ������3 */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].ReserverType3)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* ����Ԥ������4 */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].ReserverType4)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* ����Ԥ������5 */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].ReserverType5)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* ����Ԥ������6 */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].ReserverType6)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* �������ͷʱ�� */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].MaxHeadInterval)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* ������С��ͷʱ�� */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].MinHeadInterval)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* �����ۼƳ�ͷʱ�� */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].AddUpHeadInterval)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* ��������� */
			if ((SOCKET_Extend_Convert4Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].MaxSpeed)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* ������С���� */
			if ((SOCKET_Extend_Convert4Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].MinSpeed)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* �����ۼƳ��� */
			if ((SOCKET_Extend_Convert4Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].AddUpSpeed)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* �������ռ��ʱ�� */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].MaxOccupancy)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* ������Сռ��ʱ�� */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].MinOccupancy)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* �����ۼ�ռ��ʱ�� */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].AddUpOccupancy)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* ���������ʱ�� */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].MaxInterval)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* ������С���ʱ�� */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].MinInterval)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* �����ۼƼ��ʱ�� */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].AddUpInterval)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* ������󳵳� */
			if ((SOCKET_Extend_Convert4Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].MaxVehicleLength)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* ������С���� */
			if ((SOCKET_Extend_Convert4Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].MinVehicleLength)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* �����ۼƳ��� */
			if ((SOCKET_Extend_Convert4Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].AddUpVehilcleLength)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* ��������Ŷӳ��� */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].MaxQueueLength)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* ������С�Ŷӳ��� */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].MinQueueLength)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* �����ۼ��Ŷӳ��� */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].AddUpQueueLength)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* �����ۼ��Ŷ�ʱ�� */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].AddUpQueueTime)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* ���봳��ƴ������� */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].RedLightCount)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* ����ǰ���������ۼ�ʱ�� */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].PrecedingWayFullTime)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* ���복������ */
			if ((SOCKET_Extend_ConvertDirverWayCode((u8 *)&addr[datalength], SocketExtendDataPacket[i].DirverWayCode)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 3;
			}
			
			/* ����״̬ */
			addr[datalength] = SocketExtendDataPacket[i].Status;
			datalength += 1;
			
			/* ����Ԥ�� */
			addr[datalength] = SocketExtendDataPacket[i].Reserver1;
			datalength += 1;
			
			/* ����Ԥ�� */
			addr[datalength] = SocketExtendDataPacket[i].Reserver2;
			datalength += 1;
		}
	}
	
	datalength -= 17;																				//��ȥ���ݰ�ͷ16byte
	
	return datalength;
}

/**********************************************************************************************************
 @Function			void SOCKET_Extend_ObtainPacketCheckCode(u8 *addr, u16 buflength)
 @Description			����У���벢����У����
 @Input				*addr		: Socket�����ַ
					buflength		: ���ݳ���
 @Return				void
**********************************************************************************************************/
void SOCKET_Extend_ObtainPacketCheckCode(u8 *addr, u16 buflength)
{
	u8 checkcodehead = 0;
	u8 checkcodedata = 0;
	u16 i = 0;
	u16 dateindex = 0;
	
	dateindex += 1;
	checkcodehead = addr[dateindex];
	dateindex += 1;
	for (i = dateindex; i < 14; i++) {
		checkcodehead = checkcodehead ^ addr[i];
	}
	
	if (buflength > 17) {
		dateindex = 0;
		dateindex += 17;
		checkcodedata = addr[dateindex];
		dateindex += 1;
		for (i = dateindex; i < buflength; i++) {
			checkcodedata = checkcodedata ^ addr[i];
		}
		
		addr[15] = checkcodehead ^ checkcodedata;
	}
	else {
		addr[15] = checkcodehead;
	}
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Extend_ConvertCrossID(u8 *addr, u32 crossid)
 @Description			SOCKET_Extend_ConvertCrossID		: ��crossidת��Ϊ�ַ���ʽ�����뻺��
 @Input				*addr						: ���ת�������ݵĵ�ַ
					crossid						: CrossIdֵ
 @Return				1							: ����
					0							: ����
**********************************************************************************************************/
u8 SOCKET_Extend_ConvertCrossID(u8 *addr, u32 crossid)
{
	u8 i = 0;
	u8 tempbuf[8];
	
	if (crossid > 99999999) {											//���CrossIdֵ�Ƿ����
		return SOCKET_EXTEND_FALSE;
	}
	
	tempbuf[0] = (crossid / 10000000) + '0';
	tempbuf[1] = ((crossid % 10000000) / 1000000) + '0';
	tempbuf[2] = ((crossid % 1000000) / 100000) + '0';
	tempbuf[3] = ((crossid % 100000) / 10000) + '0';
	tempbuf[4] = ((crossid % 10000) / 1000) + '0';
	tempbuf[5] = ((crossid % 1000) / 100) + '0';
	tempbuf[6] = ((crossid % 100) / 10) + '0';
	tempbuf[7] = (crossid % 10) + '0';
	
	for (i = 0; i < 8; i++) {
		addr[i] = tempbuf[i];
	}
	
	return SOCKET_EXTEND_TRUE;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Extend_ConvertDirverWayCode(u8 *addr, u32 dirverWayCode)
 @Description			SOCKET_Extend_ConvertDirverWayCode	: ��dirverWayCodeת��Ϊ�ַ���ʽ�����뻺��
 @Input				*addr						: ���ת�������ݵĵ�ַ
					dirverWayCode					: dirverWayCodeֵ
 @Return				1							: ����
					0							: ����
**********************************************************************************************************/
u8 SOCKET_Extend_ConvertDirverWayCode(u8 *addr, u32 dirverWayCode)
{
	u8 i = 0;
	u8 tempbuf[3];
	
	if (dirverWayCode > 999) {											//���LaneNoֵ�Ƿ����
		return SOCKET_EXTEND_FALSE;
	}
	
	tempbuf[0] = (dirverWayCode / 100) + '0';
	tempbuf[1] = ((dirverWayCode % 100) / 10) + '0';
	tempbuf[2] = (dirverWayCode % 10) + '0';
	
	for (i = 0; i < 3; i++) {
		addr[i] = tempbuf[i];
	}
	
	return SOCKET_EXTEND_TRUE;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Extend_Convert2Byte(u8 *addr, u16 data)
 @Description			SOCKET_Extend_Convert2Byte	: ��2Byte����ת�������뻺��
 @Input				*addr					: ���ת�������ݵĵ�ַ
					data						: ��ֵ
 @Return				1						: ����
					0						: ����
**********************************************************************************************************/
u8 SOCKET_Extend_Convert2Byte(u8 *addr, u16 data)
{
	u8 i = 0;
	u8 tempbuf[2];
	
	tempbuf[0] = data & 0xff;
	tempbuf[1] = (data >> 8) & 0xff;

	for (i = 0; i < 2; i++) {
		addr[i] = tempbuf[i];
	}
	
	return SOCKET_EXTEND_TRUE;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Extend_Convert4Byte(u8 *addr, float data)
 @Description			SOCKET_Extend_Convert4Byte	: ��float����ת�������뻺��
 @Input				*addr					: ���ת�������ݵĵ�ַ
					data						: ��ֵ
 @Return				1						: ����
					0						: ����
**********************************************************************************************************/
u8 SOCKET_Extend_Convert4Byte(u8 *addr, float data)
{
	u8 i = 0;
	u8 tempbuf[4];
	float temp, *ptemp;
	u32 *ptemp1;
	
	temp = data;
	ptemp = &temp;
	ptemp1 = (u32 *)ptemp;
	
	tempbuf[0] = *ptemp1 & 0xff;
	tempbuf[1] = (*ptemp1 >> 8) & 0xff;
	tempbuf[2] = (*ptemp1 >> 16) & 0xff;
	tempbuf[3] = *ptemp1 >> 24;
	
	for (i = 0; i < 4; i++) {
		addr[i] = tempbuf[i];
	}
	
	return SOCKET_EXTEND_TRUE;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Extend_Convert4ByteInt(u8 *addr, u32 data)
 @Description			SOCKET_Extend_Convert4ByteInt	: ��Int����ת�������뻺��
 @Input				*addr					: ���ת�������ݵĵ�ַ
					data						: ��ֵ
 @Return				1						: ����
					0						: ����
**********************************************************************************************************/
u8 SOCKET_Extend_Convert4ByteInt(u8 *addr, u32 data)
{
	u8 i = 0;
	u8 tempbuf[4];
	
	tempbuf[0] = data & 0xff;
	tempbuf[1] = (data >> 8) & 0xff;
	tempbuf[2] = (data >> 16) & 0xff;
	tempbuf[3] = data >> 24;
	
	for (i = 0; i < 4; i++) {
		addr[i] = tempbuf[i];
	}
	
	return SOCKET_EXTEND_TRUE;
}

/**********************************************************************************************************
 @Function			void SOCKET_Extend_GetOutputID(u16 *outputid)
 @Description			SOCKET_Extend_GetOutputID	: ��ȡoutput_ID����˿ڵĲ�����SocketExtend�������ݰ�
 @Input				Output_ID��ŵ�ַ
 @Return				void
**********************************************************************************************************/
void SOCKET_Extend_GetOutputID(u16 *outputid)
{
	u8 i = 0;
	
	for (i = 0; i < OUTPUT_MAX; i++) {
		SocketExtendDataPacket[i].OutputID = outputid[i];
	}
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_PackagedStorage(void)
 @Description			SocketExtend�����ݴ�����뻺����
 @Input				void
 @Return				u16 SendLength		: �������ݳ���
**********************************************************************************************************/
u16 SOCKET_Extend_PackagedStorage(void)
{
	u16 headlength = 0;
	u16 datalength = 0;
	
	socket_extend_dev.FillData();											//����������������ݰ�
	headlength = socket_extend_dev.ObtainPacketHead((u8 *)SocketExtendSendBuf);	//��ȡSocketExtend��ͷ���ݲ����뻺��
	datalength = socket_extend_dev.ObtainPacketData((u8 *)SocketExtendSendBuf);	//��ȡSocketExtend�������ݰ������뻺��
	headlength = headlength + datalength;
	socket_extend_dev.ObtainPacketCheckCode((u8 *)SocketExtendSendBuf, headlength);	//����У���벢����У����
	
	return headlength;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_RtcCheckPackagedStorage(void)
 @Description			SocketExtend����ʱ���ݴ�����뻺����
 @Input				void
 @Return				u16 SendLength		: �������ݳ���
**********************************************************************************************************/
u16 SOCKET_Extend_RtcCheckPackagedStorage(void)
{
	u16 headlength = 0;
	
	socket_extend_dev.RtcCheckFillData();
	headlength = socket_extend_dev.ObtainPacketHead((u8 *)SocketExtendSendBuf);	//��ȡSocketExtend��ͷ���ݲ����뻺��
	socket_extend_dev.ObtainPacketCheckCode((u8 *)SocketExtendSendBuf, headlength);	//����У���벢����У����
	
	return (headlength - 1);
}

/********************************************** END OF FLEE **********************************************/
