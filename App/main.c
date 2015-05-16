#include "global.h"

#include "led.h"
#include "usart.h"
#include "com.h"
#include "encoder.h"
#include "gpt.h"
#include "motor.h"
#include "pwm.h"
/*min=428*/
PWM_typedef pwm=
{
	.chid                   =   PWM_CH1,    
  .duty_cycle             =   427,       /* start default dutycycle value  = 0 */
  .duty_min               =   PWM_MIN_DUTYCYCLE,
  .duty_max               =   PWM_MAX_DUTYCYCLE  
};
/* Task to be created. */
void vTaskCode( void * pvParameters )
{
  for( ;; )
  {
      /* Task code goes here. */
		//printf("ok : %d\r\n",TIM_GetCounter(TIM4));
		printf("ok : %d\r\n",Encoder_GetVal(ENC_TIM));
//		if(Encoder_GetDir(ENC_TIM))
//			printf("backward\r\n");
//		else
//			printf("forward\r\n");
		vTaskDelay(100);
  }
}

int main(void)
{
	TaskHandle_t xHandle = NULL;

	COM_Init(COM2,COM_BAUD_38400,NULL);
	motor_Init();
	Encoder_Init(ENC_TIM);
	LED_Init();
	
	PWM_Init(&pwm);
	PWM_SetDutyCycle(&pwm);
	PWM_Start();

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
