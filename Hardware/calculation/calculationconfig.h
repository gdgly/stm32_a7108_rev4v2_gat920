#ifndef __CALCULATION_CONFIG_H
#define   __CALCULATION_CONFIG_H

#include "stm32f10x_lib.h"
#include "platform_config.h"


typedef struct
{
	u16		OutputID;													//�ش�ID��
	
	u16		CarNumState;												//�شű���ĳ�������״ֵ̬
	u16		CarNumProcess;												//һ��ʱ���ڸõشų�����������
	
	u8		Speed_CarinState;											//������״̬��־
	u8		Speed_CaroutState;											//�����״̬��־
	u16		Speed_CarinUseTime;											//������ʹ��ʱ��ms
	u16		Speed_CaroutUseTime;										//�����ʹ��ʱ��ms
	u32		Speed_CarSumTime;											//һ��ʱ���ڸó���������ʹ��ʱ��
	u16		Speed_CarNum;												//һ��ʱ���ڸó�����������
	
	u8		Occupancy_CarState;											//����״̬��־λ 1 : �� 0 : ��
	u32		Occupancy_CarUseTime;										//����ռ��ʱ��
	u16		Occupancy_CarNumState;										//���ͬ��(ͬ��������,�����CarNumֵ��ͬ)
	u32		Occupancy_CarSumTime;										//һ��ʱ���ڸó���������ռ��ʱ��
	u16		Occupancy_CarSumNum;										//һ��ʱ���ڸó�����������
	
	u8		Headtime_CarState;											//��ͷʱ��ʱ���ۼ�����־λ 1 : �� 0 : ��
	u32		Headtime_CarUseTime;										//���μ��ʱ��
	u16		Headtime_CarNumState;										//��������������ֵ
	u32		Headtime_CarSumTime;										//һ��ʱ���ڸó��������ܼ��ʱ��
	u16		Headtime_CarSumNum;											//һ��ʱ���ڸó�����������
	
	u8		Interval_CarState1;
	u32		Interval_CarUseTime1;
	u16		Interval_CarNumState1;
	u32		Interval_CarSumTime1;
	u16		Interval_CarSumNum1;
	
	u8		Interval_CarState2;
	u32		Interval_CarUseTime2;
	u16		Interval_CarNumState2;
	u32		Interval_CarSumTime2;
	u16		Interval_CarSumNum2;
}Calculation_Data_Packet;												//�������ݰ�

/* calculation�ⲿ�ӿ����ݶ�ȡ���ƹ����� */
struct _m_calculation_dev
{
	void		(*ReadOutputID)(u16 *outputid);								//��ȡoutput_ID����˿ڵĲ�����Calculation�������ݰ�
	
	void		(*GetVolume)(u8 *buf);										//���ݵشŷ��͵����ݰ����㳵������
	u16		(*ReadVolume)(u16 outputid);									//��ȡ�ó���������ֵ
	
	void		(*GetAvgSpeed)(u8 *buf);										//���ݵشŷ��͵����ݰ�����ƽ���ٶ�
	float	(*ReadAvgSpeed)(u16 outputid);								//��ȡ�ó���ƽ���ٶ�ֵ
	
	void		(*GetAvgOccupancy)(u8 *buf);									//���ݵشŷ��͵����ݰ�����ƽ��ռ��ʱ��
	u16		(*ReadAvgOccupancy)(u16 outputid);								//��ȡ�ó���ƽ��ռ��ʱ��ֵ
	u32		(*ReadAvgOccupancyExtend)(u16 outputid);						//��ȡ�ó���ƽ��ռ��ʱ��ֵ(����֧������Ϊ4�ֽ�int)
	
	void		(*GetAvgHeadTime)(u8 *buf);									//���ݵشŷ��͵����ݰ�����ƽ����ͷʱ��
	u16		(*ReadAvgHeadTime)(u16 outputid);								//��ȡ�ó���ƽ����ͷʱ��
	u32		(*ReadAvgHeadTimeExtend)(u16 outputid);							//��ȡ�ó���ƽ����ͷʱ��(����֧������Ϊ4�ֽ�int)
	
	void		(*GetAvgInterval)(u8 *buf);
	u32		(*ReadAvgInterval)(u16 outputid, u32* sumTime1, u32* sumTime2);
};


extern Calculation_Data_Packet			CalculationDataPacket[OUTPUT_MAX];		//���ݼ����
extern struct _m_calculation_dev 			calculation_dev;					//calculation�ⲿ�ӿ����ݶ�ȡ����������

void CALCULATION_GetOutputID(u16 *outputid);									//��ȡoutput_ID����˿ڵĲ�����Calculation�������ݰ�


#endif
