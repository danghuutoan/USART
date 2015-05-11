#include "led.h"

void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	 /* GPIOD Periph clock enable */
	  RCC_AHB1PeriphClockCmd(LED_RCC_Clk, ENABLE);

	  /* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
	  GPIO_InitStructure.GPIO_Pin = LED_Pin;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_Init(LED_Port, &GPIO_InitStructure);
}

void LED_On(void)
{
	GPIO_SetBits(LED_Port, LED_Pin);
}
void LED_Off(void)
{
	GPIO_ResetBits(LED_Port, LED_Pin);
}
