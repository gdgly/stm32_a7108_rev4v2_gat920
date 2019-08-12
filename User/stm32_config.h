// BYTE type definition
#ifndef _STM32_CONFIG_H
#define _STM32_CONFIG_H

#include "stm32f10x_type.h"

extern void GPIO_Configuration(void);
extern void TIM_Configuration(void);
extern void Delay(vu32 nCount);
extern void delay_1ms(u32 nCount);
extern void delay_5us(u8 count);
extern void EXTI_Configuration(void);
extern void hand_RTdata(void);
extern void hand_config(u8* buf);
extern void hand_IOOutput(u8* buf);
extern void hand_HeartBeatOutput(u8* buf);
extern void param_save_to_flash(void);

void MAX823_IWDGInitEnable(void);											//��ʼ��MAX823Ӳ�����Ź�
void MAX823_IWDGDisable(void);											//�ر�MAX823
void MAX823_IWDGReloadCounter(void);										//MAX823Ӳ�����Ź�ι��

#endif   /* _STM32_CONFIG_H */
