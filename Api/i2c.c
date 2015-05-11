/*==============================================================================
**                                  
**                      Proprietary  - Copyright (C) 2015
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F4
** Supported Compilers : ARM GCC
**------------------------------------------------------------------------------
** File name         : i2c.c
**
** Module name       : I2C
**
**
** Summary: 
**
**= History ====================================================================
** 1.0.0 04/20/2015
**  - Creation
**  - Supports polling transfer
===============================================================================*/
/*******************************************************************************/
/* INCLUSIONS                                                                  */
/*******************************************************************************/
#include "i2c.h"

/*******************************************************************************/
/* DEFINITION OF CONST                                                         */
/*******************************************************************************/


/*******************************************************************************/
/* DECLARATION OF LOCAL FUNCTIONS                                              */
/*******************************************************************************/
/* init I2C pin function  */
static void I2C_iPinConfigure(I2C_Channel ch);
/*******************************************************************************/
/* DEFINITION OF LOCAL TYPE                                                    */
/*******************************************************************************/
/* GPIO Pin struct */
typedef struct _I2C_iGPIOPinType
{
    uint32_t               Clock;
    GPIO_TypeDef           *Port;
    uint16_t               PinNum;
    uint8_t                AFPin;
    uint8_t                AFFunc;
}I2C_iGPIOPinType;

typedef struct _I2C_IRQ
{
    uint8_t   EventIRQ;
    uint8_t   ErrorIRQ;
}I2C_IRQ;
/* I2C pin struct */
typedef struct _I2C_iStruct
{
    I2C_TypeDef            *I2CBase;
    uint32_t               ClockMask;
    I2C_IRQ                IRQ;
    I2C_iGPIOPinType       PIN_SCL;
    I2C_iGPIOPinType       PIN_SDA;
}I2C_iStruct;

/*******************************************************************************/
/* DECLARATION OF LOCAL VARIABLES                                              */
/*******************************************************************************/
/* I2C pin configure */
static const I2C_iStruct I2C_iParam[] =
{
    /* I2C1 */
    {
        .I2CBase     = I2C1,                 /* I2C1 base address */
        .ClockMask   = RCC_APB1Periph_I2C1,  /* I2C1 Clock bit */
        /* Interrupt */
        {
           .EventIRQ = I2C1_EV_IRQn,
           .ErrorIRQ = I2C1_ER_IRQn, 
        },
        /* I2C1_SCL */
        {
            .Clock = RCC_AHB1Periph_GPIOB,
            .Port  = GPIOB,
            .PinNum= GPIO_Pin_6,
            .AFPin = GPIO_PinSource6,
            .AFFunc= GPIO_AF_I2C1,
        },
        /* I2C1_SDA */  
        {
            .Clock = RCC_AHB1Periph_GPIOB,
            .Port  = GPIOB,
            .PinNum= GPIO_Pin_7,
            .AFPin = GPIO_PinSource7,
            .AFFunc= GPIO_AF_I2C1,
        },  
    },
    /* I2C2 */
    {
        .I2CBase     = I2C2,                 /* I2C2 base address */
        .ClockMask   = RCC_APB1Periph_I2C2,  /* I2C2 Clock bit */
        /* Interrupt */
        {
           I2C2_EV_IRQn,
           I2C2_ER_IRQn, 
        },
        /* I2C2_SCL */
        {
            .Clock = RCC_AHB1Periph_GPIOB,
            .Port  = GPIOB,
            .PinNum= GPIO_Pin_10,
            .AFPin = GPIO_PinSource10,
            .AFFunc= GPIO_AF_I2C2,
        },
        /* I2C2_SDA */  
        {
            .Clock = RCC_AHB1Periph_GPIOB,
            .Port  = GPIOB,
            .PinNum= GPIO_Pin_11,
            .AFPin = GPIO_PinSource11,
            .AFFunc= GPIO_AF_I2C2,
        },  
    },
    /* I2C3 */
    {
        .I2CBase     = I2C3,                 /* I2C3 base address */
        .ClockMask   = RCC_APB1Periph_I2C3,  /* I2C3 Clock bit */
        /* Interrupt */
        {
           I2C3_EV_IRQn,
           I2C3_ER_IRQn, 
        },
        /* I2C3_SCL */
        {
            .Clock = RCC_AHB1Periph_GPIOA,
            .Port  = GPIOB,
            .PinNum= GPIO_Pin_8,
            .AFPin = GPIO_PinSource8,
            .AFFunc= GPIO_AF_I2C3,
        },  
        /* I2C3_SDA */
        {
            .Clock = RCC_AHB1Periph_GPIOC,
            .Port  = GPIOB,
            .PinNum= GPIO_Pin_9,
            .AFPin = GPIO_PinSource9,
            .AFFunc= GPIO_AF_I2C3,
        },  
    }
};

/*******************************************************************************/
/* DEFINITION OF GLOBAL FUNCTIONS                                              */
/*******************************************************************************/
/*******************************************************************************
** Function name: I2C_Initialize( I2C_Struct * dev)
** Description  : The function shall be initialize Independent i2c channel
** Parameter    : dev
** Return value : I2C_ERR  
**                I2C_OK   
*******************************************************************************/
I2C_ReturnType I2C_Initialize( I2C_Struct * dev )
{
    I2C_ReturnType          Ret = I2C_ERR;
    I2C_InitTypeDef         I2C_InitStruct;
    NVIC_InitTypeDef        NVIC_InitStruct;
    if(dev != I2C_NULL)
    {
        /* enable clock for I2C module */
        RCC->APB1ENR |= I2C_iParam[dev->Config.Channel].ClockMask;
        /* configure I2C pin */
        I2C_iPinConfigure(dev->Config.Channel); 

        /* Initialize I2C peripheral */
        I2C_InitStruct.I2C_Mode         = I2C_Mode_I2C;
        I2C_InitStruct.I2C_DutyCycle    = I2C_DutyCycle_2;
        I2C_InitStruct.I2C_Ack          = I2C_Ack_Enable;
        I2C_InitStruct.I2C_OwnAddress1  = dev->Config.OwnerAddress;
        I2C_InitStruct.I2C_ClockSpeed   = (uint32_t)dev->Config.ClockSpeed;
        if(dev->Config.AddressMode == I2C_7BIT)
        {
            I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;            
        }
        else
        {
            I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_10bit;
        }
        /* Initialize I2C */
        I2C_Init(I2C_iParam[dev->Config.Channel].I2CBase, &I2C_InitStruct);
        /* check IRQ enabled */
        /* if(dev->Config.IRQHandle == IRQ_ENABLE) */
        if(0) /* current version is not support IRQ handle */
        {
            /* Enable Error and Buffer Interrupts */
            I2C_ITConfig(I2C_iParam[dev->Config.Channel].I2CBase, (I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR), ENABLE);
            /* Configure the I2C event priority */
            NVIC_InitStruct.NVIC_IRQChannel = I2C_iParam[dev->Config.Channel].IRQ.EventIRQ;
            NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
            NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStruct);
            /* Configure I2C error interrupt to have the higher priority */
            NVIC_InitStruct.NVIC_IRQChannel = I2C_iParam[dev->Config.Channel].IRQ.ErrorIRQ;
            NVIC_Init(&NVIC_InitStruct);
        }
        else
        {
            /* do no thing */
        }
        dev->Status = I2C_READY;
        Ret = I2C_OK;
    }         
    else
    {
        /* do nothing */
    }
    return Ret; 
}

/*******************************************************************************
** Function name: I2C_Start( I2C_Struct * dev)
** Description  : The function settings dutycycle on i2c channel
** Parameter    : dev - I2C device
** Return value : I2C_ERR  
**                I2C_OK    
*******************************************************************************/
I2C_ReturnType I2C_Start( I2C_Struct * dev )
{
  I2C_ReturnType ret = I2C_ERR;
  if(dev != I2C_NULL)
  {
    I2C_Cmd(I2C_iParam[dev->Config.Channel].I2CBase, ENABLE);
    ret = I2C_OK;
  }
  else
  {
    /* fail */
  }
  return ret; 
}

/*******************************************************************************
** Function name: I2C_Stop( I2C_Struct * dev)
** Description  : The function stops on i2c channel
** Parameter    : dev - I2C device
** Return value : I2C_ERR  
**                I2C_OK    
*******************************************************************************/
I2C_ReturnType I2C_Stop( I2C_Struct * dev )
{
  I2C_ReturnType ret = I2C_ERR;
  if(dev != I2C_NULL)
  {
    I2C_Cmd(I2C_iParam[dev->Config.Channel].I2CBase, DISABLE);
    ret = I2C_OK;
  }
  else
  {
    /* fail */
  }
  return ret; 
}

/*******************************************************************************
** Function name: I2C_Tx( I2C_Struct * dev)
** Description  : The function sends data on i2c channel
** Parameter    : dev - I2C device
** Return value : I2C_OK
**                I2C_ERR
**                I2C_BUFFER_ERR
**                I2C_SEND_COMPLETE
*******************************************************************************/
I2C_ReturnType I2C_Tx( I2C_Struct * dev )
{
  I2C_ReturnType       Ret = I2C_ERR;
  I2C_TypeDef          *I2Cx = I2C_NULL;
  if(dev != I2C_NULL)
  {
    I2Cx = I2C_iParam[dev->Config.Channel].I2CBase;
    /* Send START condition */
    I2C_GenerateSTART(I2Cx, ENABLE);
    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
    /* Send slave address for write */
    I2C_Send7bitAddress(I2Cx, dev->Data.SlaveAddr, I2C_Direction_Transmitter);
    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    /* transfer data */
    while(dev->Data.Len--)
    {
        /* Send data to slave */
        I2C_SendData(I2Cx, *(dev->Data.Buf++));
        /* Test on EV8 and clear it */
        while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }
    /* Send STOP condition */
    I2C_GenerateSTOP(I2Cx, ENABLE);
    Ret = I2C_OK;
  }
  else
  {
    /* Input parameter is null */
  }
  return Ret;
}

/*******************************************************************************
** Function name: I2C_Rx( I2C_Struct * dev)
** Description  : The function recieves data on i2c channel
** Parameter    : dev - I2C device
** Return value : I2C_OK
**                I2C_ERR
**                2C_BUFFER_ERR
**                I2C_READ_COMPLETE
*******************************************************************************/
I2C_ReturnType I2C_Rx( I2C_Struct * dev )
{
  I2C_ReturnType       Ret = I2C_ERR;
  I2C_TypeDef          *I2Cx = I2C_NULL;

  if(dev != I2C_NULL)
  {
    I2Cx = I2C_iParam[dev->Config.Channel].I2CBase;
    /* While the bus is busy */
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
    /* Send START condition */
    I2C_GenerateSTART(I2Cx, ENABLE);
    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
    /* Send slave address for write */
    I2C_Send7bitAddress(I2Cx, dev->Data.SlaveAddr, I2C_Direction_Transmitter);
    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    /* Clear EV6 by setting again the PE bit */
    I2C_Cmd(I2Cx, ENABLE);
    /* Send the internal address to write to */
    I2C_SendData(I2Cx, dev->Data.Buf[0]);
    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    /* Send Start condition a second time */
    I2C_GenerateSTART(I2Cx, ENABLE);
    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
    /* Send address for read */
    I2C_Send7bitAddress(I2Cx, dev->Data.SlaveAddr, I2C_Direction_Receiver);
    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    /* While there is data to be read */
    while (dev->Data.Len)
    {
        if (dev->Data.Len == 1)
        {
            /* Disable Acknowledgement */
            I2C_AcknowledgeConfig(I2Cx, DISABLE);
            /* Send STOP Condition */
            I2C_GenerateSTOP(I2Cx, ENABLE);
        }

        /* Test on EV7 and clear it */
        if (I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            /* Read a byte from the slave */
            *(dev->Data.Buf++) = I2C_ReceiveData(I2Cx);
            /* Decrement the read bytes counter */
            dev->Data.Len--;
        }
    }

    /* Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
    Ret = I2C_OK;
  }
  else
  {
    /* Input parameter is null */
  }
  
  return Ret; 
}

/*******************************************************************************/
/* DEFINITION OF LOCAL FUNCTIONS                                               */
/*******************************************************************************/
static void I2C_iPinConfigure(I2C_Channel ch)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    /* enable clock pin */
    RCC_AHB1PeriphClockCmd(I2C_iParam[ch].PIN_SCL.Clock,ENABLE); /* SCL pin */
    RCC_AHB1PeriphClockCmd(I2C_iParam[ch].PIN_SDA.Clock,ENABLE); /* SDA pin */
    /* AF configure */
    /* Connect PinSource to I2Cx */
    GPIO_PinAFConfig(
                    I2C_iParam[ch].PIN_SCL.Port,
                    I2C_iParam[ch].PIN_SCL.AFPin, 
                    I2C_iParam[ch].PIN_SCL.AFFunc
                    ); /* SCL */
    GPIO_PinAFConfig(
                    I2C_iParam[ch].PIN_SDA.Port,
                    I2C_iParam[ch].PIN_SDA.AFPin, 
                    I2C_iParam[ch].PIN_SDA.AFFunc
                    ); /* SDA */                    
    /* GPIO mode configure */
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType  = GPIO_OType_OD;
    GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_50MHz;
    /* SCL pin init */
    GPIO_InitStruct.GPIO_Pin    = I2C_iParam[ch].PIN_SCL.PinNum;
    GPIO_Init(I2C_iParam[ch].PIN_SCL.Port, &GPIO_InitStruct);
    /* SDA pin init */
    GPIO_InitStruct.GPIO_Pin    = I2C_iParam[ch].PIN_SDA.PinNum;
    GPIO_Init(I2C_iParam[ch].PIN_SCL.Port, &GPIO_InitStruct);

}

/*******************************************************************************/
/* DEFINITION OF INTERRUPT FUNCTIONS                                            */
/*******************************************************************************/

/* I2C1 Event ISR handler */
void I2C1_ISR_Event (void)
{

}
/* I2C1 Error ISR handler */
void I2C1_ISR_Error (void)
{
    uint16_t err = I2C_ReadRegister(I2C_iParam[0].I2CBase, I2C_Register_SR1) & 0xFF00;
    /* Read SR1 register to get I2C error */
    /* check error flags */
    if ((err & 0xFF00) != 0x00)
    {
        /* Clears error flags */
        I2C_iParam[0].I2CBase->SR1 &= 0x00FF;
    }
    else
    {
        /* do nothing */
    }
}
/* I2C2 Event ISR handler */
void I2C2_ISR_Event (void)
{

}
/* I2C2 Error ISR handler */
void I2C2_ISR_Error (void)
{
    uint16_t err = I2C_ReadRegister(I2C_iParam[1].I2CBase, I2C_Register_SR1) & 0xFF00;
    /* Read SR1 register to get I2C error */
    /* check error flags */
    if ((err & 0xFF00) != 0x00)
    {
        /* Clears error flags */
        I2C_iParam[1].I2CBase->SR1 &= 0x00FF;
    }
    else
    {
        /* do nothing */
    }
}
/* I2C3 Event ISR handler */
void I2C3_ISR_Event (void)
{

}
/* I2C3 Error ISR handler */
void I2C3_ISR_Error (void)
{
    uint16_t err = I2C_ReadRegister(I2C_iParam[2].I2CBase, I2C_Register_SR1) & 0xFF00;
    /* Read SR1 register to get I2C error */
    /* check error flags */
    if ((err & 0xFF00) != 0x00)
    {
        /* Clears error flags */
        I2C_iParam[2].I2CBase->SR1 &= 0x00FF;
    }
    else
    {
        /* do nothing */
    }
}
/*******************************************************************************/
/*                          End Of File                                        */
/*******************************************************************************/
