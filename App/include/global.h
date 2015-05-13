#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <stdio.h>
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
/* Kernel includes. */
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"

#include "task.h"
#include "timers.h"
#include "semphr.h"

typedef struct _AF_iGPIOPinType
{
    uint32_t               Clock;
    GPIO_TypeDef           *Port;
    uint16_t               PinNum;
    uint8_t                AFPin;
    uint8_t                AFFunc;
}AF_iGPIOPinType;

/* IRQ Handle */
typedef enum _IRQ_t
{
    IRQ_ENABLE,
    IRQ_DISABLE
}IRQ_t;
#endif

