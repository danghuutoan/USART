/*
 * usart.h
 *
 *  Created on: Apr 15, 2015
 *      Author: toandang
 */

#ifndef USART_USART_H_
#define USART_USART_H_
#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "misc.h"
//#include "fifo.h"
void USART_Config(void);
void USARTPritnf(uint8_t* ch);


#endif /* USART_USART_H_ */
