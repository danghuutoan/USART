/*==============================================================================
**                                  QUADROBO
**                      Proprietary - Copyright (C) 2014
**                      
**------------------------------------------------------------------------------
** Supported MCUs      : MCU STM32
** Supported Compilers : ARM GCC
**------------------------------------------------------------------------------
** File name         : com.c
**  
** Module name       : Serial Communications 
**
** Summary: Source file of Serial driver
**
**= History ===================================================================
** 00.00.01  14/1/2014  Cuong Chu 
**            - Creation
** 00.00.02  1/4/2014 Cuong Chu
**           Remove DataReceivedHandle != NULL_PTR in Init function
**           Enable RCC clock
**           Enable Pin RCC clock
**           add feature enable/disable interrupt receive handle
** 00.00.03  28/07/2014 Cuong Chu
**           Added COM4,COM5 
** 00.00.04  06/08/2014 Cuong Chu
**           Added USART2 remap option
==============================================================================*/
/******************************************************************************/
/* INCLUSIONS                                                                 */
/******************************************************************************/
#include "com.h"
#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"
/*******************************************************************************/
/* GLOBAL CONFIGURATIONS                                                       */
/*******************************************************************************/

/******************************************************************************/
/* DEFINITION OF TYPE                                                         */
/******************************************************************************/
typedef struct __com_hw_pin_config
{
    uint32_t               Clock;
    GPIO_TypeDef           *Port;
    uint16_t               PinNum;
    uint8_t                AFPin;
    uint8_t                AFFunc;
}com_hw_pin;

typedef struct __com_hw_config
{
   COM_BaudrateType     ComBaudrate;
   USART_TypeDef*       COMx;
   uint32_t             ClockMask;
   com_hw_pin           PIN_TX;
   com_hw_pin           PIN_RX;
}com_hw_config;
/******************************************************************************/
/* DEFINITION OF LOCAL VARIABLE                                               */
/******************************************************************************/
static void (*DataReceivedHandler[5])(uint8_t);

static const com_hw_config hw_config[] = 
{
  /* USART1 */
  {
    .ComBaudrate  =  COM_BAUD_115200,
    .COMx         =  USART1,
    .ClockMask    =  RCC_APB2Periph_USART1,
    /* tx pin */
    {
      .Clock = RCC_AHB1Periph_GPIOB,
      .Port  = GPIOB,
      .PinNum= GPIO_Pin_6,
      .AFPin = GPIO_PinSource6,
      .AFFunc= GPIO_AF_USART1,
    },
    /* rx pin */
    {
      .Clock = RCC_AHB1Periph_GPIOB,
      .Port  = GPIOB,
      .PinNum= GPIO_Pin_7,
      .AFPin = GPIO_PinSource7,
      .AFFunc= GPIO_AF_USART1,
    }
  },
  /* USART2 */
  {
    .ComBaudrate  =  COM_BAUD_38400,
    .COMx         =  USART2,
    .ClockMask    =  RCC_APB1Periph_USART2,
    /* tx pin */
    {
      .Clock = RCC_AHB1Periph_GPIOA,
      .Port  = GPIOA,
      .PinNum= GPIO_Pin_2,
      .AFPin = GPIO_PinSource2,
      .AFFunc= GPIO_AF_USART2,
    },
    /* rx pin */
    {
      .Clock = RCC_AHB1Periph_GPIOA,
      .Port  = GPIOA,
      .PinNum= GPIO_Pin_3,
      .AFPin = GPIO_PinSource3,
      .AFFunc= GPIO_AF_USART2,
    }
  },
  /* USART3 */
  {
    .ComBaudrate  =  COM_BAUD_9600,
    .COMx         =  USART3,
    .ClockMask    =  RCC_APB1Periph_USART3,
    /* tx pin */
    {
      .Clock = RCC_AHB1Periph_GPIOB,
      .Port  = GPIOB,
      .PinNum= GPIO_Pin_10,
      .AFPin = GPIO_PinSource10,
      .AFFunc= GPIO_AF_USART3,
    },
    /* rx pin */
    {
      .Clock = RCC_AHB1Periph_GPIOB,
      .Port  = GPIOB,
      .PinNum= GPIO_Pin_11,
      .AFPin = GPIO_PinSource11,
      .AFFunc= GPIO_AF_USART3,
    }
  },
  /* UART4 */
  {
    .ComBaudrate  =  COM_BAUD_115200,
    .COMx         =  UART4,
    .ClockMask    =  RCC_APB1Periph_UART4,
    /* tx pin */
    {
      .Clock = RCC_AHB1Periph_GPIOB,
      .Port  = GPIOB,
      .PinNum= GPIO_Pin_6,
      .AFPin = GPIO_PinSource6,
      .AFFunc= GPIO_AF_UART4,
    },
    /* rx pin */
    {
      .Clock = RCC_AHB1Periph_GPIOB,
      .Port  = GPIOB,
      .PinNum= GPIO_Pin_6,
      .AFPin = GPIO_PinSource6,
      .AFFunc= GPIO_AF_UART4,
    }
  },
  /* UART5 */
  {
    .ComBaudrate  =  COM_BAUD_115200,
    .COMx         =  UART5,
    .ClockMask    =  RCC_APB1Periph_UART5,
    /* tx pin */
    {
      .Clock = RCC_AHB1Periph_GPIOB,
      .Port  = GPIOB,
      .PinNum= GPIO_Pin_6,
      .AFPin = GPIO_PinSource6,
      .AFFunc= GPIO_AF_UART5,
    },
    /* rx pin */
    {
      .Clock = RCC_AHB1Periph_GPIOB,
      .Port  = GPIOB,
      .PinNum= GPIO_Pin_6,
      .AFPin = GPIO_PinSource6,
      .AFFunc= GPIO_AF_UART5,
    }
  }
};

/******************************************************************************/
/* DEFINITION OF CONSTANTS                                                    */
/******************************************************************************/
#define NULL_PTR ((void *)0)

/******************************************************************************/
/* DEFINITION OF INTERNAL FUNCTIONS                                           */
/******************************************************************************/

/******************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                             */
/******************************************************************************/
/*******************************************************************************
** Function name: COM_Init( COM_NameType ComName, COM_ReturnType Baud
**                             void * DataReceivedHandle )
** Description  : The function shall be init hardware communication layer
** Parameter    : ComName - Name of COM
**                Baud    - Baudrate select ( see in COM_ReturnType)
**                DataReceivedHandle - point to function process interrupt when
**                                      hardware received one byte data.
** Return value : COM_OK     - Process success
**                COM_FAILED - Process failed
** Remarks      : global variables used, side effects
*******************************************************************************/
COM_ReturnType COM_Init
(
	COM_NameType ComName, 
  COM_BaudrateType Baud,
  void (*DataReceivedHandle)(uint8_t)
)
{
  COM_ReturnType retVal = COM_OK;
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  // Enable needed clocks for uart.
	if(ComName == COM1)
	{
		RCC_APB2PeriphClockCmd(hw_config[ComName].ClockMask, ENABLE);
	}
	else
	{
		RCC_APB1PeriphClockCmd(hw_config[ComName].ClockMask, ENABLE);
	}
  
  RCC_AHB1PeriphClockCmd(hw_config[ComName].PIN_TX.Clock, ENABLE);
  RCC_AHB1PeriphClockCmd(hw_config[ComName].PIN_RX.Clock, ENABLE);

  // Make sure you use 'GPIO_PinSource2' and NOT 'GPIO_Pin_2'.  Using the
  // latter will not work!
  GPIO_PinAFConfig(
                  hw_config[ComName].PIN_TX.Port,
                  hw_config[ComName].PIN_TX.AFPin,
                  hw_config[ComName].PIN_TX.AFFunc);
  GPIO_PinAFConfig(
                  hw_config[ComName].PIN_RX.Port,
                  hw_config[ComName].PIN_RX.AFPin,
                  hw_config[ComName].PIN_RX.AFFunc);

  // Setup Tx / Rx pins.
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = hw_config[ComName].PIN_TX.PinNum;			// Tx Pin
  GPIO_Init(hw_config[ComName].PIN_TX.Port, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = hw_config[ComName].PIN_RX.PinNum;			// Rx Pin
  GPIO_Init(hw_config[ComName].PIN_RX.Port, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = hw_config[ComName].ComBaudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(hw_config[ComName].COMx, &USART_InitStructure);
  USART_Cmd(hw_config[ComName].COMx, ENABLE);
  /* send dummy byte */
  COM_WriteByte(ComName,0x00);
  return retVal;
}
/*******************************************************************************
** Function name: COM_WriteByte( COM_NameType ComName ,uint8_t byte )
** Description  : The function sends one byte to hardware layer
** Parameter    : ComName - COM select
**                byte - data send
** Return value : COM_OK     - Process success
**                COM_FAILED - Process failed
** Remarks      : global variables used, side effects
*******************************************************************************/
COM_ReturnType COM_WriteByte
(
  COM_NameType ComName,
	uint8_t byte
)
{
  /* Transmit Data */
  USART_SendData(hw_config[ComName].COMx, byte);
  /* wait transmit success */
  while(  (hw_config[ComName].COMx->SR & 0x0040) == (uint16_t)0);
  return COM_OK;
}
/*******************************************************************************
** Function name: COM_ReadByte( COM_NameType ComName )
** Description  : The function reads one byte from hardware layer
** Parameter    : ComName - COM select
** Return value : return data read from hardware
** Remarks      : global variables used, side effects
*******************************************************************************/
uint8_t COM_ReadByte
(
  COM_NameType ComName
)
{
  /* Reaceive Data */
  return (uint8_t)(hw_config[ComName].COMx->DR & (uint16_t)0x01FF);
}
/*******************************************************************************
** Function name: USART1_IRQHandler( void )
** Description  : Interrupt function of COM1
** Parameter    : None
** Return value : None
** Remarks      : global variables used, side effects
*******************************************************************************/
void USART1_IRQHandler
(
	void
)
{
	uint8_t c = 0xFF;
  /* check  */
  if(DataReceivedHandler[COM1] != NULL_PTR)
  {
    /* read data */
    c = COM_ReadByte(COM1);
    /* call back function to trasnfer data to upper ComM module */
#if (COM_USART1_LB_DBG == STD_ON)
    COM_WriteByte(COM1,c);
#endif       
    (*DataReceivedHandler[COM1])(c);
  }
  else
  {
    /* do nothing */
  } 
}
/*******************************************************************************
** Function name: USART2_IRQHandler( void )
** Description  : Interrupt function of COM2
** Parameter    : None
** Return value : None
** Remarks      : global variables used, side effects
*******************************************************************************/
void USART2_IRQHandler
(
	void
)
{
	uint8_t c = 0xFF;
  /* check  */
  if(DataReceivedHandler[COM2] != NULL_PTR)
  {
    /* read data */
    c = COM_ReadByte(COM2);
    /* call back function to trasnfer data to upper ComM module */
#if (COM_USART2_LB_DBG == STD_ON)
    COM_WriteByte(COM2,c);
#endif       
    (*DataReceivedHandler[COM2])(c);
  }
  else
  {
    /* do nothing */
  } 
}
/*******************************************************************************
** Function name: USART3_IRQHandler( void )
** Description  : Interrupt function of COM3
** Parameter    : None
** Return value : None
** Remarks      : global variables used, side effects
*******************************************************************************/
void USART3_IRQHandler
(
	void
)
{
	uint8_t c = 0xFF;
  /* check  */
  if(DataReceivedHandler[COM3] != NULL_PTR)
  {
    /* read data */
    c = COM_ReadByte(COM3);
    /* call back function to trasnfer data to upper ComM module */
#if (COM_USART3_LB_DBG == STD_ON)
    COM_WriteByte(COM3,c);
#endif       
    (*DataReceivedHandler[COM3])(c);
  }
  else
  {
    /* do nothing */
  } 
}
/*******************************************************************************
** Function name: UART5_IRQHandler( void )
** Description  : Interrupt function of COM4
** Parameter    : None
** Return value : None
** Remarks      : global variables used, side effects
*******************************************************************************/
void UART4_IRQHandler
(
	void
)
{
	uint8_t c = 0xFF;
  /* check  */
  if(DataReceivedHandler[COM4] != NULL_PTR)
  {
    /* read data */
    c = COM_ReadByte(COM4);
    /* call back function to trasnfer data to upper ComM module */
    (*DataReceivedHandler[COM4])(c);
#if (COM_USART4_LB_DBG == STD_ON)
    COM_WriteByte(COM4,c);
#endif       
  }
  else
  {
    /* do nothing */
  } 
}
/*******************************************************************************
** Function name: UART5_IRQHandler( void )
** Description  : Interrupt function of COM5
** Parameter    : None
** Return value : None
** Remarks      : global variables used, side effects
*******************************************************************************/
void UART5_IRQHandler
(
	void
)
{
	uint8_t c = 0xFF;
  /* check  */
  if(DataReceivedHandler[COM5] != NULL_PTR)
  {
    /* read data */
    c = COM_ReadByte(COM5);
    /* call back function to trasnfer data to upper ComM module */
#if (COM_USART5_LB_DBG == STD_ON)
    COM_WriteByte(COM5,c);
#endif    
    (*DataReceivedHandler[COM5])(c);
  }
  else
  {
    /* do nothing */
  } 
}
/*******************************************************************************
** Function name: COM_HardwareInterruptEnabled( COM_NameType ComName )
** Description  : The function enables hardware interrupt
** Parameter    : ComName - COM select
** Return value : None
** Remarks      : global variables used, side effects
*******************************************************************************/
void COM_HardwareInterruptEnabled
(
  COM_NameType ComName
)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  /* Set the PRIGROUP[10:8] bits according to NVIC_PriorityGroup value */
  SCB->AIRCR = 0x05FA0000 | 0x700;
  hw_config[ComName].COMx->CR1 |= (1UL << 5);
  /* enable interrupt channel */
  //@TODO
	//NVIC_InitStructure.NVIC_IRQChannel =  COM_PortConfig[ComName].IntChn;
  //NVIC_InitStructure.NVIC_IRQChannelSubPriority = COM_PortConfig[ComName].Prio;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );
}
/*******************************************************************************
** Function name: COM_HardwareInterruptDisabled( COM_NameType ComName )
** Description  : The function disables hardware interrupt
** Parameter    : ComName - COM select
** Return value : None
** Remarks      : global variables used, side effects
*******************************************************************************/
void COM_HardwareInterruptDisabled
(
  COM_NameType ComName
)
{
  /* usart interrupt disable */
  hw_config[ComName].COMx->CR1 &= ~(1UL << 5);
}
/*****************************************************************************/
/* End Of File Copyright LINDBOARD 2014                                      */
/*****************************************************************************/
