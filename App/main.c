#include "global.h"





int main(void)
{

	 COM_Init(COM2,COM_BAUD_38400,NULL);
	LED_Init();
	
	
	LED_On();
	printf("hello\r\n");
	while(1)
	{

	}
}

