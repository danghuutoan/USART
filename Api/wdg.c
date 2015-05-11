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
===============================================================================*/
/******************************************************************************/
/* INCLUSIONS                                                                 */
/******************************************************************************/
#include "wdg.h"
#include "wdg_cfg.h"
/******************************************************************************/
/* DEFINITION OF TYPES                                                        */
/******************************************************************************/
// typedef volatile unsigned short   vu16;
// typedef          unsigned short    u16;
// typedef volatile unsigned long    vu32;
// typedef          unsigned long     u32;
//  Peripheral memory map 
// /* Watchdog */
// #define IWDG_BASE         0x40003000
// #define IWDG            ((IWDG_TypeDef *) IWDG_BASE)
// typedef struct
// {
//   vu32 KR;
//   vu32 PR;
//   vu32 RLR;
//   vu32 SR;
// } IWDG_TypeDef;
/******************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                             */
/******************************************************************************/
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
)
{
  /*  enable write to PR, RLR */
  IWDG->KR  = WDG_CFG_DEFAULT_KEY_EN;               
  /* Init prescaler, default prescaler divider is 256 */
  IWDG->PR  = 6;                                       
  /* timeout period in milisecond, minimum is 1000 ms */
  if(WDG_CFG_IWDG_TIMEOUT > 1000)
  {
    /* init value for reload register */
    IWDG->RLR = WDG_CFG_MAX_VALUE_RELOAD&((WDG_CFG_IWDG_TIMEOUT/1000)*128);
  }
  else
  {
    IWDG->RLR = 128;   
  }
  /* Reload the watchdog */
  IWDG->KR  = WDG_CFG_DEFAULT_KEY_RELOAD;  
  
}
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
)
{
  /* writting value for starts */
  IWDG->KR  = WDG_CFG_DEFAULT_KEY_START;     
}
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
)
{
  /* do nothing */
}
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
)
{
  /* the watchdog refreshes */
  IWDG->KR  = WDG_CFG_DEFAULT_KEY_RELOAD;  
}
/*******************************************************************************/
/*                      End of File                                            */
/*******************************************************************************/
