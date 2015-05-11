/*==============================================================================
**                                  
**                      Proprietary  - Copyright (C) 2015
**------------------------------------------------------------------------------
** Supported MCUs      : STM32F4
** Supported Compilers : ARM GCC
**------------------------------------------------------------------------------
** File name         : i2c.h
**
** Module name       : I2C
**
**
** Summary: 
**
**= History ====================================================================
** 1.0.0 04/20/2015 Cuong Chu
**  - Creation
===============================================================================*/
#ifndef __I2C_H__
#define __I2C_H__
/*******************************************************************************/
/* INCLUSIONS                                                                  */
/*******************************************************************************/
#include <stdint.h>
#include "misc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_rcc.h"

/*******************************************************************************/
/* DEFINITION OF CONST                                                         */
/*******************************************************************************/
/* software version */
#define I2C_SW_VERSION_MAJOR              1
#define I2C_SW_VERSION_MINOR              0
#define I2C_SW_VERSION_REV                0

#define I2C_NULL                          (void*)0
#define I2C_TIME_OUT                      1000000
/*******************************************************************************/
/* DEFINITION OF MACROS                                                        */
/*******************************************************************************/
/* I2C interrupt processing */
#define I2C1_ISR_Event                       I2C1_EV_IRQHandler   
#define I2C1_ISR_Error                       I2C1_ER_IRQHandler
#define I2C2_ISR_Event                       I2C2_EV_IRQHandler   
#define I2C2_ISR_Error                       I2C2_ER_IRQHandler
#define I2C3_ISR_Event                       I2C3_EV_IRQHandler   
#define I2C3_ISR_Error                       I2C3_ER_IRQHandler

/*******************************************************************************/
/* DEFINITION OF TYPES                                                         */
/*******************************************************************************/
typedef enum _I2C_ReturnType
{
    I2C_OK,
    I2C_ERR,
    I2C_BUFFER_ERR,
    I2C_READ_COMPLETE,
    I2C_SEND_COMPLETE
}I2C_ReturnType;

/* I2C operation mode */
typedef enum _I2C_OperationMode
{
    I2C_MASTER  = 0,
    I2C_SLAVE   = 1
}I2C_OperationMode;

/* address mode */
typedef enum _I2C_AddrMode
{
    I2C_7BIT    = 0,
    I2C_10BIT   = 1
}I2C_AddrMode;

/* I2C channel */
typedef enum _I2C_Channel
{
    I2C_CH_1,
    I2C_CH_2,
    I2C_CH_3
}I2C_Channel;

/* I2C buffer  */
typedef struct _I2C_Buffer
{
    uint16_t     Len;
    uint8_t*     Buf;
    uint8_t      SlaveAddr;
}I2C_BufferType;

/* I2C resource status */
typedef enum _I2C_Status
{
    I2C_INIT,
    I2C_READY,
    I2C_TX_BUSY,
    I2C_RX_BUSY
}I2C_StatusType;
/* IRQ Handle */
typedef enum _I2C_IRQHandle
{
    IRQ_ENABLE,
    IRQ_DISABLE
}I2C_IRQHandle;

/* I2C clock speed */
typedef enum _I2C_ClockSpeed
{
    I2C_SPEED_100KHz    = 100000,
    I2C_SPEED_400KHz    = 400000
}I2C_ClockSpeed;

/* I2C configure paramters  */
typedef struct _I2C_ConfigDef
{
    const I2C_Channel       Channel;
    const I2C_OperationMode OperationMode;
    const I2C_AddrMode      AddressMode;
    const uint8_t           OwnerAddress;
    const I2C_ClockSpeed    ClockSpeed;
    const I2C_IRQHandle     IRQHandle;
}I2C_ConfigType;

/* I2C structure */
typedef struct _I2C_Struct
{
    I2C_ConfigType        Config;  
    I2C_BufferType        Data;
    I2C_StatusType        Status;
}I2C_Struct;

/*******************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                             */
/*******************************************************************************/
void I2C1_ISR_Event (void);
void I2C1_ISR_Error (void);
void I2C2_ISR_Event (void);
void I2C2_ISR_Error (void);
void I2C3_ISR_Event (void);
void I2C3_ISR_Error (void);
/*******************************************************************************
** Function name: I2C_Initialize( I2C_Struct * i2c)
** Description  : The function shall be initialize Independent i2c channel
** Parameter    : i2c
** Return value : I2C_ERR  
**                I2C_OK   
*******************************************************************************/
I2C_ReturnType I2C_Initialize( I2C_Struct * i2c );

/*******************************************************************************
** Function name: I2C_Start( I2C_Struct * i2c)
** Description  : The function settings dutycycle on i2c channel
** Parameter    : i2c - I2C device
** Return value : I2C_ERR  
**                I2C_OK    
*******************************************************************************/
I2C_ReturnType I2C_Start( I2C_Struct * i2c );

/*******************************************************************************
** Function name: I2C_Stop( I2C_Struct * i2c)
** Description  : The function stops on i2c channel
** Parameter    : i2c - I2C device
** Return value : I2C_ERR  
**                I2C_OK    
*******************************************************************************/
I2C_ReturnType I2C_Stop( I2C_Struct * i2c );

/*******************************************************************************
** Function name: I2C_Tx( I2C_Struct * i2c)
** Description  : The function sends data on i2c channel
** Parameter    : i2c - I2C device
** Return value : I2C_OK
**                I2C_ERR
**                I2C_BUFFER_ERR
**                I2C_SEND_COMPLETE
*******************************************************************************/
I2C_ReturnType I2C_Tx( I2C_Struct * i2c );

/*******************************************************************************
** Function name: I2C_Rx( I2C_Struct * i2c)
** Description  : The function recieves data on i2c channel
** Parameter    : i2c - I2C device
** Return value : I2C_OK
**                I2C_ERR
**                2C_BUFFER_ERR
**                I2C_READ_COMPLETE
*******************************************************************************/
I2C_ReturnType I2C_Rx( I2C_Struct * i2c );

#endif /* __I2C_H__ */
