/*==============================================================================
**                                  
**                      Proprietary - Copyright (C) 2015
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F4
** Supported Compilers : ARM GCC
**------------------------------------------------------------------------------
** File name         : PWM.h
**
** Module name       : PWM
**
**
** Summary: 
**
**= History ====================================================================
** 01.00.00 04/15/2015
** - Creation
===============================================================================*/
#ifndef __PWM_H__
#define __PWM_H__
/*******************************************************************************/
/* INCLUSIONS                                                                  */
/*******************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
/*******************************************************************************/
/* DEFINITION OF CONST                                                         */
/*******************************************************************************/
/* source version control */
#define PWM_VERSION_MAJOR        1
#define PWM_VERSION_MINOR        0
#define PWM_VERSION_REV          0

#define PWM_MAX_DUTYCYCLE        1000
#define PWM_MIN_DUTYCYCLE        427

#define PWM_NBR_CHANNEL          4
#define PWM_FAIL                 0
#define PWM_OK                   1

/*******************************************************************************/
/* DEFINITION OF TYPES                                                         */
/*******************************************************************************/
/* pwm channel index */
typedef enum _PWM_channel_tag
{
	PWM_CH1      =  0,
	PWM_CH2      =  1,
	PWM_CH3      =  2,
	PWM_CH4      =  3
}PWM_channel;
/* pwm structure */
typedef struct _PWM_tag
{
	const PWM_channel       	  chid;              /* pwm channel id */
	uint32_t                    duty_cycle;        /* pwm duty cycle */  
  const uint32_t              duty_min;          /* pwm min duty cycle*/  
  const uint32_t              duty_max;          /* pwm min duty cycle*/ 
}PWM_typedef;
/*******************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                             */
/*******************************************************************************/
/*******************************************************************************
** Function name: PWM_Init( PWM_typedef * PWM)
** Description  : The function shall be initialize Independent PWM channel
** Parameter    : None
** Return value : None
** Remarks      : PWM_FAIL  - Init unsuccessful
**                PWM_OK    - Init successful
*******************************************************************************/
int PWM_Init( PWM_typedef * PWM );
/*******************************************************************************
** Function name: PWM_SetDutyCycle( PWM_typedef * PWM)
** Description  : The function settings dutycycle on PWM channel
** Parameter    : None
** Return value : None
** Remarks      : PWM_FAIL  - Init unsuccessful
**                PWM_OK    - Init successful
*******************************************************************************/
int PWM_SetDutyCycle( PWM_typedef * PWM );
/*******************************************************************************
** Function name: PWM_Start( PWM_typedef * PWM)
** Description  : The function starts on PWM channel
** Parameter    : None
** Return value : None
** Remarks      : PWM_FAIL  - Init unsuccessful
**                PWM_OK    - Init successful
*******************************************************************************/
int PWM_Start( void);
/*******************************************************************************
** Function name: PWM_Stop( PWM_typedef * PWM)
** Description  : The function stops on PWM channel
** Parameter    : None
** Return value : None
** Remarks      : PWM_FAIL  - Init unsuccessful
**                PWM_OK    - Init successful
*******************************************************************************/
int PWM_Stop( void );

#endif /* __PWM_H__ */
