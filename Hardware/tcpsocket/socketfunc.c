/**
  *********************************************************************************************************
  * @file    socketfunc.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   socket�ڲ�Э��
  *********************************************************************************************************
  * @attention
  *			���� : 
  *			1.  ��ȡSocket��ͷ���ݲ����뻺��					(�ⲿ����)
  *			2.  ��ȡSocket�������ݰ������뻺��					(�ⲿ����)
  *			3.  ����У���벢���������̱����У����				(�ⲿ����)
  *			4.  ��ȡoutput_ID����˿ڵĲ�����Socket�������ݰ�		(�ⲿ����)
  *			5.  ��crossidת��Ϊ�ַ���ʽ�����뻺��				(�ڲ�����)
  *			6.  ��packetinfoת�������뻺��					(�ڲ�����)
  *			7.  ��intervalת�������뻺��						(�ڲ�����)
  *			8.  ��lannoת��Ϊ�ַ���ʽ�����뻺��				(�ڲ�����)
  *			9.  ��ϵͳʱ��ת��Ϊ�ַ���ʽ�����뻺��				(�ڲ�����)
  *			10. ��2Byte����ת�������뻺��						(�ڲ�����)
  *			11. ��float����ת�������뻺��						(�ڲ�����)
  *
  *********************************************************************************************************
  */

#include "socketfunc.h"
#include "rtc.h"


Socket_Packet_Head			SocketHeadPacket;								//Socket���ݰ�ͷ
Socket_Packet_Data			SocketDataPacket[OUTPUT_MAX];						//Socket�������ݰ���
volatile u8 SocketSendBuf[SOCKET_CACHE_SIZE];								//Socket�������ݻ���
volatile u8 SocketReceiveBuf[SOCKET_RECVIVE_SIZE];							//Socket�������ݻ���


/**********************************************************************************************************
 @Function			u16 SOCKET_ObtainPacketHead(u8 *addr)
 @Description			��ȡSocket��ͷ���ݲ����뻺��
 @Input				*addr		: Socket�����ַ
 @Return				���뻺�����ݳ���
					SOCKET_FALSE   : ����
**********************************************************************************************************/
u16 SOCKET_ObtainPacketHead(u8 *addr)
{
	u16 headlength = 0;																				//���뻺�����ݳ���
	
	if ((SOCKET_ConvertCrossID((u8 *)&addr[headlength], SocketHeadPacket.CrossID)) == SOCKET_FALSE) {				//����CrossID
		return SOCKET_FALSE;
	}
	else {
		headlength += 8;																			//��ַƫ��8byte
	}
	
	addr[headlength] = SocketHeadPacket.PacketType;														//������ˮ������
	headlength += 1;																				//��ַƫ��1byte
	
	if ((SOCKET_ConvertPacketInfo((u8 *)&addr[headlength], SocketHeadPacket.PacketInfo)) == SOCKET_FALSE) {			//��������������ݰ���
		return SOCKET_FALSE;
	}
	else {
		headlength += 4;																			//��ַƫ��4byte
	}
	
	return headlength;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_ObtainPacketData(u8 *addr)
 @Description			��ȡSocket�������ݰ������뻺��
 @Input				*addr		: Socket�����ַ
 @Return				���뻺�����ݳ���
					SOCKET_FALSE   : ����
**********************************************************************************************************/
u16 SOCKET_ObtainPacketData(u8 *addr)
{
	u8 i = 0;
	u16 datalength = 0;																				//���뻺�����ݳ���
	
	datalength += 13;																				//��ַƫ�Ƴ����ݰ�ͷ
	
	for (i = 0; i < OUTPUT_MAX; i++) {																	//������Ѱ���ù��ĳ�����
		if (SocketDataPacket[i].OutputID != 0) {
			
			addr[datalength] = SocketDataPacket[i].DeviceType;													//�����豸���
			datalength += 1;																		//��ַƫ��1byte
			
			if ((SOCKET_ConvertInterval((u8 *)&addr[datalength], SocketDataPacket[i].Interval)) == SOCKET_FALSE) {			//����ͳ��ʱ��
				return SOCKET_FALSE;
			}
			else {
				datalength += 2;																	//��ַƫ��2byte
			}
			
			if ((SOCKET_ConvertLaneNo((u8 *)&addr[datalength], SocketDataPacket[i].LaneNo)) == SOCKET_FALSE) {				//���복����
				return SOCKET_FALSE;
			}
			else {
				datalength += 3;																	//��ַƫ��3byte
			}
			
			if ((SOCKET_ConvertDateTime((u8 *)&addr[datalength], SocketDataPacket[i].DateTime)) == SOCKET_FALSE) {			//������ˮ����ʱ��
				return SOCKET_FALSE;
			}
			else {
				datalength += 20;																	//��ַƫ��20byte
			}
			
			if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketDataPacket[i].Volume)) == SOCKET_FALSE) {				//����һ��ͨ������
				return SOCKET_FALSE;
			}
			else {
				datalength += 2;																	//��ַƫ��2byte
			}
			
			if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketDataPacket[i].Volume1)) == SOCKET_FALSE) {				//����΢С��������
				return SOCKET_FALSE;
			}
			else {
				datalength += 2;																	//��ַƫ��2byte
			}
			
			if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketDataPacket[i].Volume2)) == SOCKET_FALSE) {				//����С������
				return SOCKET_FALSE;
			}
			else {
				datalength += 2;																	//��ַƫ��2byte
			}
			
			if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketDataPacket[i].Volume3)) == SOCKET_FALSE) {				//�����г�����
				return SOCKET_FALSE;
			}
			else {
				datalength += 2;																	//��ַƫ��2byte
			}
			
			if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketDataPacket[i].Volume4)) == SOCKET_FALSE) {				//���������
				return SOCKET_FALSE;
			}
			else {
				datalength += 2;																	//��ַƫ��2byte
			}
			
			if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketDataPacket[i].Volume5)) == SOCKET_FALSE) {				//���볬������
				return SOCKET_FALSE;
			}
			else {
				datalength += 2;																	//��ַƫ��2byte
			}
			
			if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketDataPacket[i].AvgOccupancy)) == SOCKET_FALSE) {			//����ƽ��ռ��ʱ��
				return SOCKET_FALSE;
			}
			else {
				datalength += 2;																	//��ַƫ��2byte
			}
			
			if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketDataPacket[i].AvgHeadTime)) == SOCKET_FALSE) {			//����ƽ����ͷʱ��
				return SOCKET_FALSE;
			}
			else {
				datalength += 2;																	//��ַƫ��2byte
			}
			
			if ((SOCKET_Convert4Byte((u8 *)&addr[datalength], SocketDataPacket[i].AvgLength)) == SOCKET_FALSE) {			//����ƽ������
				return SOCKET_FALSE;
			}
			else {
				datalength += 4;																	//��ַƫ��4byte
			}
			
			if ((SOCKET_Convert4Byte((u8 *)&addr[datalength], SocketDataPacket[i].AvgSpeed)) == SOCKET_FALSE) {				//����ƽ���ٶ�
				return SOCKET_FALSE;
			}
			else {
				datalength += 4;																	//��ַƫ��4byte
			}
			
			addr[datalength] = SocketDataPacket[i].Saturation;													//���뱥�Ͷ�
			datalength += 1;																		//��ַƫ��1byte
			
			if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketDataPacket[i].Density)) == SOCKET_FALSE) {				//�����ܶ�
				return SOCKET_FALSE;
			}
			else {
				datalength += 2;																	//��ַƫ��2byte
			}
			
			if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketDataPacket[i].Voltage)) == SOCKET_FALSE) {				//����Voltage
				return SOCKET_FALSE;
			}
			else {
				datalength += 2;																	//��ַƫ��2byte
			}
			
			if ((SOCKET_Convert4Byte((u8 *)&addr[datalength], SocketDataPacket[i].AvgQueueLength)) == SOCKET_FALSE) {		//�����Ŷӳ���
				return SOCKET_FALSE;
			}
			else {
				datalength += 4;																	//��ַƫ��4byte
			}
		}
	}
	
	datalength -= 13;																				//��ȥ���ݰ�ͷ13byte
	
	return datalength;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_ObtainPacketManuCheck(u8 *addr, u16 buflength)
 @Description			����У���벢���������̱����У����
 @Input				*addr		: Socket�����ַ
					buflength		: ���ݳ���
 @Return				�����̱����У���볤��
**********************************************************************************************************/
u16 SOCKET_ObtainPacketManuCheck(u8 *addr, u16 buflength)
{
	u8 checkcode = 0;
	u16 i = 0;
	u16 dateindex = 0;
	
	dateindex += 13;													//��ַƫ�Ƴ����ݰ�ͷ
	checkcode = addr[dateindex];
	dateindex += 1;
	for (i = dateindex; i < buflength; i++) {								//����У����
		checkcode = checkcode ^ addr[i];
	}
	
	addr[buflength] = MANUFACTURERCODE;									//���������̱���
	addr[buflength + 1] = checkcode;										//����У����
	
	return 2;															//�����̱����У�����ֽ���
}

/**********************************************************************************************************
 @Function			u8 SOCKET_ConvertCrossID(u8 *addr, u32 crossid)
 @Description			��crossidת��Ϊ�ַ���ʽ�����뻺��
 @Input				*addr		: ���ת�������ݵĵ�ַ
					crossid		: CrossIdֵ
 @Return				1			: ����
					0			: ����
**********************************************************************************************************/
u8 SOCKET_ConvertCrossID(u8 *addr, u32 crossid)
{
	u8 i = 0;
	u8 tempbuf[8];
	
	if (crossid > 99999999) {											//���CrossIdֵ�Ƿ����
		return SOCKET_FALSE;
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
	
	return SOCKET_TRUE;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_ConvertPacketInfo(u8 *addr, u32 packetinfo)
 @Description			��packetinfoת�������뻺��
 @Input				*addr		: ���ת�������ݵĵ�ַ
					packetinfo	: PacketInfoֵ
 @Return				1			: ����
					0			: ����
**********************************************************************************************************/
u8 SOCKET_ConvertPacketInfo(u8 *addr, u32 packetinfo)
{
	u8 i = 0;
	u8 tempbuf[4];
	
	tempbuf[0] = packetinfo & 0xff;
	tempbuf[1] = (packetinfo >> 8) & 0xff;
	tempbuf[2] = (packetinfo >> 16) & 0xff;
	tempbuf[3] = packetinfo >> 24;
	
	for (i = 0; i < 4; i++) {
		addr[i] = tempbuf[i];
	}
	
	return SOCKET_TRUE;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_ConvertInterval(u8 *addr, u16 interval)
 @Description			��intervalת�������뻺��
 @Input				*addr		: ���ת�������ݵĵ�ַ
					interval		: Intervalֵ
 @Return				1			: ����
					0			: ����
**********************************************************************************************************/
u8 SOCKET_ConvertInterval(u8 *addr, u16 interval)
{
	u8 i = 0;
	u8 tempbuf[2];
	
	tempbuf[0] = interval & 0xff;
	tempbuf[1] = (interval >> 8) & 0xff;

	for (i = 0; i < 2; i++) {
		addr[i] = tempbuf[i];
	}
	
	return SOCKET_TRUE;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_ConvertLaneNo(u8 *addr, u16 lanno)
 @Description			��lannoת��Ϊ�ַ���ʽ�����뻺��
 @Input				*addr		: ���ת�������ݵĵ�ַ
					lanno		: LaneNoֵ
 @Return				1			: ����
					0			: ����
**********************************************************************************************************/
u8 SOCKET_ConvertLaneNo(u8 *addr, u16 lanno)
{
	u8 i = 0;
	u8 tempbuf[3];
	
	if (lanno > 999) {													//���LaneNoֵ�Ƿ����
		return SOCKET_FALSE;
	}
	
	tempbuf[0] = (lanno / 100) + '0';
	tempbuf[1] = ((lanno % 100) / 10) + '0';
	tempbuf[2] = (lanno % 10) + '0';
	
	for (i = 0; i < 3; i++) {
		addr[i] = tempbuf[i];
	}
	
	return SOCKET_TRUE;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_ConvertDateTime(u8 *addr, u32 rtccounter)
 @Description			��ϵͳʱ��ת��Ϊ�ַ���ʽ�����뻺��
 @Input				*addr		: ���ת�������ݵĵ�ַ
					rtccounter	: RTC������ֵ
 @Return				1			: ����
					0			: ����
**********************************************************************************************************/
u8 SOCKET_ConvertDateTime(u8 *addr, u32 rtccounter)
{
	u8 i = 0;
	u8 tempbuf[20];
	
	to_tm(rtccounter, &systemtime);										//RTC������ֵת��Ϊʱ��ṹ��
	
	tempbuf[0] = (systemtime.tm_year / 1000) + '0';
	tempbuf[1] = ((systemtime.tm_year % 1000) / 100) + '0';
	tempbuf[2] = ((systemtime.tm_year % 100) / 10) + '0';
	tempbuf[3] = (systemtime.tm_year % 10) + '0';
	tempbuf[4] = 0x2d;
	tempbuf[5] = (systemtime.tm_mon / 10) + '0';
	tempbuf[6] = (systemtime.tm_mon % 10) + '0';
	tempbuf[7] = 0x2d;
	tempbuf[8] = (systemtime.tm_mday / 10) + '0';
	tempbuf[9] = (systemtime.tm_mday % 10) + '0';
	tempbuf[10] = 0x20;
	tempbuf[11] = (systemtime.tm_hour / 10) + '0';
	tempbuf[12] = (systemtime.tm_hour % 10) + '0';
	tempbuf[13] = 0x3a;
	tempbuf[14] = (systemtime.tm_min / 10) + '0';
	tempbuf[15] = (systemtime.tm_min % 10) + '0';
	tempbuf[16] = 0x3a;
	tempbuf[17] = (systemtime.tm_sec / 10) + '0';
	tempbuf[18] = (systemtime.tm_sec % 10) + '0';
	tempbuf[19] = 0x00;
	
	for (i = 0; i < 20; i++) {
		addr[i] = tempbuf[i];
	}
	
	return SOCKET_TRUE;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Convert2Byte(u8 *addr, u16 data)
 @Description			��2Byte����ת�������뻺��
 @Input				*addr		: ���ת�������ݵĵ�ַ
					data			: ��ֵ
 @Return				1			: ����
					0			: ����
**********************************************************************************************************/
u8 SOCKET_Convert2Byte(u8 *addr, u16 data)
{
	u8 i = 0;
	u8 tempbuf[2];
	
	tempbuf[0] = data & 0xff;
	tempbuf[1] = (data >> 8) & 0xff;

	for (i = 0; i < 2; i++) {
		addr[i] = tempbuf[i];
	}
	
	return SOCKET_TRUE;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Convert4Byte(u8 *addr, float data)
 @Description			��float����ת�������뻺��
 @Input				*addr		: ���ת�������ݵĵ�ַ
					data			: ��ֵ
 @Return				1			: ����
					0			: ����
**********************************************************************************************************/
u8 SOCKET_Convert4Byte(u8 *addr, float data)
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
	
	return SOCKET_TRUE;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Convert4ByteInt(u8 *addr, u32 data)
 @Description			��Int����ת�������뻺��
 @Input				*addr		: ���ת�������ݵĵ�ַ
					data			: ��ֵ
 @Return				1			: ����
					0			: ����
**********************************************************************************************************/
u8 SOCKET_Convert4ByteInt(u8 *addr, u32 data)
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
	
	return SOCKET_TRUE;
}

/**********************************************************************************************************
 @Function			void SOCKET_GetOutputID(u16 *outputid)
 @Description			��ȡoutput_ID����˿ڵĲ�����Socket�������ݰ�
 @Input				Output_ID��ŵ�ַ
 @Return				void
**********************************************************************************************************/
void SOCKET_GetOutputID(u16 *outputid)
{
	u8 i = 0;
	
	for (i = 0; i < OUTPUT_MAX; i++) {
		SocketDataPacket[i].OutputID = outputid[i];
	}
}

/**********************************************************************************************************
 @Function			u16 SOCKET_PackagedStorage(void)
 @Description			Socket�����ݴ�����뻺����
 @Input				void
 @Return				u16 SendLength		: �������ݳ���
**********************************************************************************************************/
u16 SOCKET_PackagedStorage(void)
{
	u16 headlength = 0;
	u16 datalength = 0;
	
	socket_dev.FillData();												//����������������ݰ�
	headlength = socket_dev.ObtainPacketHead((u8 *)SocketSendBuf);				//��ȡSocket��ͷ���ݲ����뻺��
	datalength = socket_dev.ObtainPacketData((u8 *)SocketSendBuf);				//��ȡSocket�������ݰ������뻺��
	headlength = headlength + datalength;
	datalength = socket_dev.ObtainPacketManuCheck((u8 *)SocketSendBuf, headlength);
	headlength = headlength + datalength;

	return headlength;
}

/********************************************** END OF FLEE **********************************************/
