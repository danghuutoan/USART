#include "global.h"

#include "led.h"
#include "usart.h"
#include "com.h"
#include "encoder.h"

//void configureEncoder() {
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//    
//    GPIO_InitTypeDef GPIO_InitStructure;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6| GPIO_Pin_7;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
//    
//    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
//    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
//    
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//    TIM_TimeBaseStructure.TIM_Period = 0xffff;
//    TIM_TimeBaseStructure.TIM_Prescaler = 0;
//    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//    
//    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
//    
//    /* Configure the timer */
//    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
//    
//    /* TIM4 counter enable */
//    TIM_Cmd(TIM4, ENABLE);
//}


/* Task to be created. */
void vTaskCode( void * pvParameters )
{
  for( ;; )
  {
      /* Task code goes here. */
		//printf("ok : %d\r\n",TIM_GetCounter(TIM4));
		printf("ok : %d\r\n",ENC_GetCounter());
		if(Encoder_GetDir(TIM_CH4))
			printf("backward\r\n");
		else
			printf("forward\r\n");
		vTaskDelay(1000);
  }
}

int main(void)
{
	TaskHandle_t xHandle = NULL;

	COM_Init(COM2,COM_BAUD_38400,NULL);
	Encoder_Init(TIM_CH4);
	LED_Init();


	LED_On();
	printf("hello\r\n");
	xTaskCreate( vTaskCode, "NAME", 200, NULL, tskIDLE_PRIORITY, 
							&xHandle );
	configASSERT( xHandle );
	/* Start the scheduler so our tasks start executing. */
	vTaskStartScheduler();
	/* never to return here */
	return 0;
}


void vApplicationMallocFailedHook(void)
{
    /* This function will only be called if an API call to create a task, queue
    or semaphore fails because there is too little heap RAM remaining - and
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. */
    /* printf("\r\nSystem crashes, not enough RAM memory when tast created"); */
    while(1);
}

void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName)
{
(void)pxTask;
(void)pcTaskName;
    /* This function will only be called if a task overflows its stack.  Note
    that stack overflow checking does slow down the context switch
    implementation and will only be performed if configCHECK_FOR_STACK_OVERFLOW
    is set to either 1 or 2 in FreeRTOSConfig.h. */
    while(1);
}

void vApplicationIdleHook(void)
{
    /* This example does not use the idle hook to perform any processing.  The
    idle hook will only be called if configUSE_IDLE_HOOK is set to 1 in 
    FreeRTOSConfig.h. */
}

void vApplicationTickHook(void)
{
    /* This example does not use the tick hook to perform any processing.   The
    tick hook will only be called if configUSE_TICK_HOOK is set to 1 in
    FreeRTOSConfig.h. */
}
