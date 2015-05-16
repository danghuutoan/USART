/*==============================================================================
**                                  
**                      Proprietary - Copyright (C) 2015
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F4
** Supported Compilers : ARM GCC
**------------------------------------------------------------------------------
** File name         : PWM.c
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
/*******************************************************************************/
/* INCLUSIONS                                                                  */
/*******************************************************************************/
#include "pwm.h"
/*******************************************************************************/
/* DEFINITION OF CONST                                                         */
/*******************************************************************************/


/*******************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                              */
/*******************************************************************************/

/*******************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                              */
/*******************************************************************************/

/*******************************************************************************
** Function name: PWM_Init( PWM_typedef * PWM)
** Description  : The function shall be initialize Independent PWM channel
** Parameter    : None
** Return value : None
** Remarks      : PWM_FAIL  - Init unsuccessful
**                PWM_OK    - Init successful
*******************************************************************************/
int PWM_Init( PWM_typedef * PWM )
{
	int Retval = PWM_FAIL;
  GPIO_InitTypeDef           GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
  TIM_OCInitTypeDef          TIM_OCInitStructure;
  uint16_t PrescalerValue = 0;
	if(PWM != NULL)
	{
    /* TIM3 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* GPIOC clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    
    /* GPIOC Configuration: TIM3 CH1 (PC6), TIM3 CH2 (PC7), TIM3 CH3 (PC8) and TIM3 CH4 (PC9) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOC, &GPIO_InitStructure); 

    /* Connect TIM3 pins to AF2 */  
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3); 
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3); 
    /* -----------------------------------------------------------------------
    TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles.
    
    In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.   
      TIM3CLK = 2 * PCLK1  
      PCLK1 = HCLK / 4 
      => TIM3CLK = HCLK / 2 = SystemCoreClock /2
          
    To get TIM3 counter clock at 21 MHz, the prescaler is computed as follows:
       Prescaler = (TIM3CLK / TIM3 counter clock) - 1
       Prescaler = ((SystemCoreClock /2) /21 MHz) - 1
                                              
    To get TIM3 output clock at 30 KHz, the period (ARR)) is computed as follows:
       ARR = (TIM3 counter clock / TIM3 output clock) - 1
           = 665
                  
    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
    TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
    TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%

    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
     Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
     function to update SystemCoreClock variable value. Otherwise, any configuration
     based on this variable will be incorrect.    
   ----------------------------------------------------------------------- */   


    /* Compute the prescaler value */
    PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 21000000) - 1;

    /* Time base configuration */ 
    /*
     * PWM 20KHz  = 1000
     * PWM 10KHz  = 2000
     * PWM 7.5KHz = 3000
     * PWM 5KHz   = 4000
     */
    TIM_TimeBaseStructure.TIM_Period = 1000; /*  PWM  */
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM3, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel2 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;

    TIM_OC2Init(TIM3, &TIM_OCInitStructure);

    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel3 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;

    TIM_OC3Init(TIM3, &TIM_OCInitStructure);

    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel4 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;

    TIM_OC4Init(TIM3, &TIM_OCInitStructure);

    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM3, ENABLE);

		Retval = PWM_OK;
	}
	else
	{
		/* do nothing */
	}

	return Retval;
}
/*******************************************************************************
** Function name: PWM_SetDutyCycle( PWM_typedef * PWM)
** Description  : The function settings dutycycle on PWM channel
** Parameter    : None
** Return value : None
** Remarks      : PWM_FAIL  - Init unsuccessful
**                PWM_OK    - Init successful
*******************************************************************************/
int PWM_SetDutyCycle( PWM_typedef * PWM )
{
	int Retval = PWM_FAIL;
  if(PWM != NULL)
  {
      switch(PWM->chid)
      {
        case PWM_CH1:
        TIM_SetCompare1(TIM3,PWM->duty_cycle);
        break;
        case PWM_CH2:
        TIM_SetCompare2(TIM3,PWM->duty_cycle);
        break;
        case PWM_CH3:
        TIM_SetCompare3(TIM3,PWM->duty_cycle);
        break;
        case PWM_CH4:
        TIM_SetCompare4(TIM3,PWM->duty_cycle);
        break;
        default: break;           
      }
      Retval = PWM_OK;
  }
  else
  {
      /* do nothing */
  }
	return Retval;
}
/*******************************************************************************
** Function name: PWM_Start( void )
** Description  : The function starts on PWM channel
** Parameter    : None
** Return value : None
** Remarks      : PWM_FAIL  - Init unsuccessful
**                PWM_OK    - Init successful
*******************************************************************************/
int PWM_Start( void )
{
  /* Start PWM on TIM3 */
  TIM_Cmd(TIM3, ENABLE);
	return PWM_OK;	
}
/*******************************************************************************
** Function name: PWM_Stop( PWM_typedef * PWM)
** Description  : The function stops on PWM channel
** Parameter    : None
** Return value : None
** Remarks      : PWM_FAIL  - Init unsuccessful
**                PWM_OK    - Init successful
*******************************************************************************/
int PWM_Stop( void )
{
  /* Stop PWM on TIM3 */
  TIM_Cmd(TIM3, DISABLE);
	return PWM_OK;
}
