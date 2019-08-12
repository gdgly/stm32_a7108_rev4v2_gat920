/**
  *********************************************************************************************************
  * @file    socketextendinstantia.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   socketXunfei各个数据例化接口
  *********************************************************************************************************
  * @attention
  *			
  *
  *
  *********************************************************************************************************
  */

#include "socketxunfeiinstantia.h"
#include "socketxunfeifunc.h"
#include "socketxunfeiinitialization.h"
#include "calculationconfig.h"
#include "calculationavgspeed.h"
#include "rtc.h"
#include "string.h"
#include "stdio.h"

/**********************************************************************************************************
 @Function			void SOCKET_Xunfei_GetFillData(SOCKET_Xunfei_ClientsTypeDef* pClient)
 @Description			SOCKET_Xunfei_GetFillData			: 获取各个车道数据
 @Input				pClient							: Socket Xunfei客户端实例
 @Return				void
**********************************************************************************************************/
void SOCKET_Xunfei_GetFillData(SOCKET_Xunfei_ClientsTypeDef* pClient)
{
	unsigned char indexA = 0;
	unsigned char indexB = 0;
	
	for (indexA = 0; indexA < OUTPUT_MAX; indexA++) {
		if (pClient->OutputID[indexA] != 0) {
			if (indexA < SPEEDLANNUMMAX) {
				/* 主地磁数据获取 */
				for (indexB = indexA; indexB > 0; indexB--) {
					if (pClient->OutputID[indexA] == pClient->OutputID[indexB - 1]) {
						pClient->Parameter[indexA].DirverWayCode = SOCKET_Xunfei_GetDirverWayCode(pClient, pClient->OutputID[indexA], indexA);
						SOCKET_Xunfei_GetDateTime(pClient, pClient->OutputID[indexA], indexA);
						pClient->Parameter[indexA].Interval = pClient->Parameter[indexB - 1].Interval;
						pClient->Parameter[indexA].Volume = pClient->Parameter[indexB - 1].Volume;
						pClient->Parameter[indexA].AvgOccupancy = pClient->Parameter[indexB - 1].AvgOccupancy;
						pClient->Parameter[indexA].AvgHeadTime = pClient->Parameter[indexB - 1].AvgHeadTime;
						pClient->Parameter[indexA].AvgSpeed = pClient->Parameter[indexB - 1].AvgSpeed;
						pClient->Parameter[indexA].AvgLength = pClient->Parameter[indexB - 1].AvgLength;
						break;
					}
				}
				if (indexB == 0) {
					pClient->Parameter[indexA].DirverWayCode = SOCKET_Xunfei_GetDirverWayCode(pClient, pClient->OutputID[indexA], indexA);
					SOCKET_Xunfei_GetDateTime(pClient, pClient->OutputID[indexA], indexA);
					pClient->Parameter[indexA].Interval = SOCKET_Xunfei_GetInterval(pClient, pClient->OutputID[indexA], indexA);
					pClient->Parameter[indexA].Volume = SOCKET_Xunfei_GetVolume(pClient, pClient->OutputID[indexA], indexA);
					pClient->Parameter[indexA].AvgOccupancy = SOCKET_Xunfei_GetAvgOccupancy(pClient, pClient->OutputID[indexA], indexA);
					pClient->Parameter[indexA].AvgHeadTime = SOCKET_Xunfei_GetAvgHeadTime(pClient, pClient->OutputID[indexA], indexA);
					pClient->Parameter[indexA].AvgSpeed = SOCKET_Xunfei_GetAvgSpeed(pClient, pClient->OutputID[indexA], indexA);
					pClient->Parameter[indexA].AvgLength = SOCKET_Xunfei_GetAvgLength(pClient, pClient->OutputID[indexA], indexA);
				}
			}
			else {
				/* 辅地磁数据获取 */
				if (pClient->OutputID[indexA - SPEEDLANNUMMAX] != 0) {
					pClient->Parameter[indexA].DirverWayCode = SOCKET_Xunfei_GetDirverWayCode(pClient, pClient->OutputID[indexA], indexA);
					SOCKET_Xunfei_GetDateTime(pClient, pClient->OutputID[indexA], indexA);
					pClient->Parameter[indexA].Interval = pClient->Parameter[indexB - SPEEDLANNUMMAX].Interval;
					pClient->Parameter[indexA].Volume = pClient->Parameter[indexB - SPEEDLANNUMMAX].Volume;
					pClient->Parameter[indexA].AvgOccupancy = pClient->Parameter[indexB - SPEEDLANNUMMAX].AvgOccupancy;
					pClient->Parameter[indexA].AvgHeadTime = pClient->Parameter[indexB - SPEEDLANNUMMAX].AvgHeadTime;
					pClient->Parameter[indexA].AvgSpeed = pClient->Parameter[indexB - SPEEDLANNUMMAX].AvgSpeed;
					pClient->Parameter[indexA].AvgLength = pClient->Parameter[indexB - SPEEDLANNUMMAX].AvgLength;
				}
				else {
					for (indexB = indexA; indexB > SPEEDLANNUMMAX; indexB--) {
						if (pClient->OutputID[indexA] == pClient->OutputID[indexB - 1]) {
							pClient->Parameter[indexA].DirverWayCode = SOCKET_Xunfei_GetDirverWayCode(pClient, pClient->OutputID[indexA], indexA);
							SOCKET_Xunfei_GetDateTime(pClient, pClient->OutputID[indexA], indexA);
							pClient->Parameter[indexA].Interval = pClient->Parameter[indexB - 1].Interval;
							pClient->Parameter[indexA].Volume = pClient->Parameter[indexB - 1].Volume;
							pClient->Parameter[indexA].AvgOccupancy = pClient->Parameter[indexB - 1].AvgOccupancy;
							pClient->Parameter[indexA].AvgHeadTime = pClient->Parameter[indexB - 1].AvgHeadTime;
							pClient->Parameter[indexA].AvgSpeed = pClient->Parameter[indexB - 1].AvgSpeed;
							pClient->Parameter[indexA].AvgLength = pClient->Parameter[indexB - 1].AvgLength;
							break;
						}
					}
					if (indexB == SPEEDLANNUMMAX) {
						pClient->Parameter[indexA].DirverWayCode = SOCKET_Xunfei_GetDirverWayCode(pClient, pClient->OutputID[indexA], indexA);
						SOCKET_Xunfei_GetDateTime(pClient, pClient->OutputID[indexA], indexA);
						pClient->Parameter[indexA].Interval = SOCKET_Xunfei_GetInterval(pClient, pClient->OutputID[indexA], indexA);
						pClient->Parameter[indexA].Volume = SOCKET_Xunfei_GetVolume(pClient, pClient->OutputID[indexA], indexA);
						pClient->Parameter[indexA].AvgOccupancy = SOCKET_Xunfei_GetAvgOccupancy(pClient, pClient->OutputID[indexA], indexA);
						pClient->Parameter[indexA].AvgHeadTime = SOCKET_Xunfei_GetAvgHeadTime(pClient, pClient->OutputID[indexA], indexA);
						pClient->Parameter[indexA].AvgSpeed = SOCKET_Xunfei_GetAvgSpeed(pClient, pClient->OutputID[indexA], indexA);
						pClient->Parameter[indexA].AvgLength = SOCKET_Xunfei_GetAvgLength(pClient, pClient->OutputID[indexA], indexA);
					}
				}
			}
		}
	}
}

/**********************************************************************************************************
 @Function			int SOCKET_Xunfei_GetDirverWayCode(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen)
 @Description			SOCKET_Xunfei_GetDirverWayCode		: 获取车道编号
 @Input				pClient							: Socket Xunfei客户端实例
					outputid							: 地磁ID
					inlen							: 路口ID
 @Return				DirverWayCode						: 车道代码
**********************************************************************************************************/
int SOCKET_Xunfei_GetDirverWayCode(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen)
{
	u8 i = 0;
	u16 laneno = 101;
	
	u8 westNum  = (param_recv.dirverway_config & 0x000F) >>  0;
	u8 southNum = (param_recv.dirverway_config & 0x00F0) >>  4;
	u8 eastNum  = (param_recv.dirverway_config & 0x0F00) >>  8;
	u8 northNum = (param_recv.dirverway_config & 0xF000) >> 12;
	
	if ((westNum + southNum + eastNum + northNum) > 16) {
		westNum  = 4;
		southNum = 4;
		eastNum  = 4;
		northNum = 4;
	}
	
	for (i = 0; i < OUTPUT_MAX; i++) {
		if (pClient->OutputID[i] == outputid) {
			i = inlen;
			if (i < westNum) {
				laneno = i + 1 + 100;
				break;
			}
			else if ((i >= westNum) && (i < (westNum + southNum))) {
				laneno = i + 1 + 200 - westNum;
				break;
			}
			else if ((i >= (westNum + southNum)) && (i < (westNum + southNum + eastNum))) {
				laneno = i + 1 + 300 - westNum - southNum;
				break;
			}
			else if ((i >= (westNum + southNum + eastNum)) && (i < (westNum + southNum + eastNum + northNum))) {
				laneno = i + 1 + 400 - westNum - southNum - eastNum;
				break;
			}
			else if ((i >= 16) && (i < (16 + westNum))) {
				laneno = i + 1 + 100 - (16 - westNum);
				break;
			}
			else if ((i >= (16 + westNum)) && (i < (16 + westNum + southNum))) {
				laneno = i + 1 + 200 - (16 + westNum - southNum);
				break;
			}
			else if ((i >= (16 + westNum + southNum)) && (i < (16 + westNum + southNum + eastNum))) {
				laneno = i + 1 + 300 - (16 + westNum + southNum - eastNum);
				break;
			}
			else if ((i >= (16 + westNum + southNum + eastNum)) && (i < (16 + westNum + southNum + eastNum + northNum))) {
				laneno = i + 1 + 400 - (16 + westNum + southNum + eastNum - northNum);
				break;
			}
		}
	}
	
	return laneno;
}

/**********************************************************************************************************
 @Function			char* SOCKET_Xunfei_GetDateTime(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen)
 @Description			SOCKET_Xunfei_GetDateTime			: 获取统计时间
 @Input				pClient							: Socket Xunfei客户端实例
					outputid							: 地磁ID
					inlen							: 路口ID
 @Return				DateTime
**********************************************************************************************************/
char* SOCKET_Xunfei_GetDateTime(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen)
{
	memset((void*)pClient->Parameter[inlen].DateTime, 0x00, sizeof(pClient->Parameter[inlen].DateTime));
	
	u32 datetime = 0;
	
	datetime = RTC_GetCounter();
	
	to_tm(datetime, &systemtime);
	
	sprintf(pClient->Parameter[inlen].DateTime, "%04d%02d%02d%02d%02d%02d", systemtime.tm_year, systemtime.tm_mon, systemtime.tm_mday, systemtime.tm_hour, systemtime.tm_min, systemtime.tm_sec);
	
	return pClient->Parameter[inlen].DateTime;
}

/**********************************************************************************************************
 @Function			int SOCKET_Xunfei_GetInterval(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen)
 @Description			SOCKET_Xunfei_GetInterval			: 获取统计周期
 @Input				pClient							: Socket Xunfei客户端实例
					outputid							: 地磁ID
					inlen							: 路口ID
 @Return				Interval							: 统计时间
**********************************************************************************************************/
int SOCKET_Xunfei_GetInterval(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen)
{
	return INTERVALTIME;
}

/**********************************************************************************************************
 @Function			int SOCKET_Xunfei_GetVolume(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen)
 @Description			SOCKET_Xunfei_GetVolume				: 获取统计周期内通过车辆总数
 @Input				pClient							: Socket Xunfei客户端实例
					outputid							: 地磁ID
					inlen							: 路口ID
 @Return				Volume							: 车辆数
**********************************************************************************************************/
int SOCKET_Xunfei_GetVolume(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen)
{
	u16 volumeval = 0;
	
	volumeval = calculation_dev.ReadVolume(outputid);
	
	return volumeval;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Xunfei_GetAvgOccupancy(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen)
 @Description			SOCKET_Xunfei_GetAvgOccupancy			: 获取平均占有时间
 @Input				pClient							: Socket Xunfei客户端实例
					outputid							: 地磁ID
					inlen							: 路口ID
 @Return				AvgOccupancy						: 平均占有时间
**********************************************************************************************************/
u16 SOCKET_Xunfei_GetAvgOccupancy(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen)
{
	u16 avgoccupancyval = 0;
	
	avgoccupancyval = calculation_dev.ReadAvgOccupancy(outputid);
	
	return avgoccupancyval;
}

/**********************************************************************************************************
 @Function			u16 SOCKET_Xunfei_GetAvgHeadTime(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen)
 @Description			SOCKET_Xunfei_GetAvgHeadTime			: 获取平均占有时间
 @Input				pClient							: Socket Xunfei客户端实例
					outputid							: 地磁ID
					inlen							: 路口ID
 @Return				AvgHeadTime						: 平均车头时距
**********************************************************************************************************/
u16 SOCKET_Xunfei_GetAvgHeadTime(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen)
{
	u16 avgheadtimeval = 0;
	
	avgheadtimeval = calculation_dev.ReadAvgHeadTime(outputid);
	
	return avgheadtimeval;
}

/**********************************************************************************************************
 @Function			float SOCKET_Xunfei_GetAvgSpeed(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen)
 @Description			SOCKET_Xunfei_GetAvgSpeed			: 获取平均车速
 @Input				pClient							: Socket Xunfei客户端实例
					outputid							: 地磁ID
					inlen							: 路口ID
 @Return				AvgSpeed							: 平均车速
**********************************************************************************************************/
float SOCKET_Xunfei_GetAvgSpeed(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen)
{
	u8 i = 0;
	u8 outputnum = 0;
	u32 avgoccupancyval = 0;
	float avgspeedval = 0;
	
	for (i = 0; i < OUTPUT_MAX; i++) {
		if (pClient->OutputID[i] != 0) {
			outputnum = i;
		}
	}
	
	if (outputnum >= SPEEDLANNUMMAX) {
		avgspeedval = calculation_dev.ReadAvgSpeed(outputid);
	}
	else {
		for (i = 0; i < OUTPUT_MAX; i++) {
			if (pClient->OutputID[i] == outputid) {
				avgoccupancyval = pClient->Parameter[i].AvgOccupancy;
			}
		}
		if (avgoccupancyval != 0) {
			avgspeedval = 3600.0 * SOCKET_XUNFEI_AVGLENGTHSINGLE / avgoccupancyval;
		}
	}
	
	return avgspeedval;
}

/**********************************************************************************************************
 @Function			float SOCKET_Xunfei_GetAvgLength(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen)
 @Description			SOCKET_Xunfei_GetAvgLength			: 获取平均车长
 @Input				pClient							: Socket Xunfei客户端实例
					outputid							: 地磁ID
					inlen							: 路口ID
 @Return				AvgSpeed							: 平均车长
**********************************************************************************************************/
float SOCKET_Xunfei_GetAvgLength(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen)
{
	u8 i = 0;
	u8 outputnum = 0;
	float avglengthval = 0;
	float avgspeedval = 0;
	u16 avgoccupancyval = 0;
	
	for (i = 0; i < OUTPUT_MAX; i++) {
		if (pClient->OutputID[i] != 0) {
			outputnum = i;
		}
	}
	
	if (outputnum >= SPEEDLANNUMMAX) {
		for (i = 0; i < OUTPUT_MAX; i++) {
			if (pClient->OutputID[i] == outputid) {
				if (i < SPEEDLANNUMMAX) {
					avgspeedval = pClient->Parameter[i].AvgSpeed;
					avgoccupancyval = pClient->Parameter[i].AvgOccupancy;
				}
				else {
#if (INTERVALTIME == 0)
					avgspeedval = pClient->Parameter[i].AvgSpeed;
					avgoccupancyval = pClient->Parameter[i].AvgOccupancy;
#else
					avgspeedval = pClient->Parameter[i - SPEEDLANNUMMAX].AvgSpeed;
					avgoccupancyval = pClient->Parameter[i - SPEEDLANNUMMAX].AvgOccupancy;
#endif
				}
			}
		}
		
		avgspeedval /= 3.6;
		avglengthval = avgspeedval * avgoccupancyval;
		avglengthval /= 1000;
		
		if ((avglengthval < SOCKET_XUNFEI_AVGLENGTHMIN) && (avglengthval > 0.1)) {
			avglengthval = SOCKET_XUNFEI_AVGLENGTHMIN;
		}
	}
	else {
		avglengthval = SOCKET_XUNFEI_AVGLENGTHSINGLE;
	}
	
	return avglengthval;
}

/********************************************** END OF FLEE **********************************************/
