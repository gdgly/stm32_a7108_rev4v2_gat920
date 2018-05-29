/**
  *********************************************************************************************************
  * @file    socketpark.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   Socket停车场数据处理
  *********************************************************************************************************
  * @attention
  *			
  *
  *			
  *********************************************************************************************************
  */

#include "socketpark.h"
#include "socketconfig.h"
#include "socketfunc.h"
#include "socketinstantia.h"
#include "calculationconfig.h"
#include "calculationavgspeed.h"
#include "socketinitialization.h"


#ifdef SOCKET_SERIALPORT_USART1
#define SOCKET_USART		USART1
#endif
#ifdef SOCKET_SERIALPORT_USART2
#define SOCKET_USART		USART2
#endif
#ifdef SOCKET_SERIALPORT_USART3
#define SOCKET_USART		USART3
#endif

extern unsigned short 		output_ID[OUTPUT_MAX];

Socket_Packet_Head			SocketParkHeadPacket;							//Socket停车场数据包头
Socket_Packet_Data			SocketParkDataPacket;							//Socket停车场数据包


/**********************************************************************************************************
 @Function			void SOCKET_ParkImplement(u8 laneNo, u16 CarNum, u8 CarInorOut)
 @Description			Socket数据实时上传
 @Input				laneNo		: 车道号
					CarNum 		: 车辆值
					CarInorOut	: 车辆进入或离开
 @Return				void
**********************************************************************************************************/
void SOCKET_ParkImplement(u8 laneNo, u16 CarNum, u8 CarInorOut)
{
	u8 indexA = 0;
	u8 indexB = 0;
	
	u8 i = 0;
	u8 outputnum = 0;
	u16 headlength = 0;
	u16 datalength = 0;
	
	indexA = laneNo;
	
	for (i = 0; i < OUTPUT_MAX; i++) {																		//获取已配置ID号最大值
		if (SocketDataPacket[i].OutputID != 0) {
			outputnum = i;
		}
	}
	
	if (outputnum >= SPEEDLANNUMMAX) {																		//判断单地磁测速还是双地磁测速
		/* 双地磁 */
		if (indexA < SPEEDLANNUMMAX) {																	//主地磁需获取数据
			for (indexB = indexA; indexB > 0; indexB--) {													//寻找本包与上一包同ID数据
				if (SocketDataPacket[indexA].OutputID == SocketDataPacket[indexB - 1].OutputID) {
					SocketDataPacket[indexA].DeviceType = SocketDataPacket[indexB - 1].DeviceType;						//写入DeviceType
					SocketDataPacket[indexA].Interval = SocketDataPacket[indexB - 1].Interval;							//写入Interval
					SocketDataPacket[indexA].LaneNo = indexA + 1;												//写入LaneNo
					SocketDataPacket[indexA].DateTime = SocketDataPacket[indexB - 1].DateTime;							//写入DateTime
					SocketDataPacket[indexA].Volume = SocketDataPacket[indexB - 1].Volume;							//写入Volume
					SocketDataPacket[indexA].Volume1 = SocketDataPacket[indexB - 1].Volume1;							//写入Volume1
					SocketDataPacket[indexA].Volume2 = SocketDataPacket[indexB - 1].Volume2;							//写入Volume2
					SocketDataPacket[indexA].Volume3 = SocketDataPacket[indexB - 1].Volume3;							//写入Volume3
					SocketDataPacket[indexA].Volume4 = SocketDataPacket[indexB - 1].Volume4;							//写入Volume4
					SocketDataPacket[indexA].Volume5 = SocketDataPacket[indexB - 1].Volume5;							//写入Volume5
					SocketDataPacket[indexA].AvgOccupancy = SocketDataPacket[indexB - 1].AvgOccupancy;					//写入AvgOccupancy
					SocketDataPacket[indexA].AvgHeadTime = SocketDataPacket[indexB - 1].AvgHeadTime;					//写入AvgHeadTime
					SocketDataPacket[indexA].AvgSpeed = SocketDataPacket[indexB - 1].AvgSpeed;							//写入AvgSpeed
					SocketDataPacket[indexA].AvgLength = SocketDataPacket[indexB - 1].AvgLength;						//写入AvgLength
					SocketDataPacket[indexA].Saturation = SocketDataPacket[indexB - 1].Saturation;						//写入Saturation
					SocketDataPacket[indexA].Density = SocketDataPacket[indexB - 1].Density;							//写入Density
					SocketDataPacket[indexA].Pcu = SocketDataPacket[indexB - 1].Pcu;									//写入Pcu
					SocketDataPacket[indexA].AvgQueueLength = SocketDataPacket[indexB - 1].AvgQueueLength;				//写入AvgQueueLength
					break;
				}
			}
			if (indexB == 0) {																			//本数据包无同ID数据包,获取数据
				SocketDataPacket[indexA].DeviceType = socket_dev.GetDeviceType(SocketDataPacket[indexA].OutputID);			//写入DeviceType
				SocketDataPacket[indexA].Interval = socket_dev.GetInterval(SocketDataPacket[indexA].OutputID);				//写入Interval
				SocketDataPacket[indexA].LaneNo = indexA + 1;													//写入LaneNo
				SocketDataPacket[indexA].DateTime = socket_dev.GetDateTime(SocketDataPacket[indexA].OutputID);				//写入DateTime
				SocketDataPacket[indexA].Volume = socket_dev.GetVolume(SocketDataPacket[indexA].OutputID);				//写入Volume
				SocketDataPacket[indexA].Volume1 = socket_dev.GetVolume1(SocketDataPacket[indexA].OutputID);				//写入Volume1
				SocketDataPacket[indexA].Volume2 = socket_dev.GetVolume2(SocketDataPacket[indexA].OutputID);				//写入Volume2
				SocketDataPacket[indexA].Volume3 = socket_dev.GetVolume3(SocketDataPacket[indexA].OutputID);				//写入Volume3
				SocketDataPacket[indexA].Volume4 = socket_dev.GetVolume4(SocketDataPacket[indexA].OutputID);				//写入Volume4
				SocketDataPacket[indexA].Volume5 = socket_dev.GetVolume5(SocketDataPacket[indexA].OutputID);				//写入Volume5
				SocketDataPacket[indexA].AvgOccupancy = socket_dev.GetAvgOccupancy(SocketDataPacket[indexA].OutputID);		//写入AvgOccupancy
				SocketDataPacket[indexA].AvgHeadTime = socket_dev.GetAvgHeadTime(SocketDataPacket[indexA].OutputID);		//写入AvgHeadTime
				SocketDataPacket[indexA].AvgSpeed = 0;															//写入AvgSpeed
				SocketDataPacket[indexA].AvgLength = 0;															//写入AvgLength
				SocketDataPacket[indexA].Saturation = socket_dev.GetSaturation(SocketDataPacket[indexA].OutputID);			//写入Saturation
				SocketDataPacket[indexA].Density = socket_dev.GetDensity(SocketDataPacket[indexA].OutputID);				//写入Density
				SocketDataPacket[indexA].Pcu = socket_dev.GetPcu(SocketDataPacket[indexA].OutputID);						//写入Pcu
				SocketDataPacket[indexA].AvgQueueLength = socket_dev.GetAvgQueueLength(SocketDataPacket[indexA].OutputID);	//写入AvgQueueLength
			}
		}
		else {																						//辅地磁获取数据
			if (SocketDataPacket[indexA - SPEEDLANNUMMAX].OutputID != 0) {										//辅地磁对应主地磁有配置
				SocketDataPacket[indexA].DeviceType = socket_dev.GetDeviceType(SocketDataPacket[indexA].OutputID);			//写入DeviceType
				SocketDataPacket[indexA].Interval = socket_dev.GetInterval(SocketDataPacket[indexA].OutputID);				//写入Interval
				SocketDataPacket[indexA].LaneNo = indexA + 1;													//写入LaneNo
				SocketDataPacket[indexA].DateTime = socket_dev.GetDateTime(SocketDataPacket[indexA].OutputID);				//写入DateTime
				SocketDataPacket[indexA].Volume = socket_dev.GetVolume(SocketDataPacket[indexA].OutputID);				//写入Volume
				SocketDataPacket[indexA].Volume1 = socket_dev.GetVolume1(SocketDataPacket[indexA].OutputID);				//写入Volume1
				SocketDataPacket[indexA].Volume2 = socket_dev.GetVolume2(SocketDataPacket[indexA].OutputID);				//写入Volume2
				SocketDataPacket[indexA].Volume3 = socket_dev.GetVolume3(SocketDataPacket[indexA].OutputID);				//写入Volume3
				SocketDataPacket[indexA].Volume4 = socket_dev.GetVolume4(SocketDataPacket[indexA].OutputID);				//写入Volume4
				SocketDataPacket[indexA].Volume5 = socket_dev.GetVolume5(SocketDataPacket[indexA].OutputID);				//写入Volume5
				SocketDataPacket[indexA].AvgOccupancy = socket_dev.GetAvgOccupancy(SocketDataPacket[indexA].OutputID);		//写入AvgOccupancy
				SocketDataPacket[indexA].AvgHeadTime = socket_dev.GetAvgHeadTime(SocketDataPacket[indexA].OutputID);		//写入AvgHeadTime
				SocketDataPacket[indexA].AvgSpeed = socket_dev.GetAvgSpeed(SocketDataPacket[indexA].OutputID);				//写入AvgSpeed
				SocketDataPacket[indexA].AvgLength = socket_dev.GetAvgLength(SocketDataPacket[indexA].OutputID);			//写入AvgLength
				SocketDataPacket[indexA].Saturation = socket_dev.GetSaturation(SocketDataPacket[indexA].OutputID);			//写入Saturation
				SocketDataPacket[indexA].Density = socket_dev.GetDensity(SocketDataPacket[indexA].OutputID);				//写入Density
				SocketDataPacket[indexA].Pcu = socket_dev.GetPcu(SocketDataPacket[indexA].OutputID);						//写入Pcu
				SocketDataPacket[indexA].AvgQueueLength = socket_dev.GetAvgQueueLength(SocketDataPacket[indexA].OutputID);	//写入AvgQueueLength
			}
			else {																					//辅地磁对应主地磁无配置
				for (indexB = indexA; indexB > SPEEDLANNUMMAX; indexB--) {										//寻找本包与上一包同ID数据
					if (SocketDataPacket[indexA].OutputID == SocketDataPacket[indexB - 1].OutputID) {
						SocketDataPacket[indexA].DeviceType = SocketDataPacket[indexB - 1].DeviceType;						//写入DeviceType
						SocketDataPacket[indexA].Interval = SocketDataPacket[indexB - 1].Interval;							//写入Interval
						SocketDataPacket[indexA].LaneNo = indexA + 1;												//写入LaneNo
						SocketDataPacket[indexA].DateTime = SocketDataPacket[indexB - 1].DateTime;							//写入DateTime
						SocketDataPacket[indexA].Volume = SocketDataPacket[indexB - 1].Volume;							//写入Volume
						SocketDataPacket[indexA].Volume1 = SocketDataPacket[indexB - 1].Volume1;							//写入Volume1
						SocketDataPacket[indexA].Volume2 = SocketDataPacket[indexB - 1].Volume2;							//写入Volume2
						SocketDataPacket[indexA].Volume3 = SocketDataPacket[indexB - 1].Volume3;							//写入Volume3
						SocketDataPacket[indexA].Volume4 = SocketDataPacket[indexB - 1].Volume4;							//写入Volume4
						SocketDataPacket[indexA].Volume5 = SocketDataPacket[indexB - 1].Volume5;							//写入Volume5
						SocketDataPacket[indexA].AvgOccupancy = SocketDataPacket[indexB - 1].AvgOccupancy;					//写入AvgOccupancy
						SocketDataPacket[indexA].AvgHeadTime = SocketDataPacket[indexB - 1].AvgHeadTime;					//写入AvgHeadTime
						SocketDataPacket[indexA].AvgSpeed = SocketDataPacket[indexB - 1].AvgSpeed;							//写入AvgSpeed
						SocketDataPacket[indexA].AvgLength = SocketDataPacket[indexB - 1].AvgLength;						//写入AvgLength
						SocketDataPacket[indexA].Saturation = SocketDataPacket[indexB - 1].Saturation;						//写入Saturation
						SocketDataPacket[indexA].Density = SocketDataPacket[indexB - 1].Density;							//写入Density
						SocketDataPacket[indexA].Pcu = SocketDataPacket[indexB - 1].Pcu;									//写入Pcu
						SocketDataPacket[indexA].AvgQueueLength = SocketDataPacket[indexB - 1].AvgQueueLength;				//写入AvgQueueLength
						break;
					}
				}
				if (indexB == SPEEDLANNUMMAX) {															//本数据包无同ID数据包,获取数据
					SocketDataPacket[indexA].DeviceType = socket_dev.GetDeviceType(SocketDataPacket[indexA].OutputID);			//写入DeviceType
					SocketDataPacket[indexA].Interval = socket_dev.GetInterval(SocketDataPacket[indexA].OutputID);				//写入Interval
					SocketDataPacket[indexA].LaneNo = indexA + 1;													//写入LaneNo
					SocketDataPacket[indexA].DateTime = socket_dev.GetDateTime(SocketDataPacket[indexA].OutputID);				//写入DateTime
					SocketDataPacket[indexA].Volume = socket_dev.GetVolume(SocketDataPacket[indexA].OutputID);				//写入Volume
					SocketDataPacket[indexA].Volume1 = socket_dev.GetVolume1(SocketDataPacket[indexA].OutputID);				//写入Volume1
					SocketDataPacket[indexA].Volume2 = socket_dev.GetVolume2(SocketDataPacket[indexA].OutputID);				//写入Volume2
					SocketDataPacket[indexA].Volume3 = socket_dev.GetVolume3(SocketDataPacket[indexA].OutputID);				//写入Volume3
					SocketDataPacket[indexA].Volume4 = socket_dev.GetVolume4(SocketDataPacket[indexA].OutputID);				//写入Volume4
					SocketDataPacket[indexA].Volume5 = socket_dev.GetVolume5(SocketDataPacket[indexA].OutputID);				//写入Volume5
					SocketDataPacket[indexA].AvgOccupancy = socket_dev.GetAvgOccupancy(SocketDataPacket[indexA].OutputID);		//写入AvgOccupancy
					SocketDataPacket[indexA].AvgHeadTime = socket_dev.GetAvgHeadTime(SocketDataPacket[indexA].OutputID);		//写入AvgHeadTime
					SocketDataPacket[indexA].AvgSpeed = socket_dev.GetAvgSpeed(SocketDataPacket[indexA].OutputID);				//写入AvgSpeed
					SocketDataPacket[indexA].AvgLength = socket_dev.GetAvgLength(SocketDataPacket[indexA].OutputID);			//写入AvgLength
					SocketDataPacket[indexA].Saturation = socket_dev.GetSaturation(SocketDataPacket[indexA].OutputID);			//写入Saturation
					SocketDataPacket[indexA].Density = socket_dev.GetDensity(SocketDataPacket[indexA].OutputID);				//写入Density
					SocketDataPacket[indexA].Pcu = socket_dev.GetPcu(SocketDataPacket[indexA].OutputID);						//写入Pcu
					SocketDataPacket[indexA].AvgQueueLength = socket_dev.GetAvgQueueLength(SocketDataPacket[indexA].OutputID);	//写入AvgQueueLength
				}
			}
		}
	}
	else {
		/* 单地磁 */
		for (indexB = indexA; indexB > 0; indexB--) {														//寻找本包与上一包同ID数据
			if (SocketDataPacket[indexA].OutputID == SocketDataPacket[indexB - 1].OutputID) {
				SocketDataPacket[indexA].DeviceType = SocketDataPacket[indexB - 1].DeviceType;						//写入DeviceType
				SocketDataPacket[indexA].Interval = SocketDataPacket[indexB - 1].Interval;							//写入Interval
				SocketDataPacket[indexA].LaneNo = indexA + 1;												//写入LaneNo
				SocketDataPacket[indexA].DateTime = SocketDataPacket[indexB - 1].DateTime;							//写入DateTime
				SocketDataPacket[indexA].Volume = SocketDataPacket[indexB - 1].Volume;							//写入Volume
				SocketDataPacket[indexA].Volume1 = SocketDataPacket[indexB - 1].Volume1;							//写入Volume1
				SocketDataPacket[indexA].Volume2 = SocketDataPacket[indexB - 1].Volume2;							//写入Volume2
				SocketDataPacket[indexA].Volume3 = SocketDataPacket[indexB - 1].Volume3;							//写入Volume3
				SocketDataPacket[indexA].Volume4 = SocketDataPacket[indexB - 1].Volume4;							//写入Volume4
				SocketDataPacket[indexA].Volume5 = SocketDataPacket[indexB - 1].Volume5;							//写入Volume5
				SocketDataPacket[indexA].AvgOccupancy = SocketDataPacket[indexB - 1].AvgOccupancy;					//写入AvgOccupancy
				SocketDataPacket[indexA].AvgHeadTime = SocketDataPacket[indexB - 1].AvgHeadTime;					//写入AvgHeadTime
				SocketDataPacket[indexA].AvgSpeed = SocketDataPacket[indexB - 1].AvgSpeed;							//写入AvgSpeed
				SocketDataPacket[indexA].AvgLength = SocketDataPacket[indexB - 1].AvgLength;						//写入AvgLength
				SocketDataPacket[indexA].Saturation = SocketDataPacket[indexB - 1].Saturation;						//写入Saturation
				SocketDataPacket[indexA].Density = SocketDataPacket[indexB - 1].Density;							//写入Density
				SocketDataPacket[indexA].Pcu = SocketDataPacket[indexB - 1].Pcu;									//写入Pcu
				SocketDataPacket[indexA].AvgQueueLength = SocketDataPacket[indexB - 1].AvgQueueLength;				//写入AvgQueueLength
				break;
			}
		}
		if (indexB == 0) {																				//本数据包无同ID数据包,获取数据
			SocketDataPacket[indexA].DeviceType = socket_dev.GetDeviceType(SocketDataPacket[indexA].OutputID);			//写入DeviceType
			SocketDataPacket[indexA].Interval = socket_dev.GetInterval(SocketDataPacket[indexA].OutputID);				//写入Interval
			SocketDataPacket[indexA].LaneNo = indexA + 1;													//写入LaneNo
			SocketDataPacket[indexA].DateTime = socket_dev.GetDateTime(SocketDataPacket[indexA].OutputID);				//写入DateTime
			SocketDataPacket[indexA].Volume = socket_dev.GetVolume(SocketDataPacket[indexA].OutputID);				//写入Volume
			SocketDataPacket[indexA].Volume1 = socket_dev.GetVolume1(SocketDataPacket[indexA].OutputID);				//写入Volume1
			SocketDataPacket[indexA].Volume2 = socket_dev.GetVolume2(SocketDataPacket[indexA].OutputID);				//写入Volume2
			SocketDataPacket[indexA].Volume3 = socket_dev.GetVolume3(SocketDataPacket[indexA].OutputID);				//写入Volume3
			SocketDataPacket[indexA].Volume4 = socket_dev.GetVolume4(SocketDataPacket[indexA].OutputID);				//写入Volume4
			SocketDataPacket[indexA].Volume5 = socket_dev.GetVolume5(SocketDataPacket[indexA].OutputID);				//写入Volume5
			SocketDataPacket[indexA].AvgOccupancy = socket_dev.GetAvgOccupancy(SocketDataPacket[indexA].OutputID);		//写入AvgOccupancy
			SocketDataPacket[indexA].AvgHeadTime = socket_dev.GetAvgHeadTime(SocketDataPacket[indexA].OutputID);		//写入AvgHeadTime
			if (CarInorOut == 1) {																		//车检入
				SocketDataPacket[indexA].AvgSpeed = 0;														//未离开
			}
			else {																					//车检出
				SocketDataPacket[indexA].AvgSpeed = 3600.0 * AVGLENGTHSINGLE / SocketDataPacket[indexA].AvgOccupancy;
			}
			SocketDataPacket[indexA].AvgLength = socket_dev.GetAvgLength(SocketDataPacket[indexA].OutputID);			//写入AvgLength
			SocketDataPacket[indexA].Saturation = socket_dev.GetSaturation(SocketDataPacket[indexA].OutputID);			//写入Saturation
			SocketDataPacket[indexA].Density = socket_dev.GetDensity(SocketDataPacket[indexA].OutputID);				//写入Density
			SocketDataPacket[indexA].Pcu = socket_dev.GetPcu(SocketDataPacket[indexA].OutputID);						//写入Pcu
			SocketDataPacket[indexA].AvgQueueLength = socket_dev.GetAvgQueueLength(SocketDataPacket[indexA].OutputID);	//写入AvgQueueLength
		}
	}
	
	/* 写入数据 */
	SocketParkHeadPacket.CrossID = socket_dev.GetCrossID();												//写入CrossID
//	SocketParkHeadPacket.PacketType = PACKETTYPE_FLOWMESSAGE;												//流量数据
	SocketParkHeadPacket.PacketType = PACKETTYPE_PARKINGLOTDATA;											//停车场数据
	SocketParkHeadPacket.PacketInfo = 1;																//数据包数
	
	SocketParkDataPacket.DeviceType = DEVICETYPE_DICI;													//写入DeviceType
	SocketParkDataPacket.Interval = INTERVALTIME;														//写入Interval
	SocketParkDataPacket.LaneNo = laneNo + 1;															//写入LaneNo
	SocketParkDataPacket.DateTime = RTC_GetCounter();														//写入DateTime
	SocketParkDataPacket.Volume = CarNum;																//写入Volume
	SocketParkDataPacket.Volume1 = 0;																	//写入Volume1
	SocketParkDataPacket.Volume2 = 0;																	//写入Volume2
	SocketParkDataPacket.Volume3 = 0;																	//写入Volume3
	SocketParkDataPacket.Volume4 = 0;																	//写入Volume4
	SocketParkDataPacket.Volume5 = 0;																	//写入Volume5
	SocketParkDataPacket.AvgOccupancy = SocketDataPacket[laneNo].AvgOccupancy;									//写入AvgOccupancy
	SocketParkDataPacket.AvgHeadTime = SocketDataPacket[laneNo].AvgHeadTime;									//写入AvgHeadTime
	SocketParkDataPacket.AvgSpeed = SocketDataPacket[laneNo].AvgSpeed;										//写入AvgSpeed
	SocketParkDataPacket.AvgLength = SocketDataPacket[laneNo].AvgLength;										//写入AvgLength
	SocketParkDataPacket.Saturation = SocketDataPacket[laneNo].Saturation;									//写入Saturation
	SocketParkDataPacket.Density = SocketDataPacket[laneNo].Density;											//写入Density
	SocketParkDataPacket.Pcu = SocketDataPacket[laneNo].Pcu;												//写入Pcu
	SocketParkDataPacket.AvgQueueLength = SocketDataPacket[laneNo].AvgQueueLength;								//写入AvgQueueLength
	
	headlength = SOCKET_ParkObtainPacketHead((u8 *)SocketSendBuf);											//读取SocketPark包头数据并填入缓存
	datalength = SOCKET_ParkObtainPacketData((u8 *)SocketSendBuf);											//读取SocketPark停车场数据包并填入缓存
	headlength = headlength + datalength;
	datalength = socket_dev.ObtainPacketManuCheck((u8 *)SocketSendBuf, headlength);
	headlength = headlength + datalength;
	
	SOCKET_USARTSend(SOCKET_USART, (u8 *)SocketSendBuf, headlength);											//发送数据
}

/**********************************************************************************************************
 @Function			void SOCKET_ParkImplementHeartbeatSend(u8 laneNo, u16 CarNum)
 @Description			Socket数据心跳上传
 @Input				laneNo : 车道号
					CarNum : 车辆值
 @Return				void
**********************************************************************************************************/
void SOCKET_ParkImplementHeartbeatSend(u8 laneNo, u16 CarNum)
{
	u16 headlength = 0;
	u16 datalength = 0;
	
	/* 写入数据 */
	SocketParkHeadPacket.CrossID = socket_dev.GetCrossID();					//写入CrossID
	SocketParkHeadPacket.PacketType = PACKETTYPE_PARKINGLOTDATA;				//停车场数据
	SocketParkHeadPacket.PacketInfo = 1;									//数据包数
	
	SocketParkDataPacket.DeviceType = DEVICETYPE_DICI;						//写入DeviceType
	SocketParkDataPacket.Interval = INTERVALTIME;							//写入Interval
	SocketParkDataPacket.LaneNo = laneNo + 1;								//写入LaneNo
	SocketParkDataPacket.DateTime = RTC_GetCounter();							//写入DateTime
	SocketParkDataPacket.Volume = CarNum;									//写入Volume
	SocketParkDataPacket.Volume1 = 0;										//写入Volume1
	SocketParkDataPacket.Volume2 = 0;										//写入Volume2
	SocketParkDataPacket.Volume3 = 0;										//写入Volume3
	SocketParkDataPacket.Volume4 = 0;										//写入Volume4
	SocketParkDataPacket.Volume5 = 0;										//写入Volume5
	SocketParkDataPacket.AvgOccupancy = 0;									//写入AvgOccupancy
	SocketParkDataPacket.AvgHeadTime = 0;									//写入AvgHeadTime
	SocketParkDataPacket.AvgLength = 0;									//写入AvgLength
	SocketParkDataPacket.AvgSpeed = 0;										//写入AvgSpeed
	SocketParkDataPacket.Saturation = 0;									//写入Saturation
	SocketParkDataPacket.Density = 0;										//写入Density
	SocketParkDataPacket.Pcu = 0;											//写入Pcu
	SocketParkDataPacket.AvgQueueLength = 0;								//写入AvgQueueLength
	
	headlength = SOCKET_ParkObtainPacketHead((u8 *)SocketSendBuf);				//读取SocketPark包头数据并填入缓存
	datalength = SOCKET_ParkObtainPacketData((u8 *)SocketSendBuf);				//读取SocketPark停车场数据包并填入缓存
	headlength = headlength + datalength;
	datalength = socket_dev.ObtainPacketManuCheck((u8 *)SocketSendBuf, headlength);
	headlength = headlength + datalength;
	
	SOCKET_USARTSend(SOCKET_USART, (u8 *)SocketSendBuf, headlength);				//发送数据
}

/**********************************************************************************************************
 @Function			void SOCKET_ParkImplementHeartbeat(u8 laneNo, u16 CarNum)
 @Description			Socket数据实时上传(心跳包上传数据)
 @Input				*buf				: 网关接收到地磁发送的数据包
 @Return				void
**********************************************************************************************************/
void SOCKET_ParkImplementHeartbeat(u8 *buf)
{
	u8 i = 0;
	u16 carnumstate = 0;
	RF_DataHeader_TypeDef *phead = (RF_DataHeader_TypeDef *)buf;
	
	//如果收到地磁心跳包
	if ( (phead->type == DATATYPE_HEARTBEAT_WITHOUT_MAGENV) || (phead->type == DATATYPE_HEARTBEAT_WITH_MAGENV) )
	{
		carnumstate = (((~0xffff) | buf[3])<<8) | buf[4];						//获取车辆数状态值
		for (i = 0; i < OUTPUT_MAX; i++) {									//遍历ID
			if ((output_ID[i] == (phead->addr_dev)) || (output_ID[i] == 0xFFFF))	//匹配同ID
			{
				SOCKET_ParkImplementHeartbeatSend(i, carnumstate);			//心跳数据包发送
			}
		}
	}
}

/**********************************************************************************************************
 @Function			u16 SOCKET_ObtainPacketHead(u8 *addr)
 @Description			读取SocketPark包头数据并填入缓存
 @Input				*addr		: Socket缓存地址
 @Return				填入缓存数据长度
					SOCKET_FALSE   : 出错
**********************************************************************************************************/
u16 SOCKET_ParkObtainPacketHead(u8 *addr)
{
	u16 headlength = 0;																				//填入缓存数据长度
	
	if ((SOCKET_ConvertCrossID((u8 *)&addr[headlength], SocketParkHeadPacket.CrossID)) == SOCKET_FALSE) {			//填入CrossID
		return SOCKET_FALSE;
	}
	else {
		headlength += 8;																			//地址偏移8byte
	}
	
	addr[headlength] = SocketParkHeadPacket.PacketType;													//填入流水包类型
	headlength += 1;																				//地址偏移1byte
	
	if ((SOCKET_ConvertPacketInfo((u8 *)&addr[headlength], SocketParkHeadPacket.PacketInfo)) == SOCKET_FALSE) {		//填入输出流量数据包数
		return SOCKET_FALSE;
	}
	else {
		headlength += 4;																			//地址偏移4byte
	}
	
	return headlength;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_ObtainPacketData(u8 *addr)
 @Description			读取SocketPark停车场数据包并填入缓存
 @Input				*addr		: Socket缓存地址
 @Return				填入缓存数据长度
					SOCKET_FALSE   : 出错
**********************************************************************************************************/
u16 SOCKET_ParkObtainPacketData(u8 *addr)
{
	u16 datalength = 0;																				//填入缓存数据长度
	
	datalength += 13;																				//地址偏移出数据包头
	
	addr[datalength] = SocketParkDataPacket.DeviceType;													//填入设备编号
	datalength += 1;																			//地址偏移1byte
	
	if ((SOCKET_ConvertInterval((u8 *)&addr[datalength], SocketParkDataPacket.Interval)) == SOCKET_FALSE) {			//填入统计时长
		return SOCKET_FALSE;
	}
	else {
		datalength += 2;																		//地址偏移2byte
	}
	
	if ((SOCKET_ConvertLaneNo((u8 *)&addr[datalength], SocketParkDataPacket.LaneNo)) == SOCKET_FALSE) {				//填入车道号
		return SOCKET_FALSE;
	}
	else {
		datalength += 3;																		//地址偏移3byte
	}
	
	if ((SOCKET_ConvertDateTime((u8 *)&addr[datalength], SocketParkDataPacket.DateTime)) == SOCKET_FALSE) {			//填入流水发生时间
		return SOCKET_FALSE;
	}
	else {
		datalength += 20;																		//地址偏移20byte
	}
	
	if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketParkDataPacket.Volume)) == SOCKET_FALSE) {				//填入一分通过车辆
		return SOCKET_FALSE;
	}
	else {
		datalength += 2;																		//地址偏移2byte
	}
	
	if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketParkDataPacket.Volume1)) == SOCKET_FALSE) {				//填入微小车辆数量
		return SOCKET_FALSE;
	}
	else {
		datalength += 2;																		//地址偏移2byte
	}
	
	if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketParkDataPacket.Volume2)) == SOCKET_FALSE) {				//填入小车数量
		return SOCKET_FALSE;
	}
	else {
		datalength += 2;																		//地址偏移2byte
	}
	
	if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketParkDataPacket.Volume3)) == SOCKET_FALSE) {				//填入中车数量
		return SOCKET_FALSE;
	}
	else {
		datalength += 2;																		//地址偏移2byte
	}
	
	if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketParkDataPacket.Volume4)) == SOCKET_FALSE) {				//填入大车数量
		return SOCKET_FALSE;
	}
	else {
		datalength += 2;																		//地址偏移2byte
	}
	
	if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketParkDataPacket.Volume5)) == SOCKET_FALSE) {				//填入超大车数量
		return SOCKET_FALSE;
	}
	else {
		datalength += 2;																		//地址偏移2byte
	}
	
	if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketParkDataPacket.AvgOccupancy)) == SOCKET_FALSE) {			//填入平均占有时间
		return SOCKET_FALSE;
	}
	else {
		datalength += 2;																		//地址偏移2byte
	}
	
	if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketParkDataPacket.AvgHeadTime)) == SOCKET_FALSE) {			//填入平均车头时距
		return SOCKET_FALSE;
	}
	else {
		datalength += 2;																		//地址偏移2byte
	}
	
	if ((SOCKET_Convert4Byte((u8 *)&addr[datalength], SocketParkDataPacket.AvgLength)) == SOCKET_FALSE) {			//填入平均车长
		return SOCKET_FALSE;
	}
	else {
		datalength += 4;																		//地址偏移4byte
	}
	
	if ((SOCKET_Convert4Byte((u8 *)&addr[datalength], SocketParkDataPacket.AvgSpeed)) == SOCKET_FALSE) {			//填入平均速度
		return SOCKET_FALSE;
	}
	else {
		datalength += 4;																		//地址偏移4byte
	}
	
	addr[datalength] = SocketParkDataPacket.Saturation;													//填入饱和度
	datalength += 1;																			//地址偏移1byte
	
	if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketParkDataPacket.Density)) == SOCKET_FALSE) {				//填入密度
		return SOCKET_FALSE;
	}
	else {
		datalength += 2;																		//地址偏移2byte
	}
	
	if ((SOCKET_Convert2Byte((u8 *)&addr[datalength], SocketParkDataPacket.Pcu)) == SOCKET_FALSE) {				//填入当量小汽车
		return SOCKET_FALSE;
	}
	else {
		datalength += 2;																		//地址偏移2byte
	}
	
	if ((SOCKET_Convert4Byte((u8 *)&addr[datalength], SocketParkDataPacket.AvgQueueLength)) == SOCKET_FALSE) {		//填入排队长度
		return SOCKET_FALSE;
	}
	else {
		datalength += 4;																		//地址偏移4byte
	}
	
	datalength -= 13;																			//减去数据包头13byte
	
	return datalength;
}

/********************************************** END OF FLEE **********************************************/
