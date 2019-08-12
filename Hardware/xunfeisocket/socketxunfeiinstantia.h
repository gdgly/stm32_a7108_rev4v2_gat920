#ifndef __SOCKET_XUNFEI_TINSTANTIA_H
#define   __SOCKET_XUNFEI_TINSTANTIA_H

#include "stm32f10x_lib.h"
#include "socketxunfeiconfig.h"

#define	SOCKET_XUNFEI_AVGLENGTHMIN		3								//平均车长最小值
#define	SOCKET_XUNFEI_AVGLENGTHSINGLE		5								//单地磁测速默认车长

void SOCKET_Xunfei_GetFillData(SOCKET_Xunfei_ClientsTypeDef* pClient);

int SOCKET_Xunfei_GetDirverWayCode(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen);

char* SOCKET_Xunfei_GetDateTime(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen);

int SOCKET_Xunfei_GetInterval(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen);

int SOCKET_Xunfei_GetVolume(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen);

u16 SOCKET_Xunfei_GetAvgOccupancy(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen);

u16 SOCKET_Xunfei_GetAvgHeadTime(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen);

float SOCKET_Xunfei_GetAvgSpeed(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen);

float SOCKET_Xunfei_GetAvgLength(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen);

#endif
