/**
  *********************************************************************************************************
  * @file    sock.c
  * @author  Movebroad -- KK
  * @version V1.0
  * @date    2019-04-22
  * @brief   
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "sock.h"

/* -短整型大小端互换- */
#define BigLittleSwap16(A)		((((unsigned short)(A) & 0xFF00) >> 8) | (((unsigned short)(A) & 0x00FF) << 8))
/* -长整型大小端互换- */
#define BigLittleSwap32(A)		((((unsigned int)(A) & 0xFF000000) >> 24) | \
							 (((unsigned int)(A) & 0x00FF0000) >> 8 ) | \
							 (((unsigned int)(A) & 0x0000FF00) << 8 ) | \
							 (((unsigned int)(A) & 0x000000FF) << 24))

/**********************************************************************************************************
 @Function			static int checkCPUendian(void)
 @Description			checkCPUendian						: 检查CPU大小端模式
 @Input				void
 @Return				1								: 大端
					0								: 小端
**********************************************************************************************************/
static int checkCPUendian(void)
{
	union {
		unsigned long int i;
		unsigned char s[4];
	}c;
	
	c.i = 0x12345678;
	
	return (0x12 == c.s[0]);
}

/**********************************************************************************************************
 @Function			unsigned long int htonl(unsigned long int h)
 @Description			htonl				: 本机字节序转网络字节序(u32)
 @Input				unsigned long int
 @Return				unsigned long int
**********************************************************************************************************/
unsigned long int htonl(unsigned long int h)
{
	return checkCPUendian() ? h : BigLittleSwap32(h);
}

/**********************************************************************************************************
 @Function			unsigned long int ntohl(unsigned long int n)
 @Description			ntohl				: 网络字节序转本机字节序(u32)
 @Input				unsigned long int
 @Return				unsigned long int
**********************************************************************************************************/
unsigned long int ntohl(unsigned long int n)
{
	return checkCPUendian() ? n : BigLittleSwap32(n);
}

/**********************************************************************************************************
 @Function			unsigned short int htons(unsigned short int h)
 @Description			htons				: 本机字节序转网络字节序(u16)
 @Input				unsigned short int
 @Return				unsigned short int
**********************************************************************************************************/
unsigned short int htons(unsigned short int h)
{
	return checkCPUendian() ? h : BigLittleSwap16(h);
}

/**********************************************************************************************************
 @Function			unsigned short int ntohs(unsigned short int n)
 @Description			ntohl				: 网络字节序转本机字节序(u16)
 @Input				unsigned short int
 @Return				unsigned short int
**********************************************************************************************************/
unsigned short int ntohs(unsigned short int n)
{
	return checkCPUendian() ? n : BigLittleSwap16(n);
}

/********************************************** END OF FLEE **********************************************/
