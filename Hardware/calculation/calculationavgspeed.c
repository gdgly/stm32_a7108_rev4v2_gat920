/**
  *********************************************************************************************************
  * @file    calculationavgspeed.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   ƽ���ٶȼ���
  *********************************************************************************************************
  * @attention
  *			���� : 
  *			1.  ���ݵشŷ��͵����ݰ�����ƽ���ٶ�								(�ⲿ����)
  *			2.  ��ȡ�ó���ƽ���ٶ�ֵ											(�ⲿ����)
  *
  *********************************************************************************************************
  */

#include "calculationavgspeed.h"


/**********************************************************************************************************
 @Function			void CALCULATION_GetAvgSpeed(u8 *buf)
 @Description			���ݵشŷ��͵����ݰ�����ƽ���ٶ�
 @Input				*buf				: ���ؽ��յ��شŷ��͵����ݰ�
 @Return				void
**********************************************************************************************************/
void CALCULATION_GetAvgSpeed(u8 *buf)
{
	u8 i = 0;
	RF_DataHeader_TypeDef *phead = (RF_DataHeader_TypeDef *)buf;
	
	//����յ��������복���
	if ( (phead->type == DATATYPE_CARIN_WITH_MAGVALUE) || (phead->type == DATATYPE_CARIN_WITHOUT_MAGVALUE) ) {
		for (i = 0; i < OUTPUT_MAX; i++) {															//����ID
			if (CalculationDataPacket[i].OutputID == (phead->addr_dev)) {								//ƥ��ͬID

				if (i < SPEEDLANNUMMAX) {														//A�شų�����
					CalculationDataPacket[i].Speed_CarinState = 1;									//A�شų������־λ��1
					CalculationDataPacket[i].Speed_CarinUseTime = 0;									//A�ش��峵����ʹ��ʱ���ۼ���
				}
				else if ((i >= SPEEDLANNUMMAX) && (i < (2 * SPEEDLANNUMMAX))) {							//B�شų�����
					CalculationDataPacket[i - SPEEDLANNUMMAX].Speed_CarinState = 0;						//A�شų������־λ��0
					//A�شų�����ʹ��ʱ��
					if ( (CalculationDataPacket[i - SPEEDLANNUMMAX].Speed_CarinUseTime != 0) && 			//��ʹ��ʱ��
						(CalculationDataPacket[i - SPEEDLANNUMMAX].Speed_CarinUseTime < SPEEDOVERTIME) ) 	//ʹ��ʱ��С�ڷ�ֵ
					{
						CalculationDataPacket[i - SPEEDLANNUMMAX].Speed_CarSumTime += CalculationDataPacket[i - SPEEDLANNUMMAX].Speed_CarinUseTime;	//A�ش��ۼ�ʱ��
						CalculationDataPacket[i].Speed_CarSumTime += CalculationDataPacket[i - SPEEDLANNUMMAX].Speed_CarinUseTime;				//B�ش��ۼ�ʱ��
						CalculationDataPacket[i - SPEEDLANNUMMAX].Speed_CarNum += 1;													//A�ش��ۼӴ���
						CalculationDataPacket[i].Speed_CarNum += 1;																	//B�ش��ۼӴ���
					}
					CalculationDataPacket[i - SPEEDLANNUMMAX].Speed_CarinUseTime = 0;					//A�ش��峵����ʹ��ʱ���ۼ���
				}

			}
		}
	}
	//����յ������뿪�����
	else if ( (phead->type == DATATYPE_CAROUT_WITH_MAGVALUE) || (phead->type == DATATYPE_CAROUT_WITHOUT_MAGVALUE) ) {
		for (i = 0; i < OUTPUT_MAX; i++) {															//����ID
			if (CalculationDataPacket[i].OutputID == (phead->addr_dev)) {								//ƥ��ͬID

				if (i < SPEEDLANNUMMAX) {														//A�شų����
					CalculationDataPacket[i].Speed_CaroutState = 1;									//A�شų������־λ��1
					CalculationDataPacket[i].Speed_CaroutUseTime = 0;									//A�ش��峵���ʹ��ʱ���ۼ���
				}
				else if ((i >= SPEEDLANNUMMAX) && (i < (2 * SPEEDLANNUMMAX))) {							//B�شų����
					CalculationDataPacket[i - SPEEDLANNUMMAX].Speed_CaroutState = 0;						//A�شų������־λ��0
					//A�شų����ʹ��ʱ��
					if ( (CalculationDataPacket[i - SPEEDLANNUMMAX].Speed_CaroutUseTime != 0) && 			//��ʹ��ʱ��
						(CalculationDataPacket[i - SPEEDLANNUMMAX].Speed_CaroutUseTime < SPEEDOVERTIME) ) 	//ʹ��ʱ��С�ڷ�ֵ
					{
						CalculationDataPacket[i - SPEEDLANNUMMAX].Speed_CarSumTime += CalculationDataPacket[i - SPEEDLANNUMMAX].Speed_CaroutUseTime;	//A�ش��ۼ�ʱ��
						CalculationDataPacket[i].Speed_CarSumTime += CalculationDataPacket[i - SPEEDLANNUMMAX].Speed_CaroutUseTime;				//B�ش��ۼ�ʱ��
						CalculationDataPacket[i - SPEEDLANNUMMAX].Speed_CarNum += 1;													//A�ش��ۼӴ���
						CalculationDataPacket[i].Speed_CarNum += 1;																	//B�ش��ۼӴ���
					}
					CalculationDataPacket[i - SPEEDLANNUMMAX].Speed_CaroutUseTime = 0;					//A�ش��峵����ʹ��ʱ���ۼ���
				}

			}
		}
	}
}

/**********************************************************************************************************
 @Function			float CALCULATION_ReadAvgSpeed(u16 outputid)
 @Description			��ȡ�ó���ƽ���ٶ�ֵ
 @Input				u16   outputid		: 		����ID��
 @Return				float AvgSpeed		: 		�ó���AvgSpeedֵ
**********************************************************************************************************/
float CALCULATION_ReadAvgSpeed(u16 outputid)
{
	u8 i = 0;
	float avgspeedval = 0;
	u32 speedcarsumtime = 0;
	u16 speedcarnum = 0;
	
	for (i = 0; i < OUTPUT_MAX; i++) {
		if (outputid == CalculationDataPacket[i].OutputID) {
			speedcarsumtime = CalculationDataPacket[i].Speed_CarSumTime;
			CalculationDataPacket[i].Speed_CarSumTime = 0;
			speedcarnum = CalculationDataPacket[i].Speed_CarNum;
			CalculationDataPacket[i].Speed_CarNum = 0;
		}
	}
	
	if (speedcarnum != 0) {
		avgspeedval = 1.0 * speedcarsumtime / speedcarnum;
		avgspeedval /= 1000;
		avgspeedval = SPEEDDISTANCEINTERVAL / avgspeedval;
		avgspeedval *= 3.6;
	}
	
	if (avgspeedval >= SPEEDMIN) avgspeedval = SPEEDMIN;
	
	return avgspeedval;
}

/********************************************** END OF FLEE **********************************************/
