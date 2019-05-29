#ifndef __SOCK_H
#define   __SOCK_H

#include "stm32f10x_lib.h"
#include "socketxunfeiconfig.h"

unsigned long int htonl(unsigned long int h);								//本机字节序转网络字节序(u32)
unsigned long int ntohl(unsigned long int n);								//网络字节序转本机字节序(u32)
unsigned short int htons(unsigned short int h);								//本机字节序转网络字节序(u16)
unsigned short int ntohs(unsigned short int n);								//网络字节序转本机字节序(u16)

#endif /* __SOCK_H */
