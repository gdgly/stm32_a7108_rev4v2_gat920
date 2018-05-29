/**
  *********************************************************************************************************
  * @file    socketextendfunc.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   Socket Modulation 内部协议
  *********************************************************************************************************
  * @attention
  *			功能 : 
  *			
  *
  *********************************************************************************************************
  */

#include "socketmodulationfunc.h"
#include "string.h"

Socket_Modulation_Data_Frame				SocketModulationDataFrame;			//SocketModulation数据帧
volatile u8 SocketModulationSendBuf[SOCKET_MODULATION_CACHE_SIZE];				//SocketModulation发送数据缓存
volatile u8 SocketModulationReceiveBuf[SOCKET_MODULATION_RECVIVE_SIZE];			//SocketModulation接收数据缓存

/**********************************************************************************************************
 @Function			u16 SOCKET_Modulation_ObtainDataFrame(u8 *addr)
 @Description			SOCKET_Modulation_ObtainDataFrame	: 读取SocketModulation数据帧并填入缓存
 @Input				*addr						: Socket缓存地址
 @Return				DataLength					: 填入缓存数据长度
					SOCKET_EXTEND_FALSE   			: 出错
**********************************************************************************************************/
u16 SOCKET_Modulation_ObtainDataFrame(u8 *addr)
{
	u8 i = 0;
	u16 framelength = 0;
	
	/* 填入帧头 */
	if ((SOCKET_Modulation_Convert2Byte_Big_endian((u8 *)&addr[framelength], SocketModulationDataFrame.HeadFrame)) == SOCKET_MODULATION_FALSE) {
		return SOCKET_MODULATION_FALSE;
	}
	else {
		framelength += 2;
	}
	
	/* 填入包长 */
	if ((SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[framelength], SocketModulationDataFrame.PacketLength)) == SOCKET_MODULATION_FALSE) {
		return SOCKET_MODULATION_FALSE;
	}
	else {
		framelength += 2;
	}
	
	/* 填入数据包类型 */
	addr[framelength] = SocketModulationDataFrame.PacketData.PacketType;
	framelength += 1;
	
	/* 填入设备身份识别码 */
	if ((SOCKET_Modulation_ConvertNByte((u8 *)&addr[framelength], SocketModulationDataFrame.PacketData.DeviceID, 16)) == SOCKET_MODULATION_FALSE) {
		return SOCKET_MODULATION_FALSE;
	}
	else {
		framelength += 16;
	}
	
	/* 填入站点编码 */
	if ((SOCKET_Modulation_ConvertNByte((u8 *)&addr[framelength], SocketModulationDataFrame.PacketData.StationID, 15)) == SOCKET_MODULATION_FALSE) {
		return SOCKET_MODULATION_FALSE;
	}
	else {
		framelength += 15;
	}
	
	/* 填入设备硬件错误码 */
	addr[framelength] = SocketModulationDataFrame.PacketData.DeviceErrCode;
	framelength += 1;
	
	/* 填入调查内容 */
	addr[framelength] = SocketModulationDataFrame.PacketData.SurveyContent;
	framelength += 1;
	
	
	/* 填入年 */
	if ((SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[framelength], SocketModulationDataFrame.PacketData.DateYear)) == SOCKET_MODULATION_FALSE) {
		return SOCKET_MODULATION_FALSE;
	}
	else {
		framelength += 2;
	}
	
	/* 填入月 */
	addr[framelength] = SocketModulationDataFrame.PacketData.DateMon;
	framelength += 1;
	
	/* 填入日 */
	addr[framelength] = SocketModulationDataFrame.PacketData.DateDay;
	framelength += 1;
	
	/* 填入交通数据处理周期 */
	addr[framelength] = SocketModulationDataFrame.PacketData.ProcessCycle;
	framelength += 1;
	
	/* 填入时间序号 */
	if ((SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[framelength], SocketModulationDataFrame.PacketData.TimeSerial)) == SOCKET_MODULATION_FALSE) {
		return SOCKET_MODULATION_FALSE;
	}
	else {
		framelength += 2;
	}
	
	/* 填入车道数 */
	addr[framelength] = SocketModulationDataFrame.PacketData.LaneNum;
	framelength += 1;
	
	for (i = 0; i < (OUTPUT_MAX / 2); i++) {																//遍历找寻配置过的车道号
		if (SocketModulationDataFrame.PacketData.DataLane[i].OutputID != 0) {
			
			/* 填入车道号 */
			addr[framelength] = SocketModulationDataFrame.PacketData.DataLane[i].LaneNo;
			framelength += 1;
			
			/* 填入跟车百分比 */
			addr[framelength] = SocketModulationDataFrame.PacketData.DataLane[i].FollowCar;
			framelength += 1;
			
			/* 填入平均车头间距 */
			if ((SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[framelength], SocketModulationDataFrame.PacketData.DataLane[i].AvgHeadDistance)) == SOCKET_MODULATION_FALSE) {
				return SOCKET_MODULATION_FALSE;
			}
			else {
				framelength += 2;
			}
			
			/* 填入时间占有率 */
			addr[framelength] = SocketModulationDataFrame.PacketData.DataLane[i].TimeOccupancy;
			framelength += 1;
			
			/* 填入小型车交通流量 */
			if ((SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[framelength], SocketModulationDataFrame.PacketData.DataLane[i].SmallVehilceCount)) == SOCKET_MODULATION_FALSE) {
				return SOCKET_MODULATION_FALSE;
			}
			else {
				framelength += 2;
			}
			
			/* 填入小型车平均地点车速 */
			addr[framelength] = SocketModulationDataFrame.PacketData.DataLane[i].SmallVehilceAvgSpeed;
			framelength += 1;
			
			/* 填入中型车交通流量 */
			if ((SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[framelength], SocketModulationDataFrame.PacketData.DataLane[i].MidVehicleCount)) == SOCKET_MODULATION_FALSE) {
				return SOCKET_MODULATION_FALSE;
			}
			else {
				framelength += 2;
			}
			
			/* 填入中型车平均地点车速 */
			addr[framelength] = SocketModulationDataFrame.PacketData.DataLane[i].MidVehicleAvgSpeed;
			framelength += 1;
			
			/* 填入大型车交通流量 */
			if ((SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[framelength], SocketModulationDataFrame.PacketData.DataLane[i].LargerVehicleCount)) == SOCKET_MODULATION_FALSE) {
				return SOCKET_MODULATION_FALSE;
			}
			else {
				framelength += 2;
			}
			
			/* 填入大型车平均地点车速 */
			addr[framelength] = SocketModulationDataFrame.PacketData.DataLane[i].LargerVehicleAvgSpeed;
			framelength += 1;
			
			/* 填入特大型车交通流量 */
			if ((SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[framelength], SocketModulationDataFrame.PacketData.DataLane[i].SupeLargerVehicleCount)) == SOCKET_MODULATION_FALSE) {
				return SOCKET_MODULATION_FALSE;
			}
			else {
				framelength += 2;
			}
			
			/* 填入特大型车平均地点车速 */
			addr[framelength] = SocketModulationDataFrame.PacketData.DataLane[i].SupeLargerVehicleAvgSpeed;
			framelength += 1;
			
			/* 填入拖拉机交通流量 */
			if ((SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[framelength], SocketModulationDataFrame.PacketData.DataLane[i].TractorCount)) == SOCKET_MODULATION_FALSE) {
				return SOCKET_MODULATION_FALSE;
			}
			else {
				framelength += 2;
			}
			
			/* 填入拖拉机平均地点车速 */
			addr[framelength] = SocketModulationDataFrame.PacketData.DataLane[i].TractorAvgSpeed;
			framelength += 1;
			
			/* 填入摩托车交通流量 */
			if ((SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[framelength], SocketModulationDataFrame.PacketData.DataLane[i].MotoCount)) == SOCKET_MODULATION_FALSE) {
				return SOCKET_MODULATION_FALSE;
			}
			else {
				framelength += 2;
			}
			
			/* 填入摩托车平均地点车速 */
			addr[framelength] = SocketModulationDataFrame.PacketData.DataLane[i].MotoAvgSpeed;
			framelength += 1;
		}
	}
	
	/* 填入CRC校验 */
	if ((SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[framelength], SocketModulationDataFrame.CRCCheckCode)) == SOCKET_MODULATION_FALSE) {
		return SOCKET_MODULATION_FALSE;
	}
	else {
		framelength += 2;
	}
	
	/* 填入帧尾 */
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
 @Description			计算校验码并填入校验码
 @Input				*addr		: Socket缓存地址
					buflength		: 数据长度
 @Return				void
**********************************************************************************************************/
void SOCKET_Modulation_ObtainDataFrameCheckCode(u8 *addr, u16 buflength)
{
	u16 CheckCodeCRC16 = 0;
	
	CheckCodeCRC16 = SOCKET_Modulation_CRC16(&addr[2], (buflength - 6));
	
	/* 填入CRC校验 */
	SOCKET_Modulation_Convert2Byte_Little_endian((u8 *)&addr[buflength - 4], CheckCodeCRC16);
}

/**********************************************************************************************************
 @Function			u8 SOCKET_Modulation_Convert2Byte_Little_endian(u8 *addr, u16 data)
 @Description			SOCKET_Modulation_Convert2Byte_Little_endian		: 将2Byte数据转换并填入缓存(小端)
 @Input				*addr									: 存放转换后数据的地址
					data										: 数值
 @Return				1										: 错误
					0										: 正常
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
 @Description			SOCKET_Modulation_Convert2Byte_Big_endian		: 将2Byte数据转换并填入缓存(大端)
 @Input				*addr									: 存放转换后数据的地址
					data										: 数值
 @Return				1										: 错误
					0										: 正常
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
 @Description			SOCKET_Modulation_Convert4Byte_Little_endian		: 将4Byte数据转换并填入缓存(小端)
 @Input				*addr									: 存放转换后数据的地址
					data										: 数值
 @Return				1										: 错误
					0										: 正常
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
 @Description			SOCKET_Modulation_Convert4Byte_Big_endian		: 将4Byte数据转换并填入缓存(大端)
 @Input				*addr									: 存放转换后数据的地址
					data										: 数值
 @Return				1										: 错误
					0										: 正常
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
 @Description			SOCKET_Modulation_ConvertNByte				: 将NByte数据填入缓存
 @Input				*addr									: 存放转换后数据的地址
					data										: 数值
 @Return				1										: 错误
					0										: 正常
**********************************************************************************************************/
u8 SOCKET_Modulation_ConvertNByte(u8 *addr, u8 *data, u16 len)
{
	memcpy(addr, data, len);
	
	return SOCKET_MODULATION_TRUE;
}

/**********************************************************************************************************
 @Function			void SOCKET_Modulation_GetOutputID(u16 *outputid)
 @Description			SOCKET_Modulation_GetOutputID	: 读取output_ID输出端口的参数到SocketModulation车道数据包
 @Input				Output_ID存放地址
 @Return				void
**********************************************************************************************************/
void SOCKET_Modulation_GetOutputID(u16 *outputid)
{
	u8 i = 0;
	
	for (i = 0; i < OUTPUT_MAX; i++) {
		SocketModulationDataFrame.PacketData.DataLane[i].OutputID = outputid[i];
	}
}

/* CRC高位字节值表 */
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

/* CRC低位字节值表 */
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
 @Description			SOCKET_Modulation_CRC16		: CRC校验
 @Input				puchMsg					: 需校验消息
					usDataLen					: 消息中字节数
 @Return				CRCCheckCode				: 校验码
**********************************************************************************************************/
unsigned short SOCKET_Modulation_CRC16(unsigned char *puchMsg, unsigned short usDataLen)
{
	unsigned char uchCRCHi = 0xFF;										// 高CRC字节初始化
	unsigned char uchCRCLo = 0xFF;										// 低CRC字节初始化
	unsigned uIndex;													// CRC循环索引
	
	while (usDataLen--) {
		
		uIndex = uchCRCLo ^ *puchMsg++;
		
		uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex];
		
		uchCRCHi = auchCRCLo[uIndex];
	}
	
	return (uchCRCHi << 8 | uchCRCLo);
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Modulation_PackagedStorage(void)
 @Description			SocketModulation将数据打包存入缓存区
 @Input				void
 @Return				u16 SendLength		: 发送数据长度
**********************************************************************************************************/
u16 SOCKET_Modulation_PackagedStorage(void)
{
	u16 sendlength = 0;
	
	socket_modulation_dev.FillData();													//将数据填入各个数据包
	sendlength = socket_modulation_dev.ObtainDataFrame((u8 *)SocketModulationSendBuf);			//读取SocketExtend包头数据并填入缓存
	socket_modulation_dev.ObtainDataFrameCheckCode((u8 *)SocketModulationSendBuf, sendlength);	//计算校验码并填入校验码
	
	return sendlength;
}

/********************************************** END OF FLEE **********************************************/
