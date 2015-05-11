/*==============================================================================
**                                  
**                      Proprietary - Copyright (C) 2015
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F4
** Supported Compilers : ARM GCC
**------------------------------------------------------------------------------
** File name         : gpt.h
**
** Module name       : GPT
**
**
** Summary:   Digital Input/Out
**
**= History ====================================================================
** 01.00.00 04/24/2015 Cuong Chu
** - Creation
===============================================================================*/
#ifndef __GPT_H__
#define __GPT_H__
/*******************************************************************************/
/* INCLUSIONS                                                                  */
/*******************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include "misc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
/*******************************************************************************/
/* DEFINITION OF CONST                                                         */
/*******************************************************************************/
/* source version control */
#define GPT_VERSION_MAJOR     1
#define GPT_VERSION_MINOR     0
#define GPT_VERSION_REV       0


#define GPT_NBR_CHANNEL          5
#define GPT_FAIL                 0
#define GPT_OK                   1

/*******************************************************************************/
/* DEFINITION OF TYPES                                                         */
/*******************************************************************************/
/* define of gpio name */
typedef enum __GPT_PortName
{
  GPT_P_1,
  GPT_P_2,
  GPT_P_3,
  GPT_P_4,
  GPT_P_5,
  GPT_P_MAX
}GPT_PortName;

/* gpio direction */
typedef enum __GPT_Direction
{
  GPT_OUT  =  0,
  GPT_IN   =  1
}GPT_Direction;

/* gpio level */
typedef enum __GPT_STD_Level
{
  GPT_LOW  = 0,
  GPT_HIGH = 1,
}GPT_Level;

/* GPT Pin struct */
typedef struct _I2C_iGPTPinType
{
  const GPT_PortName           PortName;
  const GPT_Direction          Dir;
  GPT_Level                    Status;
}GPT_PinType;


/*******************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                             */
/*******************************************************************************/
/*******************************************************************************
** Function name: GPT_Init( GPT_PinType * GPT)
** Description  : The function shall be initialize independent GPIO channel
** Parameter    : - GPT : GPT struct
** Return value : None
** Remarks      : None
*******************************************************************************/
void GPT_Init( GPT_PinType * GPT );
/*******************************************************************************
** Function name: GPT_SetDutyCycle( GPT_PinType * GPT)
** Description  : The function reads status on GPT channel
** Parameter    : - GPT : GPT struct
** Return value : None
** Remarks      : None
*******************************************************************************/
void GPT_ReadStatus( GPT_PinType * GPT );
/*******************************************************************************
** Function name: GPT_Start( GPT_PinType * GPT)
** Description  : The function sets status on GPT channel
** Parameter    : - GPT : GPT struct
** Return value : None
** Remarks      : None
*******************************************************************************/
void GPT_SetStatus( GPT_PinType * GPT );


#endif /* __GPT_H__ */
/****************************** End Of File ************************************
**                        Copyright  2015                              
*******************************************************************************/
