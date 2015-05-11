#ifndef __LED_H__
#define __LED_H__
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define LED_Pin 		GPIO_Pin_5
#define LED_Port		GPIOA
#define LED_RCC_Clk		RCC_AHB1Periph_GPIOA


void LED_Init(void);
void LED_On(void);
void LED_Off(void);

#endif
