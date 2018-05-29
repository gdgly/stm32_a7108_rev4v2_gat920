/**
  *********************************************************************************************************
  * @file    socketextendfunc.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   socket Extend 内部协议
  *********************************************************************************************************
  * @attention
  *			功能 : 
  *			1.  读取SocketExtend包头数据并填入缓存							(外部调用)
  *			2.  读取SocketExtend流量数据包并填入缓存						(外部调用)
  *			3.  计算校验码并填入校验码									(外部调用)
  *
  *			4.  将crossid转换为字符格式并填入缓存							(外部调用)
  *			5.  将2Byte数据转换并填入缓存									(外部调用)
  *			6.  将float数据转换并填入缓存									(外部调用)
  *			7.  将Int数据转换并填入缓存									(外部调用)
  *
  *			8.  读取output_ID输出端口的参数到SocketExtend流量数据包			(外部调用)
  *			9.  SocketExtend将数据打包存入缓存区							(外部调用)
  *			10. SocketExtend将对时数据打包存入缓存区						(外部调用)
  *
  *********************************************************************************************************
  */

#include "socketextendfunc.h"

Socket_Extend_Packet_Head			SocketExtendHeadPacket;					//SocketExtend数据包头
Socket_Extend_Packet_Data			SocketExtendDataPacket[OUTPUT_MAX];		//SocketExtend流量数据包集
volatile u8 SocketExtendSendBuf[SOCKET_EXTEND_CACHE_SIZE];						//SocketExtend发送数据缓存
volatile u8 SocketExtendReceiveBuf[SOCKET_EXTEND_RECVIVE_SIZE];					//SocketExtend接收数据缓存

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_ObtainPacketHead(u8 *addr)
 @Description			SOCKET_Extend_ObtainPacketHead	: 读取SocketExtend包头数据并填入缓存
 @Input				*addr						: Socket缓存地址
 @Return				DataLength					: 填入缓存数据长度
					SOCKET_EXTEND_FALSE   			: 出错
**********************************************************************************************************/
u16 SOCKET_Extend_ObtainPacketHead(u8 *addr)
{
	u16 headlength = 0;
	
	addr[headlength] = SocketExtendHeadPacket.StartX;															//填入起始字
	headlength += 1;
	
	addr[headlength] = SocketExtendHeadPacket.ServerType;														//填入业务类型
	headlength += 1;
	
	if ((SOCKET_Extend_ConvertCrossID((u8 *)&addr[headlength], SocketExtendHeadPacket.CrossID)) == SOCKET_EXTEND_FALSE) {	//填入CrossID
		return SOCKET_EXTEND_FALSE;
	}
	else {
		headlength += 8;
	}
	
	if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[headlength], SocketExtendHeadPacket.DataLength)) == SOCKET_EXTEND_FALSE) {	//填入数据长度
		return SOCKET_EXTEND_FALSE;
	}
	else {
		headlength += 2;
	}
	
	if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[headlength], SocketExtendHeadPacket.Channel)) == SOCKET_EXTEND_FALSE) {	//填入传送组号
		return SOCKET_EXTEND_FALSE;
	}
	else {
		headlength += 2;
	}
	
	addr[headlength] = SocketExtendHeadPacket.ExitX;															//填入结束字
	headlength += 1;
	
	addr[headlength] = SocketExtendHeadPacket.CheckCode;														//填入校验字
	headlength += 1;
	
	addr[headlength] = SocketExtendHeadPacket.DataCount;														//填入数据条数
	headlength += 1;
	
	return headlength;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_ObtainPacketData(u8 *addr)
 @Description			SOCKET_Extend_ObtainPacketData	: 读取SocketExtend流量数据包并填入缓存
 @Input				*addr						: Socket缓存地址
 @Return				DataLength					: 填入缓存数据长度
					SOCKET_FALSE   				: 出错
**********************************************************************************************************/
u16 SOCKET_Extend_ObtainPacketData(u8 *addr)
{
	u8 i = 0;
	u16 datalength = 0;
	
	datalength += 17;																				//地址偏移出数据包头
	
	for (i = 0; i < OUTPUT_MAX; i++) {																	//遍历找寻配置过的车道号
		if (SocketExtendDataPacket[i].OutputID != 0) {
			
			/* 填入流水号 */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].RecordNo)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* 填入上报时间 */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].SendTime)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* 填入车流量 */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].VehicleCount)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* 填入大车流量 */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].LargerVehicleCount)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* 填入中车流量 */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].MidVehicleCount)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* 填入小车流量 */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].SmallVehilceCount)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* 填入微型车流量 */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].MiniVehicleCount)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* 填入摩托车流量 */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].Moto)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* 填入预留车型1 */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].ReserverType1)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* 填入预留车型2 */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].ReserverType2)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* 填入预留车型3 */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].ReserverType3)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* 填入预留车型4 */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].ReserverType4)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* 填入预留车型5 */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].ReserverType5)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* 填入预留车型6 */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].ReserverType6)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* 填入最大车头时距 */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].MaxHeadInterval)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* 填入最小车头时距 */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].MinHeadInterval)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* 填入累计车头时距 */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].AddUpHeadInterval)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* 填入最大车速 */
			if ((SOCKET_Extend_Convert4Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].MaxSpeed)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* 填入最小车速 */
			if ((SOCKET_Extend_Convert4Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].MinSpeed)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* 填入累计车速 */
			if ((SOCKET_Extend_Convert4Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].AddUpSpeed)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* 填入最大占有时间 */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].MaxOccupancy)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* 填入最小占有时间 */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].MinOccupancy)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* 填入累计占有时间 */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].AddUpOccupancy)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* 填入最大间隔时间 */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].MaxInterval)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* 填入最小间隔时间 */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].MinInterval)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* 填入累计间隔时间 */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].AddUpInterval)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* 填入最大车长 */
			if ((SOCKET_Extend_Convert4Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].MaxVehicleLength)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* 填入最小车长 */
			if ((SOCKET_Extend_Convert4Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].MinVehicleLength)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* 填入累计车长 */
			if ((SOCKET_Extend_Convert4Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].AddUpVehilcleLength)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* 填入最大排队长度 */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].MaxQueueLength)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* 填入最小排队长度 */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].MinQueueLength)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* 填入累计排队长度 */
			if ((SOCKET_Extend_Convert4ByteInt((u8 *)&addr[datalength], SocketExtendDataPacket[i].AddUpQueueLength)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 4;
			}
			
			/* 填入累计排队时间 */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].AddUpQueueTime)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* 填入闯红灯触发次数 */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].RedLightCount)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* 填入前方车道满累计时间 */
			if ((SOCKET_Extend_Convert2Byte((u8 *)&addr[datalength], SocketExtendDataPacket[i].PrecedingWayFullTime)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 2;
			}
			
			/* 填入车道代码 */
			if ((SOCKET_Extend_ConvertDirverWayCode((u8 *)&addr[datalength], SocketExtendDataPacket[i].DirverWayCode)) == SOCKET_EXTEND_FALSE) {
				return SOCKET_EXTEND_FALSE;
			}
			else {
				datalength += 3;
			}
			
			/* 填入状态 */
			addr[datalength] = SocketExtendDataPacket[i].Status;
			datalength += 1;
			
			/* 填入预留 */
			addr[datalength] = SocketExtendDataPacket[i].Reserver1;
			datalength += 1;
			
			/* 填入预留 */
			addr[datalength] = SocketExtendDataPacket[i].Reserver2;
			datalength += 1;
		}
	}
	
	datalength -= 17;																				//减去数据包头16byte
	
	return datalength;
}

/**********************************************************************************************************
 @Function			void SOCKET_Extend_ObtainPacketCheckCode(u8 *addr, u16 buflength)
 @Description			计算校验码并填入校验码
 @Input				*addr		: Socket缓存地址
					buflength		: 数据长度
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
 @Description			SOCKET_Extend_ConvertCrossID		: 将crossid转换为字符格式并填入缓存
 @Input				*addr						: 存放转换后数据的地址
					crossid						: CrossId值
 @Return				1							: 错误
					0							: 正常
**********************************************************************************************************/
u8 SOCKET_Extend_ConvertCrossID(u8 *addr, u32 crossid)
{
	u8 i = 0;
	u8 tempbuf[8];
	
	if (crossid > 99999999) {											//检测CrossId值是否错误
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
 @Description			SOCKET_Extend_ConvertDirverWayCode	: 将dirverWayCode转换为字符格式并填入缓存
 @Input				*addr						: 存放转换后数据的地址
					dirverWayCode					: dirverWayCode值
 @Return				1							: 错误
					0							: 正常
**********************************************************************************************************/
u8 SOCKET_Extend_ConvertDirverWayCode(u8 *addr, u32 dirverWayCode)
{
	u8 i = 0;
	u8 tempbuf[3];
	
	if (dirverWayCode > 999) {											//检测LaneNo值是否错误
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
 @Description			SOCKET_Extend_Convert2Byte	: 将2Byte数据转换并填入缓存
 @Input				*addr					: 存放转换后数据的地址
					data						: 数值
 @Return				1						: 错误
					0						: 正常
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
 @Description			SOCKET_Extend_Convert4Byte	: 将float数据转换并填入缓存
 @Input				*addr					: 存放转换后数据的地址
					data						: 数值
 @Return				1						: 错误
					0						: 正常
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
 @Description			SOCKET_Extend_Convert4ByteInt	: 将Int数据转换并填入缓存
 @Input				*addr					: 存放转换后数据的地址
					data						: 数值
 @Return				1						: 错误
					0						: 正常
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
 @Description			SOCKET_Extend_GetOutputID	: 读取output_ID输出端口的参数到SocketExtend流量数据包
 @Input				Output_ID存放地址
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
 @Description			SocketExtend将数据打包存入缓存区
 @Input				void
 @Return				u16 SendLength		: 发送数据长度
**********************************************************************************************************/
u16 SOCKET_Extend_PackagedStorage(void)
{
	u16 headlength = 0;
	u16 datalength = 0;
	
	socket_extend_dev.FillData();											//将数据填入各个数据包
	headlength = socket_extend_dev.ObtainPacketHead((u8 *)SocketExtendSendBuf);	//读取SocketExtend包头数据并填入缓存
	datalength = socket_extend_dev.ObtainPacketData((u8 *)SocketExtendSendBuf);	//读取SocketExtend流量数据包并填入缓存
	headlength = headlength + datalength;
	socket_extend_dev.ObtainPacketCheckCode((u8 *)SocketExtendSendBuf, headlength);	//计算校验码并填入校验码
	
	return headlength;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Extend_RtcCheckPackagedStorage(void)
 @Description			SocketExtend将对时数据打包存入缓存区
 @Input				void
 @Return				u16 SendLength		: 发送数据长度
**********************************************************************************************************/
u16 SOCKET_Extend_RtcCheckPackagedStorage(void)
{
	u16 headlength = 0;
	
	socket_extend_dev.RtcCheckFillData();
	headlength = socket_extend_dev.ObtainPacketHead((u8 *)SocketExtendSendBuf);	//读取SocketExtend包头数据并填入缓存
	socket_extend_dev.ObtainPacketCheckCode((u8 *)SocketExtendSendBuf, headlength);	//计算校验码并填入校验码
	
	return (headlength - 1);
}

/********************************************** END OF FLEE **********************************************/
