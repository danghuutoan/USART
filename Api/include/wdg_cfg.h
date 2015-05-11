/*==============================================================================
**                                  
**                      Proprietary - Copyright (C) 2013
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F
** Supported Compilers : ARM GCC
**------------------------------------------------------------------------------
** File name         : WDG_Cfg.h
**
** Module name       : Independent Watchdog 
**
**
** Summary: IWDG Configurations
**
**= History ====================================================================
** 00.00.01 7/1/2014 Cuong Chu
** - Creation
===============================================================================*/
#ifndef __WDG_CFG_H__
#define __WDG_CFG_H__
/*******************************************************************************/
/* INCLUSIONS                                                                  */
/*******************************************************************************/
#include <stdint.h>
/*******************************************************************************/
/* DEFINITION OF CONSTS                                                        */
/*******************************************************************************/
/* Timeout of IWDG (ms) */
#define WDG_CFG_IWDG_TIMEOUT                  2000                 
/* Default Key Enable */
#define WDG_CFG_DEFAULT_KEY_EN                0x5555        /* no edit */
/* Default Key Reload */
#define WDG_CFG_DEFAULT_KEY_RELOAD            0xAAAA        /* no edit */
/* Default Key Start */
#define WDG_CFG_DEFAULT_KEY_START             0xCCCC        /* no edit */
/* Maximum value of reload register */
#define WDG_CFG_MAX_VALUE_RELOAD              0xFFF         /* no edit */
#endif /* __WDG_CFG_H__ */
