/**
  *********************************************************************************************************
  * @file    calculationconfig.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   �ڲ����ݼ�������
  *********************************************************************************************************
  * @attention
  *			
  *
  *********************************************************************************************************
  */

#include "calculationconfig.h"
#include "calculationvolume.h"
#include "calculationavgspeed.h"
#include "calculationavgoccupancy.h"
#include "calculationavgheadtime.h"
#include "calculationavginterval.h"

Calculation_Data_Packet			CalculationDataPacket[OUTPUT_MAX];				//���ݼ����


/* calculation�ⲿ�ӿ����ݶ�ȡ���������� */
struct _m_calculation_dev calculation_dev = 
{
	CALCULATION_GetOutputID,
	
	CALCULATION_GetVolume,
	CALCULATION_ReadVolume,
	
	CALCULATION_GetAvgSpeed,
	CALCULATION_ReadAvgSpeed,
	
	CALCULATION_GetAvgOccupancy,
	CALCULATION_ReadAvgOccupancy,
	CALCULATION_ReadAvgOccupancyExtend,
	
	CALCULATION_GetAvgHeadTime,
	CALCULATION_ReadAvgHeadTime,
	CALCULATION_ReadAvgHeadTimeExtend,
	
	CALCULATION_GetAvgInterval,
	CALCULATION_ReadAvgInterval,
};

/**********************************************************************************************************
 @Function			void CALCULATION_GetOutputID(u16 *outputid)
 @Description			��ȡoutput_ID����˿ڵĲ�����Calculation�������ݰ�
 @Input				Output_ID��ŵ�ַ
 @Return				void
**********************************************************************************************************/
void CALCULATION_GetOutputID(u16 *outputid)
{
	u8 i = 0;
	
	for (i = 0; i < OUTPUT_MAX; i++) {
		CalculationDataPacket[i].OutputID = outputid[i];
	}
}

/********************************************** END OF FLEE **********************************************/
