/*==============================================================================
**                                  QUADROBO
**                      Proprietary - Copyright (C) 2014
**------------------------------------------------------------------------------
** Supported MCUs      : MCU STM32
** Supported Compilers : ARM GCC
**------------------------------------------------------------------------------
** File name         : COM.h
**  
** Module name       : COM interface
**
** Summary: Header file of COM driver
**
**= History ===================================================================
** 00.00.01  14/1/2014  Cuong Chu
**           - Creation
** 00.00.02  29/7/2014  Cuong Chu
**           - Added COM4,COM5 
** 00.00.02  06/8/2014  Cuong Chu
**           - Added USART2 remap option
==============================================================================*/
#ifndef __COM_H__
#define __COM_H__
/******************************************************************************/
/* INCLUSIONS                                                                 */
/******************************************************************************/
#include "stdint.h"
/******************************************************************************/
/* DEFINITION OF CONST                                                        */
/******************************************************************************/
/* switch ON */
#ifndef STD_ON
#define STD_ON 1
#endif
/* switch OFF */
#ifndef STD_OFF
#define STD_OFF 0
#endif

/* USART1 Configure */
/* remap  */
#define COM_USART1_REMAP              STD_ON
#define COM_USART1_LB_DBG             STD_OFF

/* USART2 Configure*/
/* remap */
#define COM_USART2_REMAP              STD_ON
#define COM_USART2_LB_DBG             STD_OFF

/* USART3 Configure*/
/* remap */
#define COM_USART3_REMAP              STD_OFF
#define COM_USART3_LB_DBG             STD_OFF

/* USART4 Configure*/
/* remap */
#define COM_USART4_REMAP              STD_OFF
#define COM_USART4_LB_DBG             STD_OFF

/* USART5 Configure*/
/* remap */
#define COM_USART5_REMAP              STD_OFF
#define COM_USART5_LB_DBG             STD_OFF
/******************************************************************************/
/* DEFINITION OF TYPES                                                        */
/******************************************************************************/

/* available COMs for board support package */
typedef enum 
{
  COM1,
  COM2,
  COM3,
  COM4,
  COM5,
}COM_NameType;

/* baudrate select list */
typedef enum 
{
  COM_BAUD_9600    = 9600,
  COM_BAUD_19200   = 19200,
  COM_BAUD_38400   = 38400,
  COM_BAUD_57600   = 57600,
  COM_BAUD_115200  = 115200
}COM_BaudrateType;

/* COM Return values*/
typedef enum
{
	COM_OK,
  COM_FAILED,
}COM_ReturnType;

typedef enum
{
  COM_P_1 = 0,
  COM_P_2 = 1,
  COM_P_3 = 2,
  COM_P_4 = 5,
  COM_P_5 = 6,
}COM_IntPrio;
/*******************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                             */
/*******************************************************************************/
/*******************************************************************************
** Function name: COM_Init( COM_NameType ComName, COM_ReturnType Baud
**                             void * DataReceivedHandler )
** Description  : The function shall be init hardware communication layer
** Parameter    : ComName - Name of COM
**                Baud    - Baudrate select ( see in COM_ReturnType)
**                DataReceivedHandler - point to function process interrupt when
**                                      hardware received one byte data.
** Return value : COM_OK     - Process success
**                COM_FAILED - Process failed
** Remarks      : global variables used, side effects
*******************************************************************************/
COM_ReturnType COM_Init
(
  COM_NameType ComName, 
  COM_BaudrateType Baud,
  void (*DataReceivedHandle)(uint8_t)
);
/*******************************************************************************
** Function name: COM_WriteByte( COM_NameType ComName ,uint8_t byte )
** Description  : The function sends one byte to hardware layer
** Parameter    : byte - data send
** Return value : COM_OK     - Process success
**                COM_FAILED - Process failed
** Remarks      : global variables used, side effects
*******************************************************************************/
COM_ReturnType COM_WriteByte
(
  COM_NameType ComName,
	uint8_t byte
);
/*******************************************************************************
** Function name: COM_ReadByte( COM_NameType ComName )
** Description  : The function reads one byte from hardware layer
** Parameter    : ComName - COM select
** Return value : return data read from hardware
** Remarks      : global variables used, side effects
*******************************************************************************/
uint8_t COM_ReadByte
(
  COM_NameType ComName
);
/*******************************************************************************
** Function name: COM_HardwareInterruptEnabled( COM_NameType ComName )
** Description  : The function enables hardware interrupt
** Parameter    : ComName - COM select
** Return value : None
** Remarks      : global variables used, side effects
*******************************************************************************/
void COM_HardwareInterruptEnabled
(
  COM_NameType ComName
);
/*******************************************************************************
** Function name: COM_HardwareInterruptDisabled( COM_NameType ComName )
** Description  : The function disables hardware interrupt
** Parameter    : ComName - COM select
** Return value : None
** Remarks      : global variables used, side effects
*******************************************************************************/
void COM_HardwareInterruptDisabled
(
  COM_NameType ComName
);
#endif /* __DV_SPI_H__ */
