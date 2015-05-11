/*==============================================================================
**                                  
**                      Proprietary - Copyright (C) 2015
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F4
** Supported Compilers : ARM GCC
**------------------------------------------------------------------------------
** File name         : gpt.c
**
** Module name       : GPT
**
**
** Summary:   Digital Input/Out
**
**= History ====================================================================
** 01.00.00 04/15/2015
** - Creation
===============================================================================*/
/*******************************************************************************/
/* INCLUSIONS                                                                  */
/*******************************************************************************/
#include "gpt.h"
/*******************************************************************************/
/* DEFINITION OF CONST                                                         */
/*******************************************************************************/


/*******************************************************************************/
/* DEFINITION OF TYPES                                                         */
/*******************************************************************************/

/* GPT Pin struct */
typedef struct _GPT_iPinType
{
  uint32_t               Clock;
  GPIO_TypeDef           *Port;
  uint16_t               PinNum;
}GPT_iPinType;

/*******************************************************************************/
/* DECLARATION OF LOCAL VARIABLES                                              */
/*******************************************************************************/
/* GPIO pin configure */
static const GPT_iPinType GPT_iPinParam[] = 
{
  {
    .Clock = RCC_AHB1Periph_GPIOA,
    .Port  = GPIOA,
    .PinNum= GPIO_Pin_0
  },
  {
    .Clock = RCC_AHB1Periph_GPIOA,
    .Port  = GPIOA,
    .PinNum= GPIO_Pin_0,
  },
  {
    .Clock = RCC_AHB1Periph_GPIOD,
    .Port  = GPIOD,
    .PinNum= GPIO_Pin_12,
  },
  {
    .Clock = RCC_AHB1Periph_GPIOD,
    .Port  = GPIOD,
    .PinNum= GPIO_Pin_15,
  },
  {
    .Clock = RCC_AHB1Periph_GPIOD,
    .Port  = GPIOD,
    .PinNum= GPIO_Pin_14,
  },
};
/*******************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                              */
/*******************************************************************************/
/*******************************************************************************
** Function name: GPT_Init( GPT_PinType * GPT)
** Description  : The function shall be initialize independent GPIO channel
** Parameter    : - GPT : GPT struct
** Return value : None
** Remarks      : None
*******************************************************************************/
void GPT_Init( GPT_PinType * GPT )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(GPT != (void*)0)
  {
    if(GPT_P_MAX > GPT->PortName)
    {
      /* check dir init */
      if(GPT_OUT == GPT->Dir)
      {
        /* enable port clock */
        RCC_AHB1PeriphClockCmd(GPT_iPinParam[GPT->PortName].Clock,ENABLE); 
        /* config GPIO */
        GPIO_InitStruct.GPIO_Pin   = GPT_iPinParam[GPT->PortName].PinNum;
        GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
        GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPT_iPinParam[GPT->PortName].Port, &GPIO_InitStruct);
        /* set default GPIO state */
        if(GPT->Status == GPT_HIGH)
        {
          GPIO_SetBits(
                      GPT_iPinParam[GPT->PortName].Port, 
                      GPT_iPinParam[GPT->PortName].PinNum);
        }
        else
        {
          GPIO_ResetBits(
                      GPT_iPinParam[GPT->PortName].Port, 
                      GPT_iPinParam[GPT->PortName].PinNum);
        }
      }
      else
      {
        /* enable port clock */
        RCC_AHB1PeriphClockCmd(GPT_iPinParam[GPT->PortName].Clock,ENABLE); 
        /* config GPIO */
        GPIO_InitStruct.GPIO_Pin   = GPT_iPinParam[GPT->PortName].PinNum;
        GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;
        GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPT_iPinParam[GPT->PortName].Port, &GPIO_InitStruct);
        /* read default GPIO level */
        if(GPIO_ReadInputDataBit(
                                GPT_iPinParam[GPT->PortName].Port, 
                                GPT_iPinParam[GPT->PortName].PinNum))
        {
          GPT->Status = GPT_HIGH;
        }
        else
        {
          GPT->Status = GPT_LOW;
        }
      }
    }
    else
    {
      /* Invalid GPT portname */
    }
  }
  else
  {
    /* GPT null pointer */
  }
}
/*******************************************************************************
** Function name: GPT_SetDutyCycle( GPT_PinType * GPT)
** Description  : The function reads status on GPT channel
** Parameter    : - GPT : GPT struct
** Return value : None
** Remarks      : None
*******************************************************************************/
void GPT_ReadStatus( GPT_PinType * GPT )
{
  if(GPT != (void*)0)
  {
    if(GPT_P_MAX > GPT->PortName)
    {
      /* check dir init */
      if(GPT_IN == GPT->Dir)
      {
        /* read default GPIO level */
        if(GPIO_ReadInputDataBit(
                                GPT_iPinParam[GPT->PortName].Port, 
                                GPT_iPinParam[GPT->PortName].PinNum))
        {
          GPT->Status = GPT_HIGH;
        }
        else
        {
          GPT->Status = GPT_LOW;
        }
      }
      else
      {
        /* do nothing */
      }
    }
    else
    {
      /* Invalid GPT portname */
    }
  }
  else
  {
    /* GPT null pointer */
  }
}
/*******************************************************************************
** Function name: GPT_Start( GPT_PinType * GPT)
** Description  : The function sets status on GPT channel
** Parameter    : - GPT : GPT struct
** Return value : None
** Remarks      : None
*******************************************************************************/
void GPT_SetStatus( GPT_PinType * GPT )
{
  if(GPT != (void*)0)
  {
    if(GPT_P_MAX > GPT->PortName)
    {
      /* check dir init */
      if(GPT_OUT == GPT->Dir)
      {
        /* set default GPIO state */
        if(GPT->Status == GPT_HIGH)
        {
          GPIO_SetBits(
                      GPT_iPinParam[GPT->PortName].Port, 
                      GPT_iPinParam[GPT->PortName].PinNum);
        }
        else
        {
          GPIO_ResetBits(
                      GPT_iPinParam[GPT->PortName].Port, 
                      GPT_iPinParam[GPT->PortName].PinNum);
        }
      }
      else
      {
        /* do nothing */
      }
    }
    else
    {
      /* Invalid GPT portname */
    }
  }
  else
  {
    /* GPT null pointer */
  }
}
/****************************** End Of File ************************************
**                        Copyright  2015                              
*******************************************************************************/
