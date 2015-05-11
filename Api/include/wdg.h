/*==============================================================================
**                                  
**                      Proprietary - Copyright (C) 2013
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : ARM GCC
**------------------------------------------------------------------------------
** File name         : WDG.h
**
** Module name       : Watchdog
**
**
** Summary: 
**
**= History ====================================================================
** 00.00.01 9/1/2014 Cuong Chu
** - Creation
** 00.00.02 4/19/2015 Cuong Chu
** - Correct name function
===============================================================================*/
#ifndef __WDG_H__
#define __WDG_H__
/*******************************************************************************/
/* INCLUSIONS                                                                  */
/*******************************************************************************/
#include <stdint.h>
#include "stm32f4xx.h"
/*******************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                             */
/*******************************************************************************/
/*******************************************************************************
** Function name: WDG_Init( void )
** Description  : The function shall be initialial Independent WDG
** Parameter    : None
** Return value : None
** Remarks      : global variables used, side effects
*******************************************************************************/
void WDG_Init
(
	void
);
/*******************************************************************************
** Function name: WDG_Start( void )
** Description  : The function starts of IWDG
** Parameter    : None
** Return value : None
** Remarks      : global variables used, side effects
*******************************************************************************/
void WDG_Start
(
	void
);
/*******************************************************************************
** Function name: WDG_Stop( void )
** Description  : The function stops of IWDG
** Parameter    : None
** Return value : None
** Remarks      : global variables used, side effects
*******************************************************************************/
void WDG_Stop
(
	void
);
/*******************************************************************************
** Function name: WDG_Refresh( void )
** Description  : The function refreshes value of the IWDG
** Parameter    : None
** Return value : None
** Remarks      : global variables used, side effects
*******************************************************************************/
void WDG_Refresh
(
	void
);
#endif /* __WDG_H__ */
