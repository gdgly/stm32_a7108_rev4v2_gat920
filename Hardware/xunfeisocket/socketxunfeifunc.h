#ifndef __SOCKET_XUNFEI_FUNC_H
#define   __SOCKET_XUNFEI_FUNC_H

#include "stm32f10x_lib.h"
#include "socketxunfeiconfig.h"

unsigned int SOCKET_Xunfei_GetNextMsgId(SOCKET_Xunfei_ClientsTypeDef* pClient);

SOCKET_Xunfei_StatusTypeDef SOCKET_Xunfei_Serialize_LoginRequest(SOCKET_Xunfei_ClientsTypeDef* pClient, const char* account, const char* passwd);
SOCKET_Xunfei_StatusTypeDef SOCKET_Xunfei_Deserialize_LoginRequest(SOCKET_Xunfei_ClientsTypeDef* pClient, char* buf, int len);

SOCKET_Xunfei_StatusTypeDef SOCKET_Xunfei_Serialize_LenDataRequest(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 outputid, u16 inlen);
SOCKET_Xunfei_StatusTypeDef SOCKET_Xunfei_Deserialize_LenDataRequest(SOCKET_Xunfei_ClientsTypeDef* pClient, char* buf, int len);

SOCKET_Xunfei_StatusTypeDef SOCKET_Xunfei_GetOutputID(SOCKET_Xunfei_ClientsTypeDef* pClient, u16 *outputid);

#endif
