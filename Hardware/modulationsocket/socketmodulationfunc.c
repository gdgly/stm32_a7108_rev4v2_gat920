/**
  *********************************************************************************************************
  * @file    socketextendfunc.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   Socket Modulation �ڲ�Э��
  *********************************************************************************************************
  * @attention
  *			���� : 
  *			
  *
  *********************************************************************************************************
  */

#include "socketmodulationfunc.h"
#include "string.h"

Socket_Modulation_Data_Frame				SocketModulationDataFrame;			//SocketModulation����֡
volatile u8 SocketModulationSendBuf[SOCKET_MODULATION_CACHE_SIZE];				//SocketModulation�������ݻ���
volatile u8 SocketModulationReceiveBuf[SOCKET_MODULATION_RECVIVE_SIZE];			//SocketModulation�������ݻ���

/**********************************************************************************************************
 @Function			u16 SOCKET_Modulation_ObtainDataFrame(u8 *addr)
 @Description			SOCKET_Modulation_ObtainDataFrame	: ��ȡSocketModulation����֡�����뻺��
 @Input				*addr						: Socket�����ַ
 @Return				DataLength					: ���뻺�����ݳ���
					SOCKET_EXTEND_FALSE   			: ����
**********************************************************************************************************/
u16 SOCKET_Modulation_ObtainDataFrame(u8 *addr)
{
	u8 i = 0;
	u16 framelength = 0;
	
	/* ����֡ͷ */
	if ((SOCKET_Modulation_Convert2Byte_Big_endian((u8 *)&addr[framelength], SocketModulationDataFrame.HeadFrame)) == SOCKET_MODULATION_FALSE) {
		return SOCKET_MODULATION_FALSE;
	}
	else {
		framelength += 2;
	}
	
	/* ������� */
	if ((SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[framelength], SocketModulationDataFrame.PacketLength)) == SOCKET_MODULATION_FALSE) {
		return SOCKET_MODULATION_FALSE;
	}
	else {
		framelength += 2;
	}
	
	/* �������ݰ����� */
	addr[framelength] = SocketModulationDataFrame.PacketData.PacketType;
	framelength += 1;
	
	/* �����豸���ʶ���� */
	if ((SOCKET_Modulation_ConvertNByte((u8 *)&addr[framelength], SocketModulationDataFrame.PacketData.DeviceID, 16)) == SOCKET_MODULATION_FALSE) {
		return SOCKET_MODULATION_FALSE;
	}
	else {
		framelength += 16;
	}
	
	/* ����վ����� */
	if ((SOCKET_Modulation_ConvertNByte((u8 *)&addr[framelength], SocketModulationDataFrame.PacketData.StationID, 15)) == SOCKET_MODULATION_FALSE) {
		return SOCKET_MODULATION_FALSE;
	}
	else {
		framelength += 15;
	}
	
	/* �����豸Ӳ�������� */
	addr[framelength] = SocketModulationDataFrame.PacketData.DeviceErrCode;
	framelength += 1;
	
	/* ����������� */
	addr[framelength] = SocketModulationDataFrame.PacketData.SurveyContent;
	framelength += 1;
	
	
	/* ������ */
	if ((SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[framelength], SocketModulationDataFrame.PacketData.DateYear)) == SOCKET_MODULATION_FALSE) {
		return SOCKET_MODULATION_FALSE;
	}
	else {
		framelength += 2;
	}
	
	/* ������ */
	addr[framelength] = SocketModulationDataFrame.PacketData.DateMon;
	framelength += 1;
	
	/* ������ */
	addr[framelength] = SocketModulationDataFrame.PacketData.DateDay;
	framelength += 1;
	
	/* ���뽻ͨ���ݴ������� */
	addr[framelength] = SocketModulationDataFrame.PacketData.ProcessCycle;
	framelength += 1;
	
	/* ����ʱ����� */
	if ((SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[framelength], SocketModulationDataFrame.PacketData.TimeSerial)) == SOCKET_MODULATION_FALSE) {
		return SOCKET_MODULATION_FALSE;
	}
	else {
		framelength += 2;
	}
	
	/* ���복���� */
	addr[framelength] = SocketModulationDataFrame.PacketData.LaneNum;
	framelength += 1;
	
	for (i = 0; i < (OUTPUT_MAX / 2); i++) {																//������Ѱ���ù��ĳ�����
		if (SocketModulationDataFrame.PacketData.DataLane[i].OutputID != 0) {
			
			/* ���복���� */
			addr[framelength] = SocketModulationDataFrame.PacketData.DataLane[i].LaneNo;
			framelength += 1;
			
			/* ��������ٷֱ� */
			addr[framelength] = SocketModulationDataFrame.PacketData.DataLane[i].FollowCar;
			framelength += 1;
			
			/* ����ƽ����ͷ��� */
			if ((SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[framelength], SocketModulationDataFrame.PacketData.DataLane[i].AvgHeadDistance)) == SOCKET_MODULATION_FALSE) {
				return SOCKET_MODULATION_FALSE;
			}
			else {
				framelength += 2;
			}
			
			/* ����ʱ��ռ���� */
			addr[framelength] = SocketModulationDataFrame.PacketData.DataLane[i].TimeOccupancy;
			framelength += 1;
			
			/* ����С�ͳ���ͨ���� */
			if ((SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[framelength], SocketModulationDataFrame.PacketData.DataLane[i].SmallVehilceCount)) == SOCKET_MODULATION_FALSE) {
				return SOCKET_MODULATION_FALSE;
			}
			else {
				framelength += 2;
			}
			
			/* ����С�ͳ�ƽ���ص㳵�� */
			addr[framelength] = SocketModulationDataFrame.PacketData.DataLane[i].SmallVehilceAvgSpeed;
			framelength += 1;
			
			/* �������ͳ���ͨ���� */
			if ((SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[framelength], SocketModulationDataFrame.PacketData.DataLane[i].MidVehicleCount)) == SOCKET_MODULATION_FALSE) {
				return SOCKET_MODULATION_FALSE;
			}
			else {
				framelength += 2;
			}
			
			/* �������ͳ�ƽ���ص㳵�� */
			addr[framelength] = SocketModulationDataFrame.PacketData.DataLane[i].MidVehicleAvgSpeed;
			framelength += 1;
			
			/* ������ͳ���ͨ���� */
			if ((SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[framelength], SocketModulationDataFrame.PacketData.DataLane[i].LargerVehicleCount)) == SOCKET_MODULATION_FALSE) {
				return SOCKET_MODULATION_FALSE;
			}
			else {
				framelength += 2;
			}
			
			/* ������ͳ�ƽ���ص㳵�� */
			addr[framelength] = SocketModulationDataFrame.PacketData.DataLane[i].LargerVehicleAvgSpeed;
			framelength += 1;
			
			/* �����ش��ͳ���ͨ���� */
			if ((SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[framelength], SocketModulationDataFrame.PacketData.DataLane[i].SupeLargerVehicleCount)) == SOCKET_MODULATION_FALSE) {
				return SOCKET_MODULATION_FALSE;
			}
			else {
				framelength += 2;
			}
			
			/* �����ش��ͳ�ƽ���ص㳵�� */
			addr[framelength] = SocketModulationDataFrame.PacketData.DataLane[i].SupeLargerVehicleAvgSpeed;
			framelength += 1;
			
			/* ������������ͨ���� */
			if ((SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[framelength], SocketModulationDataFrame.PacketData.DataLane[i].TractorCount)) == SOCKET_MODULATION_FALSE) {
				return SOCKET_MODULATION_FALSE;
			}
			else {
				framelength += 2;
			}
			
			/* ����������ƽ���ص㳵�� */
			addr[framelength] = SocketModulationDataFrame.PacketData.DataLane[i].TractorAvgSpeed;
			framelength += 1;
			
			/* ����Ħ�г���ͨ���� */
			if ((SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[framelength], SocketModulationDataFrame.PacketData.DataLane[i].MotoCount)) == SOCKET_MODULATION_FALSE) {
				return SOCKET_MODULATION_FALSE;
			}
			else {
				framelength += 2;
			}
			
			/* ����Ħ�г�ƽ���ص㳵�� */
			addr[framelength] = SocketModulationDataFrame.PacketData.DataLane[i].MotoAvgSpeed;
			framelength += 1;
		}
	}
	
	/* ����CRCУ�� */
	if ((SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[framelength], SocketModulationDataFrame.CRCCheckCode)) == SOCKET_MODULATION_FALSE) {
		return SOCKET_MODULATION_FALSE;
	}
	else {
		framelength += 2;
	}
	
	/* ����֡β */
	if ((SOCKET_Modulation_Convert2Byte_Big_endian((u8 *)&addr[framelength], SocketModulationDataFrame.TailFrame)) == SOCKET_MODULATION_FALSE) {
		return SOCKET_MODULATION_FALSE;
	}
	else {
		framelength += 2;
	}
	
	return framelength;
}

/**********************************************************************************************************
 @Function			void SOCKET_Modulation_ObtainDataFrameCheckCode(u8 *addr, u16 buflength)
 @Description			����У���벢����У����
 @Input				*addr		: Socket�����ַ
					buflength		: ���ݳ���
 @Return				void
**********************************************************************************************************/
void SOCKET_Modulation_ObtainDataFrameCheckCode(u8 *addr, u16 buflength)
{
	u16 CheckCodeCRC16 = 0;
	
	CheckCodeCRC16 = SOCKET_Modulation_CRC16(&addr[2], (buflength - 6));
	
	/* ����CRCУ�� */
	SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[buflength - 4], CheckCodeCRC16);
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_Convert2Byte_Little_endian(u8 *addr, u16 data)
 @Description			SOCKET_Modulation_Convert2Byte_Little_endian		: ��2Byte����ת�������뻺��(С��)
 @Input				*addr									: ���ת�������ݵĵ�ַ
					data										: ��ֵ
 @Return				1										: ����
					0										: ����
**********************************************************************************************************/
u8 SOCKET_Modulation_Convert2Byte_Little_endian(u8 *addr, u16 data)
{
	u8 i = 0;
	u8 tempbuf[2];
	
	tempbuf[0] = data & 0xff;
	tempbuf[1] = (data >> 8) & 0xff;

	for (i = 0; i < 2; i++) {
		addr[i] = tempbuf[i];
	}
	
	return SOCKET_MODULATION_TRUE;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_Convert2Byte_Big_endian(u8 *addr, u16 data)
 @Description			SOCKET_Modulation_Convert2Byte_Big_endian		: ��2Byte����ת�������뻺��(���)
 @Input				*addr									: ���ת�������ݵĵ�ַ
					data										: ��ֵ
 @Return				1										: ����
					0										: ����
**********************************************************************************************************/
u8 SOCKET_Modulation_Convert2Byte_Big_endian(u8 *addr, u16 data)
{
	u8 i = 0;
	u8 tempbuf[2];
	
	tempbuf[1] = data & 0xff;
	tempbuf[0] = (data >> 8) & 0xff;

	for (i = 0; i < 2; i++) {
		addr[i] = tempbuf[i];
	}
	
	return SOCKET_MODULATION_TRUE;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_Convert4Byte_Little_endian(u8 *addr, u32 data)
 @Description			SOCKET_Modulation_Convert4Byte_Little_endian		: ��4Byte����ת�������뻺��(С��)
 @Input				*addr									: ���ת�������ݵĵ�ַ
					data										: ��ֵ
 @Return				1										: ����
					0										: ����
**********************************************************************************************************/
u8 SOCKET_Modulation_Convert4Byte_Little_endian(u8 *addr, u32 data)
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
	
	return SOCKET_MODULATION_TRUE;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_Convert4Byte_Big_endian(u8 *addr, u32 data)
 @Description			SOCKET_Modulation_Convert4Byte_Big_endian		: ��4Byte����ת�������뻺��(���)
 @Input				*addr									: ���ת�������ݵĵ�ַ
					data										: ��ֵ
 @Return				1										: ����
					0										: ����
**********************************************************************************************************/
u8 SOCKET_Modulation_Convert4Byte_Big_endian(u8 *addr, u32 data)
{
	u8 i = 0;
	u8 tempbuf[4];
	
	tempbuf[3] = data & 0xff;
	tempbuf[2] = (data >> 8) & 0xff;
	tempbuf[1] = (data >> 16) & 0xff;
	tempbuf[0] = data >> 24;
	
	for (i = 0; i < 4; i++) {
		addr[i] = tempbuf[i];
	}
	
	return SOCKET_MODULATION_TRUE;
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_ConvertNByte(u8 *addr, u8 *data, u16 len)
 @Description			SOCKET_Modulation_ConvertNByte				: ��NByte�������뻺��
 @Input				*addr									: ���ת�������ݵĵ�ַ
					data										: ��ֵ
 @Return				1										: ����
					0										: ����
**********************************************************************************************************/
u8 SOCKET_Modulation_ConvertNByte(u8 *addr, u8 *data, u16 len)
{
	memcpy(addr, data, len);
	
	return SOCKET_MODULATION_TRUE;
}

/**********************************************************************************************************
 @Function			void SOCKET_Modulation_GetOutputID(u16 *outputid)
 @Description			SOCKET_Modulation_GetOutputID	: ��ȡoutput_ID����˿ڵĲ�����SocketModulation�������ݰ�
 @Input				Output_ID��ŵ�ַ
 @Return				void
**********************************************************************************************************/
void SOCKET_Modulation_GetOutputID(u16 *outputid)
{
	u8 i = 0;
	
	for (i = 0; i < OUTPUT_MAX; i++) {
		SocketModulationDataFrame.PacketData.DataLane[i].OutputID = outputid[i];
	}
}

/* CRC��λ�ֽ�ֵ�� */
static unsigned char auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
	
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
	
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
	
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,

0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,

0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,

0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,

0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,

0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,

0x40
};

/* CRC��λ�ֽ�ֵ�� */
static unsigned char auchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,

0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,

0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,

0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,

0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
	
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
	
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
	
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,

0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,

0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,

0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,

0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,

0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,

0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,

0x40
};

/**********************************************************************************************************
 @Function			unsigned short SOCKET_Modulation_CRC16(unsigned char *puchMsg, unsigned short usDataLen)
 @Description			SOCKET_Modulation_CRC16		: CRCУ��
 @Input				puchMsg					: ��У����Ϣ
					usDataLen					: ��Ϣ���ֽ���
 @Return				CRCCheckCode				: У����
**********************************************************************************************************/
unsigned short SOCKET_Modulation_CRC16(unsigned char *puchMsg, unsigned short usDataLen)
{
	unsigned char uchCRCHi = 0xFF;										// ��CRC�ֽڳ�ʼ��
	unsigned char uchCRCLo = 0xFF;										// ��CRC�ֽڳ�ʼ��
	unsigned uIndex;													// CRCѭ������
	
	while (usDataLen--) {
		
		uIndex = uchCRCLo ^ *puchMsg++;
		
		uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex];
		
		uchCRCHi = auchCRCLo[uIndex];
	}
	
	return (uchCRCHi << 8 | uchCRCLo);
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Modulation_PackagedStorage(void)
 @Description			SocketModulation�����ݴ�����뻺����
 @Input				void
 @Return				u16 SendLength		: �������ݳ���
**********************************************************************************************************/
u16 SOCKET_Modulation_PackagedStorage(void)
{
	u16 sendlength = 0;
	
	socket_modulation_dev.FillData();													//����������������ݰ�
	sendlength = socket_modulation_dev.ObtainDataFrame((u8 *)SocketModulationSendBuf);			//��ȡSocketExtend��ͷ���ݲ����뻺��
	socket_modulation_dev.ObtainDataFrameCheckCode((u8 *)SocketModulationSendBuf, sendlength);	//����У���벢����У����
	
	return sendlength;
}

/********************************************** END OF FLEE **********************************************/
