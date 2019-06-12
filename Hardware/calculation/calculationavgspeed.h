#ifndef __CALCULATION_AVGSPEED_H
#define   __CALCULATION_AVGSPEED_H

#include "stm32f10x_lib.h"
#include "calculationconfig.h"


#define		SPEEDDISTANCEINTERVAL		3								//���شż������3M
#define		SPEEDOVERTIME				5000								//��ʱ��ֵ5000ms
#define		SPEEDLANNUMMAX				SOCKETLANNUMMAX					//���ó��������16·����


void  CALCULATION_GetAvgSpeed(u8 *buf);										//���ݵشŷ��͵����ݰ�����ƽ���ٶ�
float CALCULATION_ReadAvgSpeed(u16 outputid);								//��ȡ�ó���ƽ���ٶ�ֵ


#endif
