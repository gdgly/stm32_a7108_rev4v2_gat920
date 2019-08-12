#ifndef __CALCULATION_AVGINTERVAL_H
#define   __CALCULATION_AVGINTERVAL_H

#include "stm32f10x_lib.h"
#include "calculationconfig.h"

void CALCULATION_GetAvgInterval(u8 *buf);									//根据地磁发送的数据包计算两地磁间隔接收到数据包时间
u32 CALCULATION_ReadAvgInterval(u16 outputid, u32* sumTime1, u32* sumTime2);		//获取该车道平均间隔时间

#endif
