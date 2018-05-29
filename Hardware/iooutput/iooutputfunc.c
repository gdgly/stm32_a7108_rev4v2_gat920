/**
  *********************************************************************************************************
  * @file    iooutputfunc.c
  * @author  MoveBroad -- KangYJ
  * @version V1.0
  * @date    
  * @brief   
  *********************************************************************************************************
  * @attention
  *			���� : 
  *					1. IO���У������
  *					2. IO�����ʱ������
  *********************************************************************************************************
  */

#include "iooutputfunc.h"
#include "lestcconfig.h"
#include "lestcfunc.h"

IOOutputTypedef				IOOutputStruct[IOOutputMAX];					//IOOutput���ƽṹ��

extern GPIO_TypeDef* OUTPUT_TYPE[16];
extern u16 OUTPUT_PIN[16];
extern u8 OUTPUT_NUM;

/**********************************************************************************************************
 @Function			void IOOutput_Mode0_Check(u16 dici_index, u16 dici_carnum, u8 dici_in)
 @Description			IO���У������ (�����ʽ0 : ���泵�����)
 @Input				dici_index  	: �ش�ID���
					dici_carnum 	: �õشų�����״ֵ̬
					dici_in	  	: 1.��������
							       0.�����뿪
 @Return				void
**********************************************************************************************************/
void IOOutput_Mode0_Check(u16 dici_index, u16 dici_carnum, u8 dici_in)
{
	u16 carnum = 0;
	
	if (IOOutputStruct[dici_index].CarNumState == 0) {						//��ʼ��ID������
		IOOutputStruct[dici_index].CarNumState = dici_carnum;					//��¼������״ֵ̬
		if (dici_in == 1) {
			if (dici_index < OUTPUT_NUM) {
				GPIO_SetBits(OUTPUT_TYPE[dici_index],  OUTPUT_PIN[dici_index]);
			}
			LestcCarInSetStatus(&LestcPacketData, dici_index);
			IOOutputStruct[dici_index].IOLevel = 1;							//��ƽ��1
			IOOutputStruct[dici_index].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
		}
		else {
			if (dici_index < OUTPUT_NUM) {
				GPIO_ResetBits(OUTPUT_TYPE[dici_index],  OUTPUT_PIN[dici_index]);
			}
			LestcCarOutSetStatus(&LestcPacketData, dici_index);
			IOOutputStruct[dici_index].IOLevel = 0;							//��ƽ��0
			IOOutputStruct[dici_index].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
		}
	}
	else {															//�ѳ�ʼ�����
		if (dici_carnum > IOOutputStruct[dici_index].CarNumState) {				//���㳵����ֵ
			if ((dici_carnum - IOOutputStruct[dici_index].CarNumState) < IOOutputNUM) {
				carnum = dici_carnum - IOOutputStruct[dici_index].CarNumState;
			}
		}
		else if (dici_carnum < IOOutputStruct[dici_index].CarNumState) {
			if ((65535 - IOOutputStruct[dici_index].CarNumState + dici_carnum) < IOOutputNUM) {
				carnum = 65535 - IOOutputStruct[dici_index].CarNumState + dici_carnum;
			}
		}
		IOOutputStruct[dici_index].CarNumState = dici_carnum;					//��¼������״ֵ̬
		
		/* �жϸõش��Ƿ����ڲ������� */
		if (IOOutputStruct[dici_index].SupplyingFlag == 0) {					//��������
			if (carnum == 0) {											//���貹�䳵����
				if (dici_in == 1) {
					if (dici_index < OUTPUT_NUM) {
						GPIO_SetBits(OUTPUT_TYPE[dici_index],  OUTPUT_PIN[dici_index]);
					}
					LestcCarInSetStatus(&LestcPacketData, dici_index);
					IOOutputStruct[dici_index].IOLevel = 1;					//��ƽ��1
					IOOutputStruct[dici_index].IOHighUseTime = 0;			//�ߵ�ƽʱ����0
				}
				else {
					if (dici_index < OUTPUT_NUM) {
						GPIO_ResetBits(OUTPUT_TYPE[dici_index],  OUTPUT_PIN[dici_index]);
					}
					LestcCarOutSetStatus(&LestcPacketData, dici_index);
					IOOutputStruct[dici_index].IOLevel = 0;					//��ƽ��0
					IOOutputStruct[dici_index].IOHighUseTime = 0;			//�ߵ�ƽʱ����0
				}
			}
			else {													//��Ҫ���䳵����
				IOOutputStruct[dici_index].SupplyCarNum = carnum;				//��ȡ��Ҫ�����ĳ�����
				IOOutputStruct[dici_index].SupplyedIOLevel = dici_in;			//��ȡ���䳵���������������ƽ
				IOOutputStruct[dici_index].SupplyingFlag = 1;				//��־���������ڲ�����
			}
		}
		else {														//���ڲ���
			IOOutputStruct[dici_index].SupplyingCarNum += carnum;				//��ȡ���ڲ������յ��ĳ�����
			if (IOOutputStruct[dici_index].SupplyingCarNum >= IOOutputNUM) {		//���ڲ������յ��ĳ����������貹���������
				IOOutputStruct[dici_index].SupplyingCarNum = IOOutputNUM;
			}
			IOOutputStruct[dici_index].SupplyedIOLevel = dici_in;				//��ȡ���䳵���������������ƽ
		}
	}
}

/**********************************************************************************************************
 @Function			void IOOutput_Mode0_Supplying(void)
 @Description			IO�����ʱ������ 1msִ��һ�� (�����ʽ0 : ���泵�����)
 @Input				void
 @Return				void
**********************************************************************************************************/
void IOOutput_Mode0_Supplying(void)
{
	u8 i = 0;
	
	for (i = 0; i < IOOutputMAX; i++) {
		if (IOOutputStruct[i].OutputID != 0) {								//ID��Ϊ0����
			
			/* �ߵ�ƽ��ʱ���� */
			if (IOOutputStruct[i].IOLevel == 1) {
				IOOutputStruct[i].IOHighUseTime += 1;
				if (IOOutputStruct[i].IOHighUseTime >= (param_recv.output_high_max * 1000)) {
					if (i < OUTPUT_NUM) {
						GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
					}
					LestcCarOutSetStatus(&LestcPacketData, i);
					IOOutputStruct[i].IOLevel = 0;						//��ƽ��0
					IOOutputStruct[i].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
				}
			}
			
			/* ���������� */
			if (IOOutputStruct[i].SupplyingFlag != 0) {
				
				if (IOOutputStruct[i].SupplyUseTime == 0) {
					if (i < OUTPUT_NUM) {
						GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
					}
					LestcCarOutSetStatus(&LestcPacketData, i);
					IOOutputStruct[i].IOLevel = 0;						//��ƽ��0
					IOOutputStruct[i].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
				}
				else if (IOOutputStruct[i].SupplyUseTime == (param_recv.output_low_min * 20)) {
					if (i < OUTPUT_NUM) {
						GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
					}
					LestcCarInSetStatus(&LestcPacketData, i);
					IOOutputStruct[i].IOLevel = 1;						//��ƽ��1
					IOOutputStruct[i].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
				}
				
				IOOutputStruct[i].SupplyUseTime += 1;
				
				if (IOOutputStruct[i].SupplyUseTime >= ((param_recv.output_low_min * 20) + (param_recv.vi_output_high_time * 20) + 1)) {
					IOOutputStruct[i].SupplyUseTime = 0;
					IOOutputStruct[i].SupplyCarNum -= 1;					//������������һ
					if (IOOutputStruct[i].SupplyCarNum == 0) {				//�������������
						IOOutputStruct[i].SupplyCarNum = 0;				//��ղ���������
						IOOutputStruct[i].SupplyingFlag = 0;				//����������0
						if (IOOutputStruct[i].SupplyedIOLevel == 0) {		//����������ƽֵ
							if (i < OUTPUT_NUM) {
								GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
							}
							LestcCarOutSetStatus(&LestcPacketData, i);
							IOOutputStruct[i].IOLevel = 0;				//��ƽ��0
							IOOutputStruct[i].IOHighUseTime = 0;			//�ߵ�ƽʱ����0
						}
						else {
							if (i < OUTPUT_NUM) {
								GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
							}
							LestcCarInSetStatus(&LestcPacketData, i);
							IOOutputStruct[i].IOLevel = 1;				//��ƽ��1
							IOOutputStruct[i].IOHighUseTime = 0;			//�ߵ�ƽʱ����0
						}
						
						if (IOOutputStruct[i].SupplyingCarNum != 0) {		//�������յ�������
							IOOutputStruct[i].SupplyCarNum += IOOutputStruct[i].SupplyingCarNum;					//��ȡ��Ҫ�����ĳ�����
							IOOutputStruct[i].SupplyingCarNum = 0;
							IOOutputStruct[i].SupplyingFlag = 1;			//��־���������ڲ�����
						}
					}
					else if (IOOutputStruct[i].SupplyCarNum > IOOutputNUM) {	//�����貹��������
						IOOutputStruct[i].SupplyCarNum = IOOutputNUM;		//������󲹷���
					}
				}
			}
		}
	}
}


/**********************************************************************************************************
 @Function			void IOOutput_Mode1_Check(u16 dici_index, u16 dici_carnum, u8 dici_in)
 @Description			IO���У������ (�����ʽ1 : ������������̶�ʱ��(����))
 @Input				dici_index  	: �ش�ID���
					dici_carnum 	: �õشų�����״ֵ̬
					dici_in	  	: 1.��������
							       0.�����뿪
 @Return				void
**********************************************************************************************************/
void IOOutput_Mode1_Check(u16 dici_index, u16 dici_carnum, u8 dici_in)
{
	u16 carnum = 0;
	
	if (IOOutputStruct[dici_index].CarNumState == 0) {						//��ʼ��ID������
		IOOutputStruct[dici_index].CarNumState = dici_carnum;					//��¼������״ֵ̬
		if (dici_in == 1) {
			if (dici_index < OUTPUT_NUM) {
				GPIO_SetBits(OUTPUT_TYPE[dici_index],  OUTPUT_PIN[dici_index]);
			}
			LestcCarInSetStatus(&LestcPacketData, dici_index);
			IOOutputStruct[dici_index].IOLevel = 1;							//��ƽ��1
			IOOutputStruct[dici_index].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
		}
		else {
			if (dici_index < OUTPUT_NUM) {
				GPIO_ResetBits(OUTPUT_TYPE[dici_index],  OUTPUT_PIN[dici_index]);
			}
			LestcCarOutSetStatus(&LestcPacketData, dici_index);
			IOOutputStruct[dici_index].IOLevel = 0;							//��ƽ��0
			IOOutputStruct[dici_index].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
		}
	}
	else {															//�ѳ�ʼ�����
		if (dici_carnum > IOOutputStruct[dici_index].CarNumState) {				//���㳵����ֵ
			if ((dici_carnum - IOOutputStruct[dici_index].CarNumState) < IOOutputNUM) {
				carnum = dici_carnum - IOOutputStruct[dici_index].CarNumState;
			}
		}
		else if (dici_carnum < IOOutputStruct[dici_index].CarNumState) {
			if ((65535 - IOOutputStruct[dici_index].CarNumState + dici_carnum) < IOOutputNUM) {
				carnum = 65535 - IOOutputStruct[dici_index].CarNumState + dici_carnum;
			}
		}
		IOOutputStruct[dici_index].CarNumState = dici_carnum;					//��¼������״ֵ̬
		
		/* �жϸõش��Ƿ����ڲ������� */
		if (IOOutputStruct[dici_index].SupplyingFlag == 0) {					//��������
			if (carnum != 0) {											//��Ҫ���䳵����
				IOOutputStruct[dici_index].SupplyCarNum = carnum;				//��ȡ��Ҫ�����ĳ�����
				IOOutputStruct[dici_index].SupplyedIOLevel = 0;				//���䳵��������������͵�ƽ
				IOOutputStruct[dici_index].SupplyingFlag = 1;				//��־���������ڲ�����
			}
		}
		else {														//���ڲ���
			IOOutputStruct[dici_index].SupplyingCarNum += carnum;				//��ȡ���ڲ������յ��ĳ�����
			if (IOOutputStruct[dici_index].SupplyingCarNum >= IOOutputNUM) {		//���ڲ������յ��ĳ����������貹���������
				IOOutputStruct[dici_index].SupplyingCarNum = IOOutputNUM;
			}
			IOOutputStruct[dici_index].SupplyedIOLevel = 0;					//���䳵��������������͵�ƽ
		}
	}
}

/**********************************************************************************************************
 @Function			void IOOutput_Mode1_Supplying(void)
 @Description			IO�����ʱ������ 1msִ��һ�� (�����ʽ1 : ������������̶�ʱ��(����))
 @Input				void
 @Return				void
**********************************************************************************************************/
void IOOutput_Mode1_Supplying(void)
{
	u8 i = 0;
	
	for (i = 0; i < IOOutputMAX; i++) {
		if (IOOutputStruct[i].OutputID != 0) {								//ID��Ϊ0����
			
			/* ���������� */
			if (IOOutputStruct[i].SupplyingFlag != 0) {
				
				if (IOOutputStruct[i].SupplyUseTime == 0) {
					if (i < OUTPUT_NUM) {
						GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
					}
					LestcCarInSetStatus(&LestcPacketData, i);
					IOOutputStruct[i].IOLevel = 1;						//��ƽ��1
					IOOutputStruct[i].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
				}
				else if (IOOutputStruct[i].SupplyUseTime == (param_recv.vi_output_high_time * 20)) {
					if (i < OUTPUT_NUM) {
						GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
					}
					LestcCarOutSetStatus(&LestcPacketData, i);
					IOOutputStruct[i].IOLevel = 0;						//��ƽ��0
					IOOutputStruct[i].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
				}
				
				IOOutputStruct[i].SupplyUseTime += 1;
				
				if (IOOutputStruct[i].SupplyUseTime >= ((param_recv.vi_output_high_time * 20) + (param_recv.output_low_min * 20) + 1)) {
					IOOutputStruct[i].SupplyUseTime = 0;
					IOOutputStruct[i].SupplyCarNum -= 1;					//������������һ
					if (IOOutputStruct[i].SupplyCarNum == 0) {				//�������������
						IOOutputStruct[i].SupplyCarNum = 0;				//��ղ���������
						IOOutputStruct[i].SupplyingFlag = 0;				//����������0
						if (i < OUTPUT_NUM) {
							GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
						}
						LestcCarOutSetStatus(&LestcPacketData, i);
						IOOutputStruct[i].IOLevel = 0;					//��ƽ��0
						IOOutputStruct[i].IOHighUseTime = 0;				//�ߵ�ƽʱ����0
						
						if (IOOutputStruct[i].SupplyingCarNum != 0) {		//�������յ�������
							IOOutputStruct[i].SupplyCarNum += IOOutputStruct[i].SupplyingCarNum;					//��ȡ��Ҫ�����ĳ�����
							IOOutputStruct[i].SupplyingCarNum = 0;
							IOOutputStruct[i].SupplyingFlag = 1;			//��־���������ڲ�����
						}
					}
					else if (IOOutputStruct[i].SupplyCarNum > IOOutputNUM) {	//�����貹��������
						IOOutputStruct[i].SupplyCarNum = IOOutputNUM;		//������󲹷���
					}
				}
			}
		}
	}
}


/**********************************************************************************************************
 @Function			void IOOutput_Mode2_Check(u16 dici_index, u16 dici_carnum, u8 dici_in)
 @Description			IO���У������ (�����ʽ2 : �����뿪����̶�ʱ��(����))
 @Input				dici_index  	: �ش�ID���
					dici_carnum 	: �õشų�����״ֵ̬
					dici_in	  	: 1.��������
							       0.�����뿪
 @Return				void
**********************************************************************************************************/
void IOOutput_Mode2_Check(u16 dici_index, u16 dici_carnum, u8 dici_in)
{
	u16 carnum = 0;
	
	if (IOOutputStruct[dici_index].CarNumState == 0) {						//��ʼ��ID������
		IOOutputStruct[dici_index].CarNumState = dici_carnum;					//��¼������״ֵ̬
		if (dici_in == 1) {
			if (dici_index < OUTPUT_NUM) {
				GPIO_SetBits(OUTPUT_TYPE[dici_index],  OUTPUT_PIN[dici_index]);
			}
			LestcCarInSetStatus(&LestcPacketData, dici_index);
			IOOutputStruct[dici_index].IOLevel = 1;							//��ƽ��1
			IOOutputStruct[dici_index].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
		}
		else {
			if (dici_index < OUTPUT_NUM) {
				GPIO_ResetBits(OUTPUT_TYPE[dici_index],  OUTPUT_PIN[dici_index]);
			}
			LestcCarOutSetStatus(&LestcPacketData, dici_index);
			IOOutputStruct[dici_index].IOLevel = 0;							//��ƽ��0
			IOOutputStruct[dici_index].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
		}
	}
	else {															//�ѳ�ʼ�����
		if (dici_carnum > IOOutputStruct[dici_index].CarNumState) {				//���㳵����ֵ
			if ((dici_carnum - IOOutputStruct[dici_index].CarNumState) < IOOutputNUM) {
				carnum = dici_carnum - IOOutputStruct[dici_index].CarNumState;
			}
		}
		else if (dici_carnum < IOOutputStruct[dici_index].CarNumState) {
			if ((65535 - IOOutputStruct[dici_index].CarNumState + dici_carnum) < IOOutputNUM) {
				carnum = 65535 - IOOutputStruct[dici_index].CarNumState + dici_carnum;
			}
		}
		IOOutputStruct[dici_index].CarNumState = dici_carnum;					//��¼������״ֵ̬
		
		/* �жϸõش��Ƿ����ڲ������� */
		if (IOOutputStruct[dici_index].SupplyingFlag == 0) {					//��������
			if (carnum != 0) {											//��Ҫ���䳵����
				IOOutputStruct[dici_index].SupplyCarNum = carnum;				//��ȡ��Ҫ�����ĳ�����
				IOOutputStruct[dici_index].SupplyedIOLevel = 0;				//���䳵��������������͵�ƽ
				IOOutputStruct[dici_index].SupplyingFlag = 1;				//��־���������ڲ�����
			}
		}
		else {														//���ڲ���
			IOOutputStruct[dici_index].SupplyingCarNum += carnum;				//��ȡ���ڲ������յ��ĳ�����
			if (IOOutputStruct[dici_index].SupplyingCarNum >= IOOutputNUM) {		//���ڲ������յ��ĳ����������貹���������
				IOOutputStruct[dici_index].SupplyingCarNum = IOOutputNUM;
			}
			IOOutputStruct[dici_index].SupplyedIOLevel = 0;					//���䳵��������������͵�ƽ
		}
	}
}

/**********************************************************************************************************
 @Function			void IOOutput_Mode2_Supplying(void)
 @Description			IO�����ʱ������ 1msִ��һ�� (�����ʽ2 : �����뿪����̶�ʱ��(����))
 @Input				void
 @Return				void
**********************************************************************************************************/
void IOOutput_Mode2_Supplying(void)
{
	u8 i = 0;
	
	for (i = 0; i < IOOutputMAX; i++) {
		if (IOOutputStruct[i].OutputID != 0) {								//ID��Ϊ0����
			
			/* ���������� */
			if (IOOutputStruct[i].SupplyingFlag != 0) {
				
				if (IOOutputStruct[i].SupplyUseTime == 0) {
					if (i < OUTPUT_NUM) {
						GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
					}
					LestcCarInSetStatus(&LestcPacketData, i);
					IOOutputStruct[i].IOLevel = 1;						//��ƽ��1
					IOOutputStruct[i].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
				}
				else if (IOOutputStruct[i].SupplyUseTime == (param_recv.vo_output_high_time * 20)) {
					if (i < OUTPUT_NUM) {
						GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
					}
					LestcCarOutSetStatus(&LestcPacketData, i);
					IOOutputStruct[i].IOLevel = 0;						//��ƽ��0
					IOOutputStruct[i].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
				}
				
				IOOutputStruct[i].SupplyUseTime += 1;
				
				if (IOOutputStruct[i].SupplyUseTime >= ((param_recv.vo_output_high_time * 20) + (param_recv.output_low_min * 20) + 1)) {
					IOOutputStruct[i].SupplyUseTime = 0;
					IOOutputStruct[i].SupplyCarNum -= 1;					//������������һ
					if (IOOutputStruct[i].SupplyCarNum == 0) {				//�������������
						IOOutputStruct[i].SupplyCarNum = 0;				//��ղ���������
						IOOutputStruct[i].SupplyingFlag = 0;				//����������0
						if (i < OUTPUT_NUM) {
							GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
						}
						LestcCarOutSetStatus(&LestcPacketData, i);
						IOOutputStruct[i].IOLevel = 0;					//��ƽ��0
						IOOutputStruct[i].IOHighUseTime = 0;				//�ߵ�ƽʱ����0
						
						if (IOOutputStruct[i].SupplyingCarNum != 0) {		//�������յ�������
							IOOutputStruct[i].SupplyCarNum += IOOutputStruct[i].SupplyingCarNum;					//��ȡ��Ҫ�����ĳ�����
							IOOutputStruct[i].SupplyingCarNum = 0;
							IOOutputStruct[i].SupplyingFlag = 1;			//��־���������ڲ�����
						}
					}
					else if (IOOutputStruct[i].SupplyCarNum > IOOutputNUM) {	//�����貹��������
						IOOutputStruct[i].SupplyCarNum = IOOutputNUM;		//������󲹷���
					}
				}
			}
		}
	}
}


/**********************************************************************************************************
 @Function			void IOOutput_Mode3_Check(u16 dici_index, u16 dici_carnum, u8 dici_in)
 @Description			IO���У������ (�����ʽ3 : ��������,�뿪ʱ������̶�ʱ��(����))
 @Input				dici_index  	: �ش�ID���
					dici_carnum 	: �õشų�����״ֵ̬
					dici_in	  	: 1.��������
							       0.�����뿪
 @Return				void
**********************************************************************************************************/
void IOOutput_Mode3_Check(u16 dici_index, u16 dici_carnum, u8 dici_in)
{
	u16 carnum = 0;
	
	if (IOOutputStruct[dici_index].CarNumState == 0) {						//��ʼ��ID������
		IOOutputStruct[dici_index].CarNumState = dici_carnum;					//��¼������״ֵ̬
		if (dici_in == 1) {
			if (dici_index < OUTPUT_NUM) {
				GPIO_SetBits(OUTPUT_TYPE[dici_index],  OUTPUT_PIN[dici_index]);
			}
			LestcCarInSetStatus(&LestcPacketData, dici_index);
			IOOutputStruct[dici_index].IOLevel = 1;							//��ƽ��1
			IOOutputStruct[dici_index].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
		}
		else {
			if (dici_index < OUTPUT_NUM) {
				GPIO_ResetBits(OUTPUT_TYPE[dici_index],  OUTPUT_PIN[dici_index]);
			}
			LestcCarOutSetStatus(&LestcPacketData, dici_index);
			IOOutputStruct[dici_index].IOLevel = 0;							//��ƽ��0
			IOOutputStruct[dici_index].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
		}
	}
	else {															//�ѳ�ʼ�����
		if (dici_carnum > IOOutputStruct[dici_index].CarNumState) {				//���㳵����ֵ
			if ((dici_carnum - IOOutputStruct[dici_index].CarNumState) < IOOutputNUM) {
				carnum = dici_carnum - IOOutputStruct[dici_index].CarNumState;
			}
		}
		else if (dici_carnum < IOOutputStruct[dici_index].CarNumState) {
			if ((65535 - IOOutputStruct[dici_index].CarNumState + dici_carnum) < IOOutputNUM) {
				carnum = 65535 - IOOutputStruct[dici_index].CarNumState + dici_carnum;
			}
		}
		IOOutputStruct[dici_index].CarNumState = dici_carnum;					//��¼������״ֵ̬
		
		/* �жϸõش��Ƿ����ڲ������� */
		if (IOOutputStruct[dici_index].SupplyingFlag == 0) {					//��������
			if (carnum != 0) {											//��Ҫ���䳵����
				if (dici_in == 1) {
					IOOutputStruct[dici_index].SupplyCarNum = (carnum * 2 - 1);	//��ȡ��Ҫ�����ĳ�����
					IOOutputStruct[dici_index].CarInOrOut = 1;				//������
					IOOutputStruct[dici_index].CarEvenOrOdd = 0;				//������
				}
				else {
					IOOutputStruct[dici_index].SupplyCarNum = (carnum * 2);	//��ȡ��Ҫ�����ĳ�����
					IOOutputStruct[dici_index].CarInOrOut = 0;				//�����
					IOOutputStruct[dici_index].CarEvenOrOdd = 1;				//ż����
				}
				IOOutputStruct[dici_index].SupplyedIOLevel = 0;				//���䳵��������������͵�ƽ
				IOOutputStruct[dici_index].SupplyingFlag = 1;				//��־���������ڲ�����
			}
			else {													//�����carnum����
				IOOutputStruct[dici_index].SupplyCarNum = 1;					//��ȡ��Ҫ�����ĳ�����
				IOOutputStruct[dici_index].CarInOrOut = 0;					//�����
				IOOutputStruct[dici_index].CarEvenOrOdd = 0;					//������
				IOOutputStruct[dici_index].SupplyedIOLevel = 0;				//���䳵��������������͵�ƽ
				IOOutputStruct[dici_index].SupplyingFlag = 1;				//��־���������ڲ�����
			}
		}
		else {																	//���ڲ���
			if (carnum != 0) {														//��Ҫ���䳵����
				if (dici_in == 1) {
					IOOutputStruct[dici_index].SupplyingCarNum += (carnum * 2 - 1);			//��ȡ���ڲ������յ��ĳ�����
					if (IOOutputStruct[dici_index].SupplyingCarNum >= (IOOutputNUM * 2)) {
						IOOutputStruct[dici_index].SupplyingCarNum -= (carnum * 2 - 1);
					}
					else {
						IOOutputStruct[dici_index].SupplyingCarInOrOut = 1;				//������
					}
				}
				else {
					IOOutputStruct[dici_index].SupplyingCarNum += (carnum * 2);				//��ȡ���ڲ������յ��ĳ�����
					if (IOOutputStruct[dici_index].SupplyingCarNum >= (IOOutputNUM * 2)) {
						IOOutputStruct[dici_index].SupplyingCarNum -= (carnum * 2);
					}
					else {
						IOOutputStruct[dici_index].SupplyingCarInOrOut = 0;				//�����
					}
				}
			}
			else {																//�����carnum����
				IOOutputStruct[dici_index].SupplyingCarNum += 1;
				if (IOOutputStruct[dici_index].SupplyingCarNum >= (IOOutputNUM * 2)) {
					IOOutputStruct[dici_index].SupplyingCarNum -= 1;
				}
				else {
					IOOutputStruct[dici_index].SupplyingCarInOrOut = 0;					//�����
				}
			}
			
			IOOutputStruct[dici_index].SupplyedIOLevel = 0;								//���䳵��������������͵�ƽ
		}
	}
}

/**********************************************************************************************************
 @Function			void IOOutput_Mode3_Supplying(void)
 @Description			IO�����ʱ������ 1msִ��һ�� (�����ʽ3 : ��������,�뿪ʱ������̶�ʱ��(����))
 @Input				void
 @Return				void
**********************************************************************************************************/
void IOOutput_Mode3_Supplying(void)
{
	u8 i = 0;
	u16 hightIOMaxTime = 0;
	
	for (i = 0; i < IOOutputMAX; i++) {
		if (IOOutputStruct[i].OutputID != 0) {												//ID��Ϊ0����
			
			/* �ߵ�ƽ��ʱ���� */
			if (IOOutputStruct[i].IOLevel == 1) {
				IOOutputStruct[i].IOHighUseTime += 1;
				if (param_recv.vi_output_high_time > param_recv.vo_output_high_time) {
					hightIOMaxTime = param_recv.vi_output_high_time;
				}
				else {
					hightIOMaxTime = param_recv.vo_output_high_time;
				}
				
				if (IOOutputStruct[i].IOHighUseTime >= (hightIOMaxTime * 1000)) {
					if (i < OUTPUT_NUM) {
						GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
					}
					LestcCarOutSetStatus(&LestcPacketData, i);
					IOOutputStruct[i].IOLevel = 0;										//��ƽ��0
					IOOutputStruct[i].IOHighUseTime = 0;									//�ߵ�ƽʱ����0
				}
			}
			
			/* ���������� */
			if (IOOutputStruct[i].SupplyingFlag != 0) {
				
				if (IOOutputStruct[i].CarInOrOut == 1) {									//���һ��Ϊ������
					if (IOOutputStruct[i].CarEvenOrOdd == 0) {								//������
						if ((IOOutputStruct[i].SupplyCarNum % 2) == 1) {
							if (IOOutputStruct[i].SupplyUseTime == 0) {
								if (i < OUTPUT_NUM) {
									GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarInSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 1;							//��ƽ��1
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							else if (IOOutputStruct[i].SupplyUseTime == (param_recv.vi_output_high_time * 20)) {
								if (i < OUTPUT_NUM) {
									GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarOutSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 0;							//��ƽ��0
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							
							IOOutputStruct[i].SupplyUseTime += 1;
							
							if (IOOutputStruct[i].SupplyUseTime >= ((param_recv.vi_output_high_time * 20) + (param_recv.output_low_min * 20) + 1)) {
								IOOutputStruct[i].SupplyUseTime = 0;
								IOOutputStruct[i].SupplyCarNum -= 1;						//������������һ
								if (IOOutputStruct[i].SupplyCarNum == 0) {					//�������������
									IOOutputStruct[i].SupplyCarNum = 0;					//��ղ���������
									IOOutputStruct[i].SupplyingFlag = 0;					//����������0
									if (i < OUTPUT_NUM) {
										GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
									}
									LestcCarOutSetStatus(&LestcPacketData, i);
									IOOutputStruct[i].IOLevel = 0;						//��ƽ��0
									IOOutputStruct[i].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
									
									if (IOOutputStruct[i].SupplyingCarNum != 0) {			//�������յ�������
										IOOutputStruct[i].SupplyCarNum = IOOutputStruct[i].SupplyingCarNum;		//��ȡ��Ҫ�����ĳ�����
										IOOutputStruct[i].CarInOrOut = IOOutputStruct[i].SupplyingCarInOrOut;
										if (IOOutputStruct[i].SupplyingCarNum % 2 == 1) {
											IOOutputStruct[i].CarEvenOrOdd = 0;
										}
										else {
											IOOutputStruct[i].CarEvenOrOdd = 1;
										}
										IOOutputStruct[i].SupplyingCarNum = 0;
										IOOutputStruct[i].SupplyingFlag = 1;				//��־���������ڲ�����
									}
								}
								else if (IOOutputStruct[i].SupplyCarNum > (IOOutputNUM * 2)) {	//�����貹��������
									IOOutputStruct[i].SupplyCarNum = (IOOutputNUM * 2);		//������󲹷���
								}
							}
						}
						else {
							if (IOOutputStruct[i].SupplyUseTime == 0) {
								if (i < OUTPUT_NUM) {
									GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarInSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 1;							//��ƽ��1
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							else if (IOOutputStruct[i].SupplyUseTime == (param_recv.vo_output_high_time * 20)) {
								if (i < OUTPUT_NUM) {
									GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarOutSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 0;							//��ƽ��0
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							
							IOOutputStruct[i].SupplyUseTime += 1;
							
							if (IOOutputStruct[i].SupplyUseTime >= ((param_recv.vo_output_high_time * 20) + (param_recv.output_low_min * 20) + 1)) {
								IOOutputStruct[i].SupplyUseTime = 0;
								IOOutputStruct[i].SupplyCarNum -= 1;						//������������һ
								if (IOOutputStruct[i].SupplyCarNum == 0) {					//�������������
									IOOutputStruct[i].SupplyCarNum = 0;					//��ղ���������
									IOOutputStruct[i].SupplyingFlag = 0;					//����������0
									if (i < OUTPUT_NUM) {
										GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
									}
									LestcCarOutSetStatus(&LestcPacketData, i);
									IOOutputStruct[i].IOLevel = 0;						//��ƽ��0
									IOOutputStruct[i].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
									
									if (IOOutputStruct[i].SupplyingCarNum != 0) {			//�������յ�������
										IOOutputStruct[i].SupplyCarNum = IOOutputStruct[i].SupplyingCarNum;		//��ȡ��Ҫ�����ĳ�����
										IOOutputStruct[i].CarInOrOut = IOOutputStruct[i].SupplyingCarInOrOut;
										if (IOOutputStruct[i].SupplyingCarNum % 2 == 1) {
											IOOutputStruct[i].CarEvenOrOdd = 0;
										}
										else {
											IOOutputStruct[i].CarEvenOrOdd = 1;
										}
										IOOutputStruct[i].SupplyingCarNum = 0;
										IOOutputStruct[i].SupplyingFlag = 1;				//��־���������ڲ�����
									}
								}
								else if (IOOutputStruct[i].SupplyCarNum > (IOOutputNUM * 2)) {	//�����貹��������
									IOOutputStruct[i].SupplyCarNum = (IOOutputNUM * 2);		//������󲹷���
								}
							}
						}
					}
					else {															//ż����
						if ((IOOutputStruct[i].SupplyCarNum % 2) == 1) {
							if (IOOutputStruct[i].SupplyUseTime == 0) {
								if (i < OUTPUT_NUM) {
									GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarInSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 1;							//��ƽ��1
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							else if (IOOutputStruct[i].SupplyUseTime == (param_recv.vi_output_high_time * 20)) {
								if (i < OUTPUT_NUM) {
									GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarOutSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 0;							//��ƽ��0
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							
							IOOutputStruct[i].SupplyUseTime += 1;
							
							if (IOOutputStruct[i].SupplyUseTime >= ((param_recv.vi_output_high_time * 20) + (param_recv.output_low_min * 20) + 1)) {
								IOOutputStruct[i].SupplyUseTime = 0;
								IOOutputStruct[i].SupplyCarNum -= 1;						//������������һ
								if (IOOutputStruct[i].SupplyCarNum == 0) {					//�������������
									IOOutputStruct[i].SupplyCarNum = 0;					//��ղ���������
									IOOutputStruct[i].SupplyingFlag = 0;					//����������0
									if (i < OUTPUT_NUM) {
										GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
									}
									LestcCarOutSetStatus(&LestcPacketData, i);
									IOOutputStruct[i].IOLevel = 0;						//��ƽ��0
									IOOutputStruct[i].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
									
									if (IOOutputStruct[i].SupplyingCarNum != 0) {			//�������յ�������
										IOOutputStruct[i].SupplyCarNum = IOOutputStruct[i].SupplyingCarNum;		//��ȡ��Ҫ�����ĳ�����
										IOOutputStruct[i].CarInOrOut = IOOutputStruct[i].SupplyingCarInOrOut;
										if (IOOutputStruct[i].SupplyingCarNum % 2 == 1) {
											IOOutputStruct[i].CarEvenOrOdd = 0;
										}
										else {
											IOOutputStruct[i].CarEvenOrOdd = 1;
										}
										IOOutputStruct[i].SupplyingCarNum = 0;
										IOOutputStruct[i].SupplyingFlag = 1;				//��־���������ڲ�����
									}
								}
								else if (IOOutputStruct[i].SupplyCarNum > (IOOutputNUM * 2)) {	//�����貹��������
									IOOutputStruct[i].SupplyCarNum = (IOOutputNUM * 2);		//������󲹷���
								}
							}
						}
						else {
							if (IOOutputStruct[i].SupplyUseTime == 0) {
								if (i < OUTPUT_NUM) {
									GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarInSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 1;							//��ƽ��1
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							else if (IOOutputStruct[i].SupplyUseTime == (param_recv.vo_output_high_time * 20)) {
								if (i < OUTPUT_NUM) {
									GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarOutSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 0;							//��ƽ��0
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							
							IOOutputStruct[i].SupplyUseTime += 1;
							
							if (IOOutputStruct[i].SupplyUseTime >= ((param_recv.vo_output_high_time * 20) + (param_recv.output_low_min * 20) + 1)) {
								IOOutputStruct[i].SupplyUseTime = 0;
								IOOutputStruct[i].SupplyCarNum -= 1;						//������������һ
								if (IOOutputStruct[i].SupplyCarNum == 0) {					//�������������
									IOOutputStruct[i].SupplyCarNum = 0;					//��ղ���������
									IOOutputStruct[i].SupplyingFlag = 0;					//����������0
									if (i < OUTPUT_NUM) {
										GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
									}
									LestcCarOutSetStatus(&LestcPacketData, i);
									IOOutputStruct[i].IOLevel = 0;						//��ƽ��0
									IOOutputStruct[i].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
									
									if (IOOutputStruct[i].SupplyingCarNum != 0) {			//�������յ�������
										IOOutputStruct[i].SupplyCarNum = IOOutputStruct[i].SupplyingCarNum;		//��ȡ��Ҫ�����ĳ�����
										IOOutputStruct[i].CarInOrOut = IOOutputStruct[i].SupplyingCarInOrOut;
										if (IOOutputStruct[i].SupplyingCarNum % 2 == 1) {
											IOOutputStruct[i].CarEvenOrOdd = 0;
										}
										else {
											IOOutputStruct[i].CarEvenOrOdd = 1;
										}
										IOOutputStruct[i].SupplyingCarNum = 0;
										IOOutputStruct[i].SupplyingFlag = 1;				//��־���������ڲ�����
									}
								}
								else if (IOOutputStruct[i].SupplyCarNum > (IOOutputNUM * 2)) {	//�����貹��������
									IOOutputStruct[i].SupplyCarNum = (IOOutputNUM * 2);		//������󲹷���
								}
							}
						}
					}
				}
				else {													//���һ��Ϊ�����
					if (IOOutputStruct[i].CarEvenOrOdd == 0) {					//������
						if ((IOOutputStruct[i].SupplyCarNum % 2) == 1) {
							if (IOOutputStruct[i].SupplyUseTime == 0) {
								if (i < OUTPUT_NUM) {
									GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarInSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 1;							//��ƽ��1
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							else if (IOOutputStruct[i].SupplyUseTime == (param_recv.vo_output_high_time * 20)) {
								if (i < OUTPUT_NUM) {
									GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarOutSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 0;							//��ƽ��0
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							
							IOOutputStruct[i].SupplyUseTime += 1;
							
							if (IOOutputStruct[i].SupplyUseTime >= ((param_recv.vo_output_high_time * 20) + (param_recv.output_low_min * 20) + 1)) {
								IOOutputStruct[i].SupplyUseTime = 0;
								IOOutputStruct[i].SupplyCarNum -= 1;						//������������һ
								if (IOOutputStruct[i].SupplyCarNum == 0) {					//�������������
									IOOutputStruct[i].SupplyCarNum = 0;					//��ղ���������
									IOOutputStruct[i].SupplyingFlag = 0;					//����������0
									if (i < OUTPUT_NUM) {
										GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
									}
									LestcCarOutSetStatus(&LestcPacketData, i);
									IOOutputStruct[i].IOLevel = 0;						//��ƽ��0
									IOOutputStruct[i].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
									
									if (IOOutputStruct[i].SupplyingCarNum != 0) {			//�������յ�������
										IOOutputStruct[i].SupplyCarNum = IOOutputStruct[i].SupplyingCarNum;		//��ȡ��Ҫ�����ĳ�����
										IOOutputStruct[i].CarInOrOut = IOOutputStruct[i].SupplyingCarInOrOut;
										if (IOOutputStruct[i].SupplyingCarNum % 2 == 1) {
											IOOutputStruct[i].CarEvenOrOdd = 0;
										}
										else {
											IOOutputStruct[i].CarEvenOrOdd = 1;
										}
										IOOutputStruct[i].SupplyingCarNum = 0;
										IOOutputStruct[i].SupplyingFlag = 1;				//��־���������ڲ�����
									}
								}
								else if (IOOutputStruct[i].SupplyCarNum > (IOOutputNUM * 2)) {	//�����貹��������
									IOOutputStruct[i].SupplyCarNum = (IOOutputNUM * 2);		//������󲹷���
								}
							}
						}
						else {
							if (IOOutputStruct[i].SupplyUseTime == 0) {
								if (i < OUTPUT_NUM) {
									GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarInSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 1;							//��ƽ��1
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							else if (IOOutputStruct[i].SupplyUseTime == (param_recv.vi_output_high_time * 20)) {
								if (i < OUTPUT_NUM) {
									GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarOutSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 0;							//��ƽ��0
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							
							IOOutputStruct[i].SupplyUseTime += 1;
							
							if (IOOutputStruct[i].SupplyUseTime >= ((param_recv.vi_output_high_time * 20) + (param_recv.output_low_min * 20) + 1)) {
								IOOutputStruct[i].SupplyUseTime = 0;
								IOOutputStruct[i].SupplyCarNum -= 1;						//������������һ
								if (IOOutputStruct[i].SupplyCarNum == 0) {					//�������������
									IOOutputStruct[i].SupplyCarNum = 0;					//��ղ���������
									IOOutputStruct[i].SupplyingFlag = 0;					//����������0
									if (i < OUTPUT_NUM) {
										GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
									}
									LestcCarOutSetStatus(&LestcPacketData, i);
									IOOutputStruct[i].IOLevel = 0;						//��ƽ��0
									IOOutputStruct[i].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
									
									if (IOOutputStruct[i].SupplyingCarNum != 0) {			//�������յ�������
										IOOutputStruct[i].SupplyCarNum = IOOutputStruct[i].SupplyingCarNum;		//��ȡ��Ҫ�����ĳ�����
										IOOutputStruct[i].CarInOrOut = IOOutputStruct[i].SupplyingCarInOrOut;
										if (IOOutputStruct[i].SupplyingCarNum % 2 == 1) {
											IOOutputStruct[i].CarEvenOrOdd = 0;
										}
										else {
											IOOutputStruct[i].CarEvenOrOdd = 1;
										}
										IOOutputStruct[i].SupplyingCarNum = 0;
										IOOutputStruct[i].SupplyingFlag = 1;				//��־���������ڲ�����
									}
								}
								else if (IOOutputStruct[i].SupplyCarNum > (IOOutputNUM * 2)) {	//�����貹��������
									IOOutputStruct[i].SupplyCarNum = (IOOutputNUM * 2);		//������󲹷���
								}
							}
						}
					}
					else {												//ż����
						if ((IOOutputStruct[i].SupplyCarNum % 2) == 1) {
							if (IOOutputStruct[i].SupplyUseTime == 0) {
								if (i < OUTPUT_NUM) {
									GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarInSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 1;							//��ƽ��1
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							else if (IOOutputStruct[i].SupplyUseTime == (param_recv.vo_output_high_time * 20)) {
								if (i < OUTPUT_NUM) {
									GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarOutSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 0;							//��ƽ��0
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							
							IOOutputStruct[i].SupplyUseTime += 1;
							
							if (IOOutputStruct[i].SupplyUseTime >= ((param_recv.vo_output_high_time * 20) + (param_recv.output_low_min * 20) + 1)) {
								IOOutputStruct[i].SupplyUseTime = 0;
								IOOutputStruct[i].SupplyCarNum -= 1;						//������������һ
								if (IOOutputStruct[i].SupplyCarNum == 0) {					//�������������
									IOOutputStruct[i].SupplyCarNum = 0;					//��ղ���������
									IOOutputStruct[i].SupplyingFlag = 0;					//����������0
									if (i < OUTPUT_NUM) {
										GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
									}
									LestcCarOutSetStatus(&LestcPacketData, i);
									IOOutputStruct[i].IOLevel = 0;						//��ƽ��0
									IOOutputStruct[i].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
									
									if (IOOutputStruct[i].SupplyingCarNum != 0) {			//�������յ�������
										IOOutputStruct[i].SupplyCarNum = IOOutputStruct[i].SupplyingCarNum;		//��ȡ��Ҫ�����ĳ�����
										IOOutputStruct[i].CarInOrOut = IOOutputStruct[i].SupplyingCarInOrOut;
										if (IOOutputStruct[i].SupplyingCarNum % 2 == 1) {
											IOOutputStruct[i].CarEvenOrOdd = 0;
										}
										else {
											IOOutputStruct[i].CarEvenOrOdd = 1;
										}
										IOOutputStruct[i].SupplyingCarNum = 0;
										IOOutputStruct[i].SupplyingFlag = 1;				//��־���������ڲ�����
									}
								}
								else if (IOOutputStruct[i].SupplyCarNum > (IOOutputNUM * 2)) {	//�����貹��������
									IOOutputStruct[i].SupplyCarNum = (IOOutputNUM * 2);		//������󲹷���
								}
							}
						}
						else {
							if (IOOutputStruct[i].SupplyUseTime == 0) {
								if (i < OUTPUT_NUM) {
									GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarInSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 1;							//��ƽ��1
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							else if (IOOutputStruct[i].SupplyUseTime == (param_recv.vi_output_high_time * 20)) {
								if (i < OUTPUT_NUM) {
									GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarOutSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 0;							//��ƽ��0
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							
							IOOutputStruct[i].SupplyUseTime += 1;
							
							if (IOOutputStruct[i].SupplyUseTime >= ((param_recv.vi_output_high_time * 20) + (param_recv.output_low_min * 20) + 1)) {
								IOOutputStruct[i].SupplyUseTime = 0;
								IOOutputStruct[i].SupplyCarNum -= 1;						//������������һ
								if (IOOutputStruct[i].SupplyCarNum == 0) {					//�������������
									IOOutputStruct[i].SupplyCarNum = 0;					//��ղ���������
									IOOutputStruct[i].SupplyingFlag = 0;					//����������0
									if (i < OUTPUT_NUM) {
										GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
									}
									LestcCarOutSetStatus(&LestcPacketData, i);
									IOOutputStruct[i].IOLevel = 0;						//��ƽ��0
									IOOutputStruct[i].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
									
									if (IOOutputStruct[i].SupplyingCarNum != 0) {			//�������յ�������
										IOOutputStruct[i].SupplyCarNum = IOOutputStruct[i].SupplyingCarNum;		//��ȡ��Ҫ�����ĳ�����
										IOOutputStruct[i].CarInOrOut = IOOutputStruct[i].SupplyingCarInOrOut;
										if (IOOutputStruct[i].SupplyingCarNum % 2 == 1) {
											IOOutputStruct[i].CarEvenOrOdd = 0;
										}
										else {
											IOOutputStruct[i].CarEvenOrOdd = 1;
										}
										IOOutputStruct[i].SupplyingCarNum = 0;
										IOOutputStruct[i].SupplyingFlag = 1;				//��־���������ڲ�����
									}
								}
								else if (IOOutputStruct[i].SupplyCarNum > (IOOutputNUM * 2)) {	//�����貹��������
									IOOutputStruct[i].SupplyCarNum = (IOOutputNUM * 2);		//������󲹷���
								}
							}
						}
					}
				}
			}
		}
	}
}


/**********************************************************************************************************
 @Function			void IOOutput_Mode4_Check(u16 dici_index, u16 dici_carnum, u8 dici_in)
 @Description			IO���У������ (�����ʽ4 : ������������̶�ʱ��(������))
 @Input				dici_index  	: �ش�ID���
					dici_carnum 	: �õشų�����״ֵ̬
					dici_in	  	: 1.��������
							       0.�����뿪
 @Return				void
**********************************************************************************************************/
void IOOutput_Mode4_Check(u16 dici_index, u16 dici_carnum, u8 dici_in)
{
	u16 carnum = 0;
	
	if (IOOutputStruct[dici_index].CarNumState == 0) {						//��ʼ��ID������
		IOOutputStruct[dici_index].CarNumState = dici_carnum;					//��¼������״ֵ̬
		if (dici_in == 1) {
			if (dici_index < OUTPUT_NUM) {
				GPIO_SetBits(OUTPUT_TYPE[dici_index],  OUTPUT_PIN[dici_index]);
			}
			LestcCarInSetStatus(&LestcPacketData, dici_index);
			IOOutputStruct[dici_index].IOLevel = 1;							//��ƽ��1
			IOOutputStruct[dici_index].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
		}
		else {
			if (dici_index < OUTPUT_NUM) {
				GPIO_ResetBits(OUTPUT_TYPE[dici_index],  OUTPUT_PIN[dici_index]);
			}
			LestcCarOutSetStatus(&LestcPacketData, dici_index);
			IOOutputStruct[dici_index].IOLevel = 0;							//��ƽ��0
			IOOutputStruct[dici_index].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
		}
	}
	else {															//�ѳ�ʼ�����
		if (dici_carnum > IOOutputStruct[dici_index].CarNumState) {				//���㳵����ֵ
			if ((dici_carnum - IOOutputStruct[dici_index].CarNumState) < IOOutputNUM) {
				carnum = dici_carnum - IOOutputStruct[dici_index].CarNumState;
			}
		}
		else if (dici_carnum < IOOutputStruct[dici_index].CarNumState) {
			if ((65535 - IOOutputStruct[dici_index].CarNumState + dici_carnum) < IOOutputNUM) {
				carnum = 65535 - IOOutputStruct[dici_index].CarNumState + dici_carnum;
			}
		}
		IOOutputStruct[dici_index].CarNumState = dici_carnum;					//��¼������״ֵ̬
		
		/* �жϸõش��Ƿ����ڲ������� */
		if (IOOutputStruct[dici_index].SupplyingFlag == 0) {					//��������
			if (carnum != 0) {											//��Ҫ���䳵����
				IOOutputStruct[dici_index].SupplyCarNum = 1;					//��ȡ��Ҫ�����ĳ�����
				IOOutputStruct[dici_index].SupplyedIOLevel = 0;				//���䳵��������������͵�ƽ
				IOOutputStruct[dici_index].SupplyingFlag = 1;				//��־���������ڲ�����
			}
		}
		else {														//���ڲ���
			IOOutputStruct[dici_index].SupplyingCarNum = 1;					//��ȡ���ڲ������յ��ĳ�����
			if (IOOutputStruct[dici_index].SupplyingCarNum >= IOOutputNUM) {		//���ڲ������յ��ĳ����������貹���������
				IOOutputStruct[dici_index].SupplyingCarNum = IOOutputNUM;
			}
			IOOutputStruct[dici_index].SupplyedIOLevel = 0;					//���䳵��������������͵�ƽ
		}
	}
}

/**********************************************************************************************************
 @Function			void IOOutput_Mode4_Supplying(void)
 @Description			IO�����ʱ������ 1msִ��һ�� (�����ʽ4 : ������������̶�ʱ��(������))
 @Input				void
 @Return				void
**********************************************************************************************************/
void IOOutput_Mode4_Supplying(void)
{
	u8 i = 0;
	
	for (i = 0; i < IOOutputMAX; i++) {
		if (IOOutputStruct[i].OutputID != 0) {								//ID��Ϊ0����
			
			/* ���������� */
			if (IOOutputStruct[i].SupplyingFlag != 0) {
				
				if (IOOutputStruct[i].SupplyUseTime == 0) {
					if (i < OUTPUT_NUM) {
						GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
					}
					LestcCarInSetStatus(&LestcPacketData, i);
					IOOutputStruct[i].IOLevel = 1;						//��ƽ��1
					IOOutputStruct[i].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
				}
				else if (IOOutputStruct[i].SupplyUseTime == (param_recv.vi_output_high_time * 20)) {
					if (i < OUTPUT_NUM) {
						GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
					}
					LestcCarOutSetStatus(&LestcPacketData, i);
					IOOutputStruct[i].IOLevel = 0;						//��ƽ��0
					IOOutputStruct[i].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
				}
				
				IOOutputStruct[i].SupplyUseTime += 1;
				
				if (IOOutputStruct[i].SupplyUseTime >= ((param_recv.vi_output_high_time * 20) + (param_recv.output_low_min * 20) + 1)) {
					IOOutputStruct[i].SupplyUseTime = 0;
					IOOutputStruct[i].SupplyCarNum -= 1;					//������������һ
					if (IOOutputStruct[i].SupplyCarNum == 0) {				//�������������
						IOOutputStruct[i].SupplyCarNum = 0;				//��ղ���������
						
						if (IOOutputStruct[i].SupplyingCarNum != 0) {		//�������յ�������
							IOOutputStruct[i].SupplyCarNum = IOOutputStruct[i].SupplyingCarNum;					//��ȡ��Ҫ�����ĳ�����
							IOOutputStruct[i].SupplyingCarNum = 0;
							IOOutputStruct[i].SupplyingFlag = 1;			//��־���������ڲ�����
						}
						else {
							IOOutputStruct[i].SupplyingFlag = 0;			//����������0
							if (i < OUTPUT_NUM) {
								GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
							}
							LestcCarOutSetStatus(&LestcPacketData, i);
							IOOutputStruct[i].IOLevel = 0;				//��ƽ��0
							IOOutputStruct[i].IOHighUseTime = 0;			//�ߵ�ƽʱ����0
						}
					}
					else if (IOOutputStruct[i].SupplyCarNum > IOOutputNUM) {	//�����貹��������
						IOOutputStruct[i].SupplyCarNum = IOOutputNUM;		//������󲹷���
					}
				}
			}
		}
	}
}


/**********************************************************************************************************
 @Function			void IOOutput_Mode5_Check(u16 dici_index, u16 dici_carnum, u8 dici_in)
 @Description			IO���У������ (�����ʽ5 : �����뿪����̶�ʱ��(������))
 @Input				dici_index  	: �ش�ID���
					dici_carnum 	: �õشų�����״ֵ̬
					dici_in	  	: 1.��������
							       0.�����뿪
 @Return				void
**********************************************************************************************************/
void IOOutput_Mode5_Check(u16 dici_index, u16 dici_carnum, u8 dici_in)
{
	u16 carnum = 0;
	
	if (IOOutputStruct[dici_index].CarNumState == 0) {						//��ʼ��ID������
		IOOutputStruct[dici_index].CarNumState = dici_carnum;					//��¼������״ֵ̬
		if (dici_in == 1) {
			if (dici_index < OUTPUT_NUM) {
				GPIO_SetBits(OUTPUT_TYPE[dici_index],  OUTPUT_PIN[dici_index]);
			}
			LestcCarInSetStatus(&LestcPacketData, dici_index);
			IOOutputStruct[dici_index].IOLevel = 1;							//��ƽ��1
			IOOutputStruct[dici_index].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
		}
		else {
			if (dici_index < OUTPUT_NUM) {
				GPIO_ResetBits(OUTPUT_TYPE[dici_index],  OUTPUT_PIN[dici_index]);
			}
			LestcCarOutSetStatus(&LestcPacketData, dici_index);
			IOOutputStruct[dici_index].IOLevel = 0;							//��ƽ��0
			IOOutputStruct[dici_index].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
		}
	}
	else {															//�ѳ�ʼ�����
		if (dici_carnum > IOOutputStruct[dici_index].CarNumState) {				//���㳵����ֵ
			if ((dici_carnum - IOOutputStruct[dici_index].CarNumState) < IOOutputNUM) {
				carnum = dici_carnum - IOOutputStruct[dici_index].CarNumState;
			}
		}
		else if (dici_carnum < IOOutputStruct[dici_index].CarNumState) {
			if ((65535 - IOOutputStruct[dici_index].CarNumState + dici_carnum) < IOOutputNUM) {
				carnum = 65535 - IOOutputStruct[dici_index].CarNumState + dici_carnum;
			}
		}
		IOOutputStruct[dici_index].CarNumState = dici_carnum;					//��¼������״ֵ̬
		
		/* �жϸõش��Ƿ����ڲ������� */
		if (IOOutputStruct[dici_index].SupplyingFlag == 0) {					//��������
			if (carnum != 0) {											//��Ҫ���䳵����
				IOOutputStruct[dici_index].SupplyCarNum = 1;					//��ȡ��Ҫ�����ĳ�����
				IOOutputStruct[dici_index].SupplyedIOLevel = 0;				//���䳵��������������͵�ƽ
				IOOutputStruct[dici_index].SupplyingFlag = 1;				//��־���������ڲ�����
			}
		}
		else {														//���ڲ���
			IOOutputStruct[dici_index].SupplyingCarNum = 1;					//��ȡ���ڲ������յ��ĳ�����
			if (IOOutputStruct[dici_index].SupplyingCarNum >= IOOutputNUM) {		//���ڲ������յ��ĳ����������貹���������
				IOOutputStruct[dici_index].SupplyingCarNum = IOOutputNUM;
			}
			IOOutputStruct[dici_index].SupplyedIOLevel = 0;					//���䳵��������������͵�ƽ
		}
	}
}

/**********************************************************************************************************
 @Function			void IOOutput_Mode5_Supplying(void)
 @Description			IO�����ʱ������ 1msִ��һ�� (�����ʽ5 : �����뿪����̶�ʱ��(������))
 @Input				void
 @Return				void
**********************************************************************************************************/
void IOOutput_Mode5_Supplying(void)
{
	u8 i = 0;
	
	for (i = 0; i < IOOutputMAX; i++) {
		if (IOOutputStruct[i].OutputID != 0) {								//ID��Ϊ0����
			
			/* ���������� */
			if (IOOutputStruct[i].SupplyingFlag != 0) {
				
				if (IOOutputStruct[i].SupplyUseTime == 0) {
					if (i < OUTPUT_NUM) {
						GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
					}
					LestcCarInSetStatus(&LestcPacketData, i);
					IOOutputStruct[i].IOLevel = 1;						//��ƽ��1
					IOOutputStruct[i].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
				}
				else if (IOOutputStruct[i].SupplyUseTime == (param_recv.vo_output_high_time * 20)) {
					if (i < OUTPUT_NUM) {
						GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
					}
					LestcCarOutSetStatus(&LestcPacketData, i);
					IOOutputStruct[i].IOLevel = 0;						//��ƽ��0
					IOOutputStruct[i].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
				}
				
				IOOutputStruct[i].SupplyUseTime += 1;
				
				if (IOOutputStruct[i].SupplyUseTime >= ((param_recv.vo_output_high_time * 20) + (param_recv.output_low_min * 20) + 1)) {
					IOOutputStruct[i].SupplyUseTime = 0;
					IOOutputStruct[i].SupplyCarNum -= 1;					//������������һ
					if (IOOutputStruct[i].SupplyCarNum == 0) {				//�������������
						IOOutputStruct[i].SupplyCarNum = 0;				//��ղ���������
						
						if (IOOutputStruct[i].SupplyingCarNum != 0) {		//�������յ�������
							IOOutputStruct[i].SupplyCarNum = IOOutputStruct[i].SupplyingCarNum;					//��ȡ��Ҫ�����ĳ�����
							IOOutputStruct[i].SupplyingCarNum = 0;
							IOOutputStruct[i].SupplyingFlag = 1;			//��־���������ڲ�����
						}
						else {
							IOOutputStruct[i].SupplyingFlag = 0;			//����������0
							if (i < OUTPUT_NUM) {
								GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
							}
							LestcCarOutSetStatus(&LestcPacketData, i);
							IOOutputStruct[i].IOLevel = 0;				//��ƽ��0
							IOOutputStruct[i].IOHighUseTime = 0;			//�ߵ�ƽʱ����0
						}
					}
					else if (IOOutputStruct[i].SupplyCarNum > IOOutputNUM) {	//�����貹��������
						IOOutputStruct[i].SupplyCarNum = IOOutputNUM;		//������󲹷���
					}
				}
			}
		}
	}
}


/**********************************************************************************************************
 @Function			void IOOutput_Mode6_Check(u16 dici_index, u16 dici_carnum, u8 dici_in)
 @Description			IO���У������ (�����ʽ6 : ��������,�뿪ʱ������̶�ʱ��(������))
 @Input				dici_index  	: �ش�ID���
					dici_carnum 	: �õشų�����״ֵ̬
					dici_in	  	: 1.��������
							       0.�����뿪
 @Return				void
**********************************************************************************************************/
void IOOutput_Mode6_Check(u16 dici_index, u16 dici_carnum, u8 dici_in)
{
	u16 carnum = 0;
	
	if (IOOutputStruct[dici_index].CarNumState == 0) {						//��ʼ��ID������
		IOOutputStruct[dici_index].CarNumState = dici_carnum;					//��¼������״ֵ̬
		if (dici_in == 1) {
			if (dici_index < OUTPUT_NUM) {
				GPIO_SetBits(OUTPUT_TYPE[dici_index],  OUTPUT_PIN[dici_index]);
			}
			LestcCarInSetStatus(&LestcPacketData, dici_index);
			IOOutputStruct[dici_index].IOLevel = 1;							//��ƽ��1
			IOOutputStruct[dici_index].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
		}
		else {
			if (dici_index < OUTPUT_NUM) {
				GPIO_ResetBits(OUTPUT_TYPE[dici_index],  OUTPUT_PIN[dici_index]);
			}
			LestcCarOutSetStatus(&LestcPacketData, dici_index);
			IOOutputStruct[dici_index].IOLevel = 0;							//��ƽ��0
			IOOutputStruct[dici_index].IOHighUseTime = 0;					//�ߵ�ƽʱ����0
		}
	}
	else {															//�ѳ�ʼ�����
		if (dici_carnum > IOOutputStruct[dici_index].CarNumState) {				//���㳵����ֵ
			if ((dici_carnum - IOOutputStruct[dici_index].CarNumState) < IOOutputNUM) {
				carnum = dici_carnum - IOOutputStruct[dici_index].CarNumState;
			}
		}
		else if (dici_carnum < IOOutputStruct[dici_index].CarNumState) {
			if ((65535 - IOOutputStruct[dici_index].CarNumState + dici_carnum) < IOOutputNUM) {
				carnum = 65535 - IOOutputStruct[dici_index].CarNumState + dici_carnum;
			}
		}
		IOOutputStruct[dici_index].CarNumState = dici_carnum;					//��¼������״ֵ̬
		
		/* �жϸõش��Ƿ����ڲ������� */
		if (IOOutputStruct[dici_index].SupplyingFlag == 0) {					//��������
			if (carnum != 0) {											//��Ҫ���䳵����
				if (dici_in == 1) {
					IOOutputStruct[dici_index].SupplyCarNum = 1;				//��ȡ��Ҫ�����ĳ�����
					IOOutputStruct[dici_index].CarInOrOut = 1;				//������
					IOOutputStruct[dici_index].CarEvenOrOdd = 0;				//������
				}
				else {
					IOOutputStruct[dici_index].SupplyCarNum = 1;				//��ȡ��Ҫ�����ĳ�����
					IOOutputStruct[dici_index].CarInOrOut = 0;				//�����
					IOOutputStruct[dici_index].CarEvenOrOdd = 1;				//ż����
				}
				IOOutputStruct[dici_index].SupplyedIOLevel = 0;				//���䳵��������������͵�ƽ
				IOOutputStruct[dici_index].SupplyingFlag = 1;				//��־���������ڲ�����
			}
			else {													//�����carnum����
				IOOutputStruct[dici_index].SupplyCarNum = 1;					//��ȡ��Ҫ�����ĳ�����
				IOOutputStruct[dici_index].CarInOrOut = 0;					//�����
				IOOutputStruct[dici_index].CarEvenOrOdd = 0;					//������
				IOOutputStruct[dici_index].SupplyedIOLevel = 0;				//���䳵��������������͵�ƽ
				IOOutputStruct[dici_index].SupplyingFlag = 1;				//��־���������ڲ�����
			}
		}
		else {																	//���ڲ���
			if (carnum != 0) {														//��Ҫ���䳵����
				if (dici_in == 1) {
					IOOutputStruct[dici_index].SupplyingCarNum = 1;						//��ȡ���ڲ������յ��ĳ�����
					if (IOOutputStruct[dici_index].SupplyingCarNum >= (IOOutputNUM * 2)) {
						IOOutputStruct[dici_index].SupplyingCarNum -= (carnum * 2 - 1);
					}
					else {
						IOOutputStruct[dici_index].SupplyingCarInOrOut = 1;				//������
					}
				}
				else {
					IOOutputStruct[dici_index].SupplyingCarNum = 1;						//��ȡ���ڲ������յ��ĳ�����
					if (IOOutputStruct[dici_index].SupplyingCarNum >= (IOOutputNUM * 2)) {
						IOOutputStruct[dici_index].SupplyingCarNum -= (carnum * 2);
					}
					else {
						IOOutputStruct[dici_index].SupplyingCarInOrOut = 0;				//�����
					}
				}
			}
			else {																//�����carnum����
				IOOutputStruct[dici_index].SupplyingCarNum = 1;
				if (IOOutputStruct[dici_index].SupplyingCarNum >= (IOOutputNUM * 2)) {
					IOOutputStruct[dici_index].SupplyingCarNum -= 1;
				}
				else {
					IOOutputStruct[dici_index].SupplyingCarInOrOut = 0;					//�����
				}
			}
			
			IOOutputStruct[dici_index].SupplyedIOLevel = 0;								//���䳵��������������͵�ƽ
		}
	}
}

/**********************************************************************************************************
 @Function			void IOOutput_Mode6_Supplying(void)
 @Description			IO�����ʱ������ 1msִ��һ�� (�����ʽ6 : ��������,�뿪ʱ������̶�ʱ��(������))
 @Input				void
 @Return				void
**********************************************************************************************************/
void IOOutput_Mode6_Supplying(void)
{
	u8 i = 0;
	u16 hightIOMaxTime = 0;
	
	for (i = 0; i < IOOutputMAX; i++) {
		if (IOOutputStruct[i].OutputID != 0) {												//ID��Ϊ0����
			
			/* �ߵ�ƽ��ʱ���� */
			if (IOOutputStruct[i].IOLevel == 1) {
				IOOutputStruct[i].IOHighUseTime += 1;
				if (param_recv.vi_output_high_time > param_recv.vo_output_high_time) {
					hightIOMaxTime = param_recv.vi_output_high_time;
				}
				else {
					hightIOMaxTime = param_recv.vo_output_high_time;
				}
				
				if (IOOutputStruct[i].IOHighUseTime >= (hightIOMaxTime * 1000)) {
					if (i < OUTPUT_NUM) {
						GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
					}
					LestcCarOutSetStatus(&LestcPacketData, i);
					IOOutputStruct[i].IOLevel = 0;										//��ƽ��0
					IOOutputStruct[i].IOHighUseTime = 0;									//�ߵ�ƽʱ����0
				}
			}
			
			/* ���������� */
			if (IOOutputStruct[i].SupplyingFlag != 0) {
				
				if (IOOutputStruct[i].CarInOrOut == 1) {									//���һ��Ϊ������
					if (IOOutputStruct[i].CarEvenOrOdd == 0) {								//������
						if ((IOOutputStruct[i].SupplyCarNum % 2) == 1) {
							if (IOOutputStruct[i].SupplyUseTime == 0) {
								if (i < OUTPUT_NUM) {
									GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarInSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 1;							//��ƽ��1
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							else if (IOOutputStruct[i].SupplyUseTime == (param_recv.vi_output_high_time * 20)) {
								if (i < OUTPUT_NUM) {
									GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarOutSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 0;							//��ƽ��0
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							
							IOOutputStruct[i].SupplyUseTime += 1;
							
							if (IOOutputStruct[i].SupplyUseTime >= ((param_recv.vi_output_high_time * 20) + (param_recv.output_low_min * 20) + 1)) {
								IOOutputStruct[i].SupplyUseTime = 0;
								IOOutputStruct[i].SupplyCarNum -= 1;						//������������һ
								if (IOOutputStruct[i].SupplyCarNum == 0) {					//�������������
									IOOutputStruct[i].SupplyCarNum = 0;					//��ղ���������
									
									if (IOOutputStruct[i].SupplyingCarNum != 0) {			//�������յ�������
										IOOutputStruct[i].SupplyCarNum = IOOutputStruct[i].SupplyingCarNum;		//��ȡ��Ҫ�����ĳ�����
										IOOutputStruct[i].CarInOrOut = IOOutputStruct[i].SupplyingCarInOrOut;
										if (IOOutputStruct[i].SupplyingCarNum % 2 == 1) {
											IOOutputStruct[i].CarEvenOrOdd = 0;
										}
										else {
											IOOutputStruct[i].CarEvenOrOdd = 1;
										}
										IOOutputStruct[i].SupplyingCarNum = 0;
										IOOutputStruct[i].SupplyingFlag = 1;				//��־���������ڲ�����
									}
									else {
										IOOutputStruct[i].SupplyingFlag = 0;				//����������0
										if (i < OUTPUT_NUM) {
											GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
										}
										LestcCarOutSetStatus(&LestcPacketData, i);
										IOOutputStruct[i].IOLevel = 0;					//��ƽ��0
										IOOutputStruct[i].IOHighUseTime = 0;				//�ߵ�ƽʱ����0
									}
								}
								else if (IOOutputStruct[i].SupplyCarNum > (IOOutputNUM * 2)) {	//�����貹��������
									IOOutputStruct[i].SupplyCarNum = (IOOutputNUM * 2);		//������󲹷���
								}
							}
						}
						else {
							if (IOOutputStruct[i].SupplyUseTime == 0) {
								if (i < OUTPUT_NUM) {
									GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarInSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 1;							//��ƽ��1
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							else if (IOOutputStruct[i].SupplyUseTime == (param_recv.vo_output_high_time * 20)) {
								if (i < OUTPUT_NUM) {
									GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarOutSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 0;							//��ƽ��0
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							
							IOOutputStruct[i].SupplyUseTime += 1;
							
							if (IOOutputStruct[i].SupplyUseTime >= ((param_recv.vo_output_high_time * 20) + (param_recv.output_low_min * 20) + 1)) {
								IOOutputStruct[i].SupplyUseTime = 0;
								IOOutputStruct[i].SupplyCarNum -= 1;						//������������һ
								if (IOOutputStruct[i].SupplyCarNum == 0) {					//�������������
									IOOutputStruct[i].SupplyCarNum = 0;					//��ղ���������
									
									if (IOOutputStruct[i].SupplyingCarNum != 0) {			//�������յ�������
										IOOutputStruct[i].SupplyCarNum = IOOutputStruct[i].SupplyingCarNum;		//��ȡ��Ҫ�����ĳ�����
										IOOutputStruct[i].CarInOrOut = IOOutputStruct[i].SupplyingCarInOrOut;
										if (IOOutputStruct[i].SupplyingCarNum % 2 == 1) {
											IOOutputStruct[i].CarEvenOrOdd = 0;
										}
										else {
											IOOutputStruct[i].CarEvenOrOdd = 1;
										}
										IOOutputStruct[i].SupplyingCarNum = 0;
										IOOutputStruct[i].SupplyingFlag = 1;				//��־���������ڲ�����
									}
									else {
										IOOutputStruct[i].SupplyingFlag = 0;				//����������0
										if (i < OUTPUT_NUM) {
											GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
										}
										LestcCarOutSetStatus(&LestcPacketData, i);
										IOOutputStruct[i].IOLevel = 0;					//��ƽ��0
										IOOutputStruct[i].IOHighUseTime = 0;				//�ߵ�ƽʱ����0
									}
								}
								else if (IOOutputStruct[i].SupplyCarNum > (IOOutputNUM * 2)) {	//�����貹��������
									IOOutputStruct[i].SupplyCarNum = (IOOutputNUM * 2);		//������󲹷���
								}
							}
						}
					}
					else {															//ż����
						if ((IOOutputStruct[i].SupplyCarNum % 2) == 1) {
							if (IOOutputStruct[i].SupplyUseTime == 0) {
								if (i < OUTPUT_NUM) {
									GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarInSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 1;							//��ƽ��1
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							else if (IOOutputStruct[i].SupplyUseTime == (param_recv.vi_output_high_time * 20)) {
								if (i < OUTPUT_NUM) {
									GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarOutSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 0;							//��ƽ��0
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							
							IOOutputStruct[i].SupplyUseTime += 1;
							
							if (IOOutputStruct[i].SupplyUseTime >= ((param_recv.vi_output_high_time * 20) + (param_recv.output_low_min * 20) + 1)) {
								IOOutputStruct[i].SupplyUseTime = 0;
								IOOutputStruct[i].SupplyCarNum -= 1;						//������������һ
								if (IOOutputStruct[i].SupplyCarNum == 0) {					//�������������
									IOOutputStruct[i].SupplyCarNum = 0;					//��ղ���������
									
									if (IOOutputStruct[i].SupplyingCarNum != 0) {			//�������յ�������
										IOOutputStruct[i].SupplyCarNum = IOOutputStruct[i].SupplyingCarNum;		//��ȡ��Ҫ�����ĳ�����
										IOOutputStruct[i].CarInOrOut = IOOutputStruct[i].SupplyingCarInOrOut;
										if (IOOutputStruct[i].SupplyingCarNum % 2 == 1) {
											IOOutputStruct[i].CarEvenOrOdd = 0;
										}
										else {
											IOOutputStruct[i].CarEvenOrOdd = 1;
										}
										IOOutputStruct[i].SupplyingCarNum = 0;
										IOOutputStruct[i].SupplyingFlag = 1;				//��־���������ڲ�����
									}
									else {
										IOOutputStruct[i].SupplyingFlag = 0;				//����������0
										if (i < OUTPUT_NUM) {
											GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
										}
										LestcCarOutSetStatus(&LestcPacketData, i);
										IOOutputStruct[i].IOLevel = 0;					//��ƽ��0
										IOOutputStruct[i].IOHighUseTime = 0;				//�ߵ�ƽʱ����0
									}
								}
								else if (IOOutputStruct[i].SupplyCarNum > (IOOutputNUM * 2)) {	//�����貹��������
									IOOutputStruct[i].SupplyCarNum = (IOOutputNUM * 2);		//������󲹷���
								}
							}
						}
						else {
							if (IOOutputStruct[i].SupplyUseTime == 0) {
								if (i < OUTPUT_NUM) {
									GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarInSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 1;							//��ƽ��1
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							else if (IOOutputStruct[i].SupplyUseTime == (param_recv.vo_output_high_time * 20)) {
								if (i < OUTPUT_NUM) {
									GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarOutSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 0;							//��ƽ��0
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							
							IOOutputStruct[i].SupplyUseTime += 1;
							
							if (IOOutputStruct[i].SupplyUseTime >= ((param_recv.vo_output_high_time * 20) + (param_recv.output_low_min * 20) + 1)) {
								IOOutputStruct[i].SupplyUseTime = 0;
								IOOutputStruct[i].SupplyCarNum -= 1;						//������������һ
								if (IOOutputStruct[i].SupplyCarNum == 0) {					//�������������
									IOOutputStruct[i].SupplyCarNum = 0;					//��ղ���������
									
									if (IOOutputStruct[i].SupplyingCarNum != 0) {			//�������յ�������
										IOOutputStruct[i].SupplyCarNum = IOOutputStruct[i].SupplyingCarNum;		//��ȡ��Ҫ�����ĳ�����
										IOOutputStruct[i].CarInOrOut = IOOutputStruct[i].SupplyingCarInOrOut;
										if (IOOutputStruct[i].SupplyingCarNum % 2 == 1) {
											IOOutputStruct[i].CarEvenOrOdd = 0;
										}
										else {
											IOOutputStruct[i].CarEvenOrOdd = 1;
										}
										IOOutputStruct[i].SupplyingCarNum = 0;
										IOOutputStruct[i].SupplyingFlag = 1;				//��־���������ڲ�����
									}
									else {
										IOOutputStruct[i].SupplyingFlag = 0;				//����������0
										if (i < OUTPUT_NUM) {
											GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
										}
										LestcCarOutSetStatus(&LestcPacketData, i);
										IOOutputStruct[i].IOLevel = 0;					//��ƽ��0
										IOOutputStruct[i].IOHighUseTime = 0;				//�ߵ�ƽʱ����0
									}
								}
								else if (IOOutputStruct[i].SupplyCarNum > (IOOutputNUM * 2)) {	//�����貹��������
									IOOutputStruct[i].SupplyCarNum = (IOOutputNUM * 2);		//������󲹷���
								}
							}
						}
					}
				}
				else {													//���һ��Ϊ�����
					if (IOOutputStruct[i].CarEvenOrOdd == 0) {					//������
						if ((IOOutputStruct[i].SupplyCarNum % 2) == 1) {
							if (IOOutputStruct[i].SupplyUseTime == 0) {
								if (i < OUTPUT_NUM) {
									GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarInSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 1;							//��ƽ��1
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							else if (IOOutputStruct[i].SupplyUseTime == (param_recv.vo_output_high_time * 20)) {
								if (i < OUTPUT_NUM) {
									GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarOutSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 0;							//��ƽ��0
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							
							IOOutputStruct[i].SupplyUseTime += 1;
							
							if (IOOutputStruct[i].SupplyUseTime >= ((param_recv.vo_output_high_time * 20) + (param_recv.output_low_min * 20) + 1)) {
								IOOutputStruct[i].SupplyUseTime = 0;
								IOOutputStruct[i].SupplyCarNum -= 1;						//������������һ
								if (IOOutputStruct[i].SupplyCarNum == 0) {					//�������������
									IOOutputStruct[i].SupplyCarNum = 0;					//��ղ���������
									
									if (IOOutputStruct[i].SupplyingCarNum != 0) {			//�������յ�������
										IOOutputStruct[i].SupplyCarNum = IOOutputStruct[i].SupplyingCarNum;		//��ȡ��Ҫ�����ĳ�����
										IOOutputStruct[i].CarInOrOut = IOOutputStruct[i].SupplyingCarInOrOut;
										if (IOOutputStruct[i].SupplyingCarNum % 2 == 1) {
											IOOutputStruct[i].CarEvenOrOdd = 0;
										}
										else {
											IOOutputStruct[i].CarEvenOrOdd = 1;
										}
										IOOutputStruct[i].SupplyingCarNum = 0;
										IOOutputStruct[i].SupplyingFlag = 1;				//��־���������ڲ�����
									}
									else {
										IOOutputStruct[i].SupplyingFlag = 0;				//����������0
										if (i < OUTPUT_NUM) {
											GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
										}
										LestcCarOutSetStatus(&LestcPacketData, i);
										IOOutputStruct[i].IOLevel = 0;					//��ƽ��0
										IOOutputStruct[i].IOHighUseTime = 0;				//�ߵ�ƽʱ����0
									}
								}
								else if (IOOutputStruct[i].SupplyCarNum > (IOOutputNUM * 2)) {	//�����貹��������
									IOOutputStruct[i].SupplyCarNum = (IOOutputNUM * 2);		//������󲹷���
								}
							}
						}
						else {
							if (IOOutputStruct[i].SupplyUseTime == 0) {
								if (i < OUTPUT_NUM) {
									GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarInSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 1;							//��ƽ��1
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							else if (IOOutputStruct[i].SupplyUseTime == (param_recv.vi_output_high_time * 20)) {
								if (i < OUTPUT_NUM) {
									GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarOutSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 0;							//��ƽ��0
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							
							IOOutputStruct[i].SupplyUseTime += 1;
							
							if (IOOutputStruct[i].SupplyUseTime >= ((param_recv.vi_output_high_time * 20) + (param_recv.output_low_min * 20) + 1)) {
								IOOutputStruct[i].SupplyUseTime = 0;
								IOOutputStruct[i].SupplyCarNum -= 1;						//������������һ
								if (IOOutputStruct[i].SupplyCarNum == 0) {					//�������������
									IOOutputStruct[i].SupplyCarNum = 0;					//��ղ���������
									
									if (IOOutputStruct[i].SupplyingCarNum != 0) {			//�������յ�������
										IOOutputStruct[i].SupplyCarNum = IOOutputStruct[i].SupplyingCarNum;		//��ȡ��Ҫ�����ĳ�����
										IOOutputStruct[i].CarInOrOut = IOOutputStruct[i].SupplyingCarInOrOut;
										if (IOOutputStruct[i].SupplyingCarNum % 2 == 1) {
											IOOutputStruct[i].CarEvenOrOdd = 0;
										}
										else {
											IOOutputStruct[i].CarEvenOrOdd = 1;
										}
										IOOutputStruct[i].SupplyingCarNum = 0;
										IOOutputStruct[i].SupplyingFlag = 1;				//��־���������ڲ�����
									}
									else {
										IOOutputStruct[i].SupplyingFlag = 0;				//����������0
										if (i < OUTPUT_NUM) {
											GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
										}
										LestcCarOutSetStatus(&LestcPacketData, i);
										IOOutputStruct[i].IOLevel = 0;					//��ƽ��0
										IOOutputStruct[i].IOHighUseTime = 0;				//�ߵ�ƽʱ����0
									}
								}
								else if (IOOutputStruct[i].SupplyCarNum > (IOOutputNUM * 2)) {	//�����貹��������
									IOOutputStruct[i].SupplyCarNum = (IOOutputNUM * 2);		//������󲹷���
								}
							}
						}
					}
					else {												//ż����
						if ((IOOutputStruct[i].SupplyCarNum % 2) == 1) {
							if (IOOutputStruct[i].SupplyUseTime == 0) {
								if (i < OUTPUT_NUM) {
									GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarInSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 1;							//��ƽ��1
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							else if (IOOutputStruct[i].SupplyUseTime == (param_recv.vo_output_high_time * 20)) {
								if (i < OUTPUT_NUM) {
									GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarOutSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 0;							//��ƽ��0
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							
							IOOutputStruct[i].SupplyUseTime += 1;
							
							if (IOOutputStruct[i].SupplyUseTime >= ((param_recv.vo_output_high_time * 20) + (param_recv.output_low_min * 20) + 1)) {
								IOOutputStruct[i].SupplyUseTime = 0;
								IOOutputStruct[i].SupplyCarNum -= 1;						//������������һ
								if (IOOutputStruct[i].SupplyCarNum == 0) {					//�������������
									IOOutputStruct[i].SupplyCarNum = 0;					//��ղ���������
									
									if (IOOutputStruct[i].SupplyingCarNum != 0) {			//�������յ�������
										IOOutputStruct[i].SupplyCarNum = IOOutputStruct[i].SupplyingCarNum;		//��ȡ��Ҫ�����ĳ�����
										IOOutputStruct[i].CarInOrOut = IOOutputStruct[i].SupplyingCarInOrOut;
										if (IOOutputStruct[i].SupplyingCarNum % 2 == 1) {
											IOOutputStruct[i].CarEvenOrOdd = 0;
										}
										else {
											IOOutputStruct[i].CarEvenOrOdd = 1;
										}
										IOOutputStruct[i].SupplyingCarNum = 0;
										IOOutputStruct[i].SupplyingFlag = 1;				//��־���������ڲ�����
									}
									else {
										IOOutputStruct[i].SupplyingFlag = 0;				//����������0
										if (i < OUTPUT_NUM) {
											GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
										}
										LestcCarOutSetStatus(&LestcPacketData, i);
										IOOutputStruct[i].IOLevel = 0;					//��ƽ��0
										IOOutputStruct[i].IOHighUseTime = 0;				//�ߵ�ƽʱ����0
									}
								}
								else if (IOOutputStruct[i].SupplyCarNum > (IOOutputNUM * 2)) {	//�����貹��������
									IOOutputStruct[i].SupplyCarNum = (IOOutputNUM * 2);		//������󲹷���
								}
							}
						}
						else {
							if (IOOutputStruct[i].SupplyUseTime == 0) {
								if (i < OUTPUT_NUM) {
									GPIO_SetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarInSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 1;							//��ƽ��1
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							else if (IOOutputStruct[i].SupplyUseTime == (param_recv.vi_output_high_time * 20)) {
								if (i < OUTPUT_NUM) {
									GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
								}
								LestcCarOutSetStatus(&LestcPacketData, i);
								IOOutputStruct[i].IOLevel = 0;							//��ƽ��0
								IOOutputStruct[i].IOHighUseTime = 0;						//�ߵ�ƽʱ����0
							}
							
							IOOutputStruct[i].SupplyUseTime += 1;
							
							if (IOOutputStruct[i].SupplyUseTime >= ((param_recv.vi_output_high_time * 20) + (param_recv.output_low_min * 20) + 1)) {
								IOOutputStruct[i].SupplyUseTime = 0;
								IOOutputStruct[i].SupplyCarNum -= 1;						//������������һ
								if (IOOutputStruct[i].SupplyCarNum == 0) {					//�������������
									IOOutputStruct[i].SupplyCarNum = 0;					//��ղ���������
									
									if (IOOutputStruct[i].SupplyingCarNum != 0) {			//�������յ�������
										IOOutputStruct[i].SupplyCarNum = IOOutputStruct[i].SupplyingCarNum;		//��ȡ��Ҫ�����ĳ�����
										IOOutputStruct[i].CarInOrOut = IOOutputStruct[i].SupplyingCarInOrOut;
										if (IOOutputStruct[i].SupplyingCarNum % 2 == 1) {
											IOOutputStruct[i].CarEvenOrOdd = 0;
										}
										else {
											IOOutputStruct[i].CarEvenOrOdd = 1;
										}
										IOOutputStruct[i].SupplyingCarNum = 0;
										IOOutputStruct[i].SupplyingFlag = 1;				//��־���������ڲ�����
									}
									else {
										IOOutputStruct[i].SupplyingFlag = 0;				//����������0
										if (i < OUTPUT_NUM) {
											GPIO_ResetBits(OUTPUT_TYPE[i],  OUTPUT_PIN[i]);
										}
										LestcCarOutSetStatus(&LestcPacketData, i);
										IOOutputStruct[i].IOLevel = 0;					//��ƽ��0
										IOOutputStruct[i].IOHighUseTime = 0;				//�ߵ�ƽʱ����0
									}
								}
								else if (IOOutputStruct[i].SupplyCarNum > (IOOutputNUM * 2)) {	//�����貹��������
									IOOutputStruct[i].SupplyCarNum = (IOOutputNUM * 2);		//������󲹷���
								}
							}
						}
					}
				}
			}
		}
	}
}


/**********************************************************************************************************
 @Function			void IOOutput_Event_IRQn(void)
 @Description			IOOutput�жϴ�����
 @Input				outputid
 @Return				void
**********************************************************************************************************/
void IOOutput_Event_IRQn(void)
{
	switch (param_recv.output_mode)
	{
	//�����ʽ0 : ���泵�����
	case 0:
		if (param_recv.handle_lost == 1) {								//�ж��Ƿ���Ҫ�Զ�������
			iooutput_dev.Mode0Supplying();							//IO����������������� ���泵�����
		}
		break;
	//�����ʽ1 : ������������̶�ʱ��(����)
	case 1:
		iooutput_dev.Mode1Supplying();								//IO����������������� ������������̶�ʱ��(����)
		break;
	//�����ʽ2 : �����뿪����̶�ʱ��(����)
	case 2:
		iooutput_dev.Mode2Supplying();								//IO����������������� �����뿪����̶�ʱ��(����)
		break;
	//�����ʽ3 : ��������,�뿪ʱ������̶�ʱ��(����)
	case 3:
		iooutput_dev.Mode3Supplying();								//IO����������������� ��������,�뿪ʱ������̶�ʱ��(����)
		break;
	//�����ʽ4 : ������������̶�ʱ��(������)
	case 4:
		iooutput_dev.Mode4Supplying();								//IO����������������� ������������̶�ʱ��(������)
		break;
	//�����ʽ5 : �����뿪����̶�ʱ��(������)
	case 5:
		iooutput_dev.Mode5Supplying();								//IO����������������� �����뿪����̶�ʱ��(������)
		break;
	//�����ʽ6 : ��������,�뿪ʱ������̶�ʱ��(������)
	case 6:
		iooutput_dev.Mode6Supplying();								//IO����������������� ��������,�뿪ʱ������̶�ʱ��(������)
		break;
	//Ĭ�� �����ʽ0 : ���泵�����
	default :
		if (param_recv.handle_lost == 1) {								//�ж��Ƿ���Ҫ�Զ�������
			iooutput_dev.Mode0Supplying();							//IO����������������� ���泵�����
		}
		break;
	}
}

/**********************************************************************************************************
 @Function			void IOOutput_GetOutputID(u16 *outputid)
 @Description			��ȡoutput_ID����˿ںŵ�IOOutput�������ݰ�
 @Input				outputid
 @Return				void
**********************************************************************************************************/
void IOOutput_GetOutputID(u16 *outputid)
{
	u8 i = 0;
	
	for (i = 0; i < IOOutputMAX; i++) {
		IOOutputStruct[i].OutputID = outputid[i];
		IOOutputStruct[i].CarNumState = 0;
	}
}

/********************************************** END OF FLEE **********************************************/
