#ifndef __RTC_H
#define   __RTC_H

#include "stm32f10x_lib.h"
#include "rtc_date.h"
#include "stdint.h"

extern struct rtc_time systemtime;

void RTC_Time_Init(struct rtc_time *tm);							//检查并配置RTC
void RTC_Config(void);											//配置RTC
void Time_Adjust(struct rtc_time *tm);								//时间调节
void RTC_Time_Adjust(u8 year, u8 mon, u8 day, u8 hous, u8 min, u8 sec);	//时间调节

int RTC_Time_GetDateYear(void);
int RTC_Time_GetDateMon(void);
int RTC_Time_GetDateDay(void);

int RTC_Time_GetTimeHour(void);
int RTC_Time_GetTimeMin(void);
int RTC_Time_GetTimeSec(void);

unsigned int RTC_Time_GetTime(u8* year, u8* mon, u8* day, u8* hous, u8* min, u8* sec);

#endif /* RTC_H */
