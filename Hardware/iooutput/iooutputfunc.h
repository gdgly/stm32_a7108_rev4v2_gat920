#ifndef __IOOUTPUT_FUNC_H
#define   __IOOUTPUT_FUNC_H

#include "stm32f10x_lib.h"
#include "iooutputconfig.h"


void IOOutput_Mode0_Check(u16 dici_index, u16 dici_carnum, u8 dici_in);			//IO���У������	(�����ʽ0 : ���泵�����)
void IOOutput_Mode0_Supplying(void);										//IO�����ʱ������	(�����ʽ0 : ���泵�����)
void IOOutput_Mode1_Check(u16 dici_index, u16 dici_carnum, u8 dici_in);			//IO���У������	(�����ʽ1 : ������������̶�ʱ��(����))
void IOOutput_Mode1_Supplying(void);										//IO�����ʱ������	(�����ʽ1 : ������������̶�ʱ��(����))
void IOOutput_Mode2_Check(u16 dici_index, u16 dici_carnum, u8 dici_in);			//IO���У������	(�����ʽ2 : �����뿪����̶�ʱ��(����))
void IOOutput_Mode2_Supplying(void);										//IO�����ʱ������	(�����ʽ2 : �����뿪����̶�ʱ��(����))
void IOOutput_Mode3_Check(u16 dici_index, u16 dici_carnum, u8 dici_in);			//IO���У������	(�����ʽ3 : ��������,�뿪ʱ������̶�ʱ��(����))
void IOOutput_Mode3_Supplying(void);										//IO�����ʱ������	(�����ʽ3 : ��������,�뿪ʱ������̶�ʱ��(����))
void IOOutput_Mode4_Check(u16 dici_index, u16 dici_carnum, u8 dici_in);			//IO���У������	(�����ʽ4 : ������������̶�ʱ��(������))
void IOOutput_Mode4_Supplying(void);										//IO�����ʱ������	(�����ʽ4 : ������������̶�ʱ��(������))
void IOOutput_Mode5_Check(u16 dici_index, u16 dici_carnum, u8 dici_in);			//IO���У������	(�����ʽ5 : �����뿪����̶�ʱ��(������))
void IOOutput_Mode5_Supplying(void);										//IO�����ʱ������	(�����ʽ5 : �����뿪����̶�ʱ��(������))
void IOOutput_Mode6_Check(u16 dici_index, u16 dici_carnum, u8 dici_in);			//IO���У������	(�����ʽ6 : ��������,�뿪ʱ������̶�ʱ��(������))
void IOOutput_Mode6_Supplying(void);										//IO�����ʱ������	(�����ʽ6 : ��������,�뿪ʱ������̶�ʱ��(������))

void IOOutput_Event_IRQn(void);											//IOOutput�жϴ�����
void IOOutput_GetOutputID(u16 *outputid);									//��ȡoutput_ID����˿ںŵ�IOOutput�������ݰ�

#endif
