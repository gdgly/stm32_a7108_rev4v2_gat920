/**
  *********************************************************************************************************
  * @file    calculationavginterval.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   两地磁间隔时间获取
  *********************************************************************************************************
  * @attention
  *			功能:
  *			1. 根据地磁发送的数据包计算两地磁间隔接收到数据包时间						(外部调用)
  *			2. 获取该车道 1入2出 与 2出1入 时间									(外部调用)
  *
  *********************************************************************************************************
  */

#include "calculationavginterval.h"

/**********************************************************************************************************
 @Function			void CALCULATION_GetAvgInterval(u8 *buf)
 @Description			根据地磁发送的数据包计算两地磁间隔接收到数据包时间
 @Input				*buf				: 网关接收到地磁发送的数据包
 @Return				void
**********************************************************************************************************/
void CALCULATION_GetAvgInterval(u8 *buf)
{
	u8 i = 0;
	u16 carnumval = 0;
	RF_DataHeader_TypeDef *phead = (RF_DataHeader_TypeDef *)buf;
	
	if (!(SOCKETLANNUMMAX <= 16)) return;
	
	//如果收到车辆进入车检包
	if ( (phead->type == DATATYPE_CARIN_WITH_MAGVALUE) || (phead->type == DATATYPE_CARIN_WITHOUT_MAGVALUE) ) {
		carnumval = (((~0xffff) | buf[3]) << 8) | buf[4];									//获取地磁发送数据包中的车辆次数
		
		for (i = 0; i < OUTPUT_MAX; i++) {												//遍历ID
			if (CalculationDataPacket[i].OutputID == (phead->addr_dev)) {					//匹配同ID
				if (i < SOCKETLANNUMMAX) {
					// 1号地磁
					if (CalculationDataPacket[i].Interval_CarNumState1 == 0) {
						CalculationDataPacket[i].Interval_CarNumState1 = carnumval;
						CalculationDataPacket[i].Interval_CarState1 = 1;
						CalculationDataPacket[i].Interval_CarUseTime1 = 0;
					}
					else {
						if (CalculationDataPacket[i].Interval_CarState1 == 1) {
							CalculationDataPacket[i].Interval_CarNumState1 = carnumval;
							CalculationDataPacket[i].Interval_CarState1 = 1;
							CalculationDataPacket[i].Interval_CarSumTime1 += CalculationDataPacket[i].Interval_CarUseTime1;
							CalculationDataPacket[i].Interval_CarUseTime1 = 0;
						}
						else {
							CalculationDataPacket[i].Interval_CarNumState1 = carnumval;
							CalculationDataPacket[i].Interval_CarState1 = 1;
							CalculationDataPacket[i].Interval_CarUseTime1 = 0;
						}
					}
				}
				else {
					// 2号地磁
					
					
					
				}
			}
		}
	}
	//如果收到车辆离开车检包
	else if ( (phead->type == DATATYPE_CAROUT_WITH_MAGVALUE) || (phead->type == DATATYPE_CAROUT_WITHOUT_MAGVALUE) ) {
		carnumval = (((~0xffff) | buf[3]) << 8) | buf[4];									//获取地磁发送数据包中的车辆次数
		
		for (i = 0; i < OUTPUT_MAX; i++) {												//遍历ID
			if (CalculationDataPacket[i].OutputID == (phead->addr_dev)) {					//匹配同ID
				if (i < SOCKETLANNUMMAX) {
					// 1号地磁
					if (CalculationDataPacket[i].Interval_CarNumState2 == 0) {
						CalculationDataPacket[i].Interval_CarNumState2 = carnumval;
						CalculationDataPacket[i].Interval_CarState2 = 1;
						CalculationDataPacket[i].Interval_CarUseTime2 = 0;
					}
					else {
						if (CalculationDataPacket[i].Interval_CarState2 == 1) {
							CalculationDataPacket[i].Interval_CarNumState2 = carnumval;
							CalculationDataPacket[i].Interval_CarState2 = 1;
							CalculationDataPacket[i].Interval_CarSumTime2 += CalculationDataPacket[i].Interval_CarUseTime2;
							CalculationDataPacket[i].Interval_CarUseTime2 = 0;
						}
						else {
							CalculationDataPacket[i].Interval_CarNumState2 = carnumval;
							CalculationDataPacket[i].Interval_CarState2 = 1;
							CalculationDataPacket[i].Interval_CarUseTime2 = 0;
						}
					}
				}
				else {
					// 2号地磁
					if (CalculationDataPacket[i - SOCKETLANNUMMAX].Interval_CarState1 == 1) {
						CalculationDataPacket[i - SOCKETLANNUMMAX].Interval_CarState1 = 0;
						CalculationDataPacket[i - SOCKETLANNUMMAX].Interval_CarSumTime1 += CalculationDataPacket[i - SOCKETLANNUMMAX].Interval_CarUseTime1;
						CalculationDataPacket[i - SOCKETLANNUMMAX].Interval_CarUseTime1 = 0;
						CalculationDataPacket[i - SOCKETLANNUMMAX].Interval_CarSumNum1 += 1;
						CalculationDataPacket[i].Interval_CarSumTime1 = CalculationDataPacket[i - SOCKETLANNUMMAX].Interval_CarSumTime1;
						CalculationDataPacket[i].Interval_CarSumNum1 += 1;
					}
					
					if (CalculationDataPacket[i - SOCKETLANNUMMAX].Interval_CarState2 == 1) {
						CalculationDataPacket[i - SOCKETLANNUMMAX].Interval_CarState2 = 0;
						CalculationDataPacket[i - SOCKETLANNUMMAX].Interval_CarSumTime2 += CalculationDataPacket[i - SOCKETLANNUMMAX].Interval_CarUseTime2;
						CalculationDataPacket[i - SOCKETLANNUMMAX].Interval_CarUseTime2 = 0;
						CalculationDataPacket[i - SOCKETLANNUMMAX].Interval_CarSumNum2 += 1;
						CalculationDataPacket[i].Interval_CarSumTime2 = CalculationDataPacket[i - SOCKETLANNUMMAX].Interval_CarSumTime2;
						CalculationDataPacket[i].Interval_CarSumNum2 += 1;
					}
				}
			}
		}
	}
}

/**********************************************************************************************************
 @Function			u32 CALCULATION_ReadAvgInterval(u16 outputid, u32* sumTime1, u32* sumTime2)
 @Description			获取该车道平均间隔时间
 @Input				
 @Return				void
**********************************************************************************************************/
u32 CALCULATION_ReadAvgInterval(u16 outputid, u32* sumTime1, u32* sumTime2)
{
	u8 i = 0;
	u32 intervalsumtime1 = 0;
	u32 intervalsumtime2 = 0;
	u16 intervalsumnum1 = 0;
	u16 intervalsumnum2 = 0;
	
	if (!(SOCKETLANNUMMAX <= 16)) return 0;
	
	for (i = 0; i < OUTPUT_MAX; i++) {
		if (outputid == CalculationDataPacket[i].OutputID) {
			if (i < SOCKETLANNUMMAX) {
				// 1号地磁
				intervalsumtime1 = CalculationDataPacket[i].Interval_CarSumTime1;
				intervalsumtime2 = CalculationDataPacket[i].Interval_CarSumTime2;
				intervalsumnum1 = CalculationDataPacket[i].Interval_CarSumNum1;
				intervalsumnum2 = CalculationDataPacket[i].Interval_CarSumNum2;
				
			}
			else {
				// 2号地磁
				intervalsumtime1 = CalculationDataPacket[i].Interval_CarSumTime1;
				intervalsumtime2 = CalculationDataPacket[i].Interval_CarSumTime2;
				intervalsumnum1 = CalculationDataPacket[i].Interval_CarSumNum1;
				intervalsumnum2 = CalculationDataPacket[i].Interval_CarSumNum2;
				CalculationDataPacket[i].Interval_CarSumTime1 = 0;
				CalculationDataPacket[i].Interval_CarSumTime2 = 0;
				CalculationDataPacket[i].Interval_CarSumNum1 = 0;
				CalculationDataPacket[i].Interval_CarSumNum2 = 0;
				CalculationDataPacket[i - SOCKETLANNUMMAX].Interval_CarSumTime1 = 0;
				CalculationDataPacket[i - SOCKETLANNUMMAX].Interval_CarSumTime2 = 0;
				CalculationDataPacket[i - SOCKETLANNUMMAX].Interval_CarSumNum1 = 0;
				CalculationDataPacket[i - SOCKETLANNUMMAX].Interval_CarSumNum2 = 0;
			}
		}
	}
	
	if (intervalsumnum1 != 0) {
		*sumTime1 = intervalsumtime1 / intervalsumnum1;
	}
	if (intervalsumnum2 != 0) {
		*sumTime2 = intervalsumtime2 / intervalsumnum2;
	}
	
	return 0;
}

/********************************************** END OF FLEE **********************************************/
