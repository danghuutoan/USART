/*==============================================================================
**                                  QUADROBO
**                      Proprietary - Copyright (C) 2015
**------------------------------------------------------------------------------
** Supported MCUs      : MCU STM32
** Supported Compilers : ARM GCC
**------------------------------------------------------------------------------
** File name         : retarget.c
**  
** Module name       : QuadRobo
**
** Summary: 
**
**= History ===================================================================
** 00.00.01  04/27/2015  Cuong Chu
** - Creation
==============================================================================*/
/******************************************************************************
**    INCLUDE
******************************************************************************/
#include <stdio.h>
#include "com.h"
/******************************************************************************
**    OPTIONS FOR RETARGET
******************************************************************************/
#define KEIL_ARM
#define ARM_GCC
#define COM_DBG          COM2

/******************************************************************************
**    GLOBAL RETARGET FUNCTIONS
******************************************************************************/
/* retarget for arm gcc */
#if defined (ARM_GCC)

int _read(int file, char *ptr, int len) 
{
  int CharCnt = 0x00;
  while(CharCnt < len) 
  {
      *ptr++ = COM_ReadByte(COM_DBG);
      CharCnt++;
  }
  return CharCnt;  
}

int _write(int file, char *ptr, int len) 
{
    int n;
    for (n = 0; n < len; n++) 
    {
        COM_WriteByte(COM_DBG,(uint8_t)(*ptr++ & (uint16_t)0x01FF));
    }
    return n;
}
#endif

/* retarget for keil ARM */
#if defined (KEIL_ARM)
struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;


int fputc(int c, FILE *f) {
  (COM_WriteByte(COM_DBG,c));
	return  c;
}


int fgetc(FILE *f) {
	int c;
	c = (int)COM_ReadByte(COM_DBG);
  return (c);
}


void _ttywrch(int c) {
  COM_WriteByte(COM_DBG,c);
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}
#endif

