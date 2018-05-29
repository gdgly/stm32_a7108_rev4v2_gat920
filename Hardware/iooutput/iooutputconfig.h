#ifndef __IOOUTPUT_CONFIG_H
#define   __IOOUTPUT_CONFIG_H

#include "stm32f10x_lib.h"
#include "platform_config.h"

#define	IOOutputMAX	32												//���IO�����
#define	IOOutputNUM	100												//IO�����󲹷���

typedef struct
{
	u16		OutputID;													//����˿�ID��
	u16		CarNumState;												//�شű���ĳ�������״ֵ̬
	u32		IOHighUseTime;												//IO�ߵ�ƽʱ��
	u8		IOLevel;													//IO��ƽֵ
	u8		CarInOrOut;												//������/�����
	
	u8		CarEvenOrOdd;												//��������ż
	u8		SupplyingCarInOrOut;										//�����г�����/�����
	u8		SupplyingFlag;												//�����б�־λ 0.�������� 1.���ڲ���
	u8		SupplyedIOLevel;											//����������ƽֵ
	u32		SupplyCarNum;												//����������
	u32		SupplyingCarNum;											//�������յ�������
	u32		SupplyUseTime;												//������ƽʱ��
}IOOutputTypedef;														//IO������ƽṹ��


/* IOOutput�ⲿ�ӿڿ��ƹ����� */
struct _m_iooutput_dev
{
	u8		EventIRQnFlag;												//�жϲ�����־
	
	void		(*Mode0IOCheck)(u16, u16, u8);								//IO���У������	(�����ʽ0 : ���泵�����)
	void		(*Mode0Supplying)(void);										//IO�����ʱ������	(�����ʽ0 : ���泵�����)
	void		(*Mode1IOCheck)(u16, u16, u8);								//IO���У������	(�����ʽ1 : ������������̶�ʱ��(����))
	void		(*Mode1Supplying)(void);										//IO�����ʱ������	(�����ʽ1 : ������������̶�ʱ��(����))
	void		(*Mode2IOCheck)(u16, u16, u8);								//IO���У������	(�����ʽ2 : �����뿪����̶�ʱ��(����))
	void		(*Mode2Supplying)(void);										//IO�����ʱ������	(�����ʽ2 : �����뿪����̶�ʱ��(����))
	void		(*Mode3IOCheck)(u16, u16, u8);								//IO���У������	(�����ʽ3 : ��������,�뿪ʱ������̶�ʱ��(����))
	void		(*Mode3Supplying)(void);										//IO�����ʱ������	(�����ʽ3 : ��������,�뿪ʱ������̶�ʱ��(����))
	void		(*Mode4IOCheck)(u16, u16, u8);								//IO���У������	(�����ʽ4 : ������������̶�ʱ��(������))
	void		(*Mode4Supplying)(void);										//IO�����ʱ������	(�����ʽ4 : ������������̶�ʱ��(������))
	void		(*Mode5IOCheck)(u16, u16, u8);								//IO���У������	(�����ʽ5 : �����뿪����̶�ʱ��(������))
	void		(*Mode5Supplying)(void);										//IO�����ʱ������	(�����ʽ5 : �����뿪����̶�ʱ��(������))
	void		(*Mode6IOCheck)(u16, u16, u8);								//IO���У������	(�����ʽ6 : ��������,�뿪ʱ������̶�ʱ��(������))
	void		(*Mode6Supplying)(void);										//IO�����ʱ������	(�����ʽ6 : ��������,�뿪ʱ������̶�ʱ��(������))
	
	void		(*EventIRQn)(void);											//IOOutput�жϴ�����
	void		(*ReadOutputID)(u16 *outputid);								//��ȡoutput_ID����˿ںŵ�IOOutput�������ݰ�
};

extern struct _m_iooutput_dev iooutput_dev;

#endif
