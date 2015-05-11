/*==============================================================================
**                                  STM32BOOT
**                      Proprietary - Copyright (C) 2014
**------------------------------------------------------------------------------
** Supported MCUs      : MCU STM32
** Supported Compilers : KeilC
**------------------------------------------------------------------------------
** File name         : Fls.c
**  
** Module name       : Flash Driver
**
** Summary: Flash Programming Functions adapted for 
**          ST Microelectronics STM32F4xx Flash 
**
**= History ===================================================================
** 00.00.01  7/1/2014  Cuong Chu
** - Creation
==============================================================================*/
/******************************************************************************/
/* INCLUSIONS                                                                 */
/******************************************************************************/
#include "fls.h"
/******************************************************************************/
/* DEFINITION OF TYPES                                                        */
/******************************************************************************/
typedef volatile unsigned char    vu8;
typedef          unsigned char     u8;
typedef volatile unsigned short   vu16;
typedef          unsigned short    u16;
typedef volatile unsigned long    vu32;
typedef          unsigned long     u32;
/******************************************************************************/
/* DEFINITION OF MACROS                                                       */
/******************************************************************************/
#define M8(adr)  (*((vu8  *) (adr)))
#define M16(adr) (*((vu16 *) (adr)))
#define M32(adr) (*((vu32 *) (adr)))
/* Peripheral Memory Map */
/* Flash */
#define FLASH_BASE        0x40023C00
#define FLASH           ((FLASH_TypeDef*) FLASH_BASE)
/* Flash Registers */
typedef struct {
  vu32 ACR;
  vu32 KEYR;
  vu32 OPTKEYR;
  vu32 SR;
  vu32 CR;
  vu32 OPTCR;
} FLASH_TypeDef;
/* Flash Keys */
#define RDPRT_KEY       0x00A5
#define FLASH_KEY1      0x45670123
#define FLASH_KEY2      0xCDEF89AB
#define FLASH_OPTKEY1   0x08192A3B
#define FLASH_OPTKEY2   0x4C5D6E7F
/* Flash Control Register definitions */
#define FLASH_PG                ((unsigned int)0x00000001)
#define FLASH_SER               ((unsigned int)0x00000002)
#define FLASH_MER               ((unsigned int)0x00000004)
#define FLASH_MER1              ((unsigned int)0x00008000)
#define FLASH_SNB_POS           ((unsigned int)0x00000003)
#define FLASH_SNB_MSK           ((unsigned int)0x000000F8)
#define FLASH_PSIZE_POS         ((unsigned int)0x00000008)
#define FLASH_PSIZE_MSK         ((unsigned int)0x00000300)
#define FLASH_STRT              ((unsigned int)0x00010000)
#define FLASH_LOCK              ((unsigned int)0x80000000)
/* Flash Option Control Register definitions */
#define FLASH_OPTLOCK           ((unsigned int)0x00000001)
#define FLASH_OPTSTRT           ((unsigned int)0x00000002)
#define FLASH_PSIZE_Byte        ((unsigned int)0x00000000)
#define FLASH_PSIZE_HalfWord    ((unsigned int)0x00000100)
#define FLASH_PSIZE_Word        ((unsigned int)0x00000200)
#define FLASH_PSIZE_DoubleWord  ((unsigned int)0x00000300)
/* Flash Status Register definitions */
#define FLASH_EOP               ((unsigned int)0x00000001)
#define FLASH_OPERR             ((unsigned int)0x00000002)
#define FLASH_WRPERR            ((unsigned int)0x00000010)
#define FLASH_PGAERR            ((unsigned int)0x00000020)
#define FLASH_PGPERR            ((unsigned int)0x00000040)
#define FLASH_PGSERR            ((unsigned int)0x00000080)
#define FLASH_BSY               ((unsigned int)0x00010000)
#define FLASH_PGERR             (FLASH_PGSERR | FLASH_PGPERR | FLASH_PGAERR | FLASH_WRPERR)
/******************************************************************************/
/* DEFINITION OF LOCAL FUNCTION                                               */
/* (Must use static)                                                          */
/******************************************************************************/
/*******************************************************************************
** Function name: GetSecNum( uint32_t adr)
** Description  : The function called to gets number of sector from address
** Parameter    : adr - Address of sector
** Return value : n   - number of sector
** Remarks      : 
*******************************************************************************/
static uint32_t GetSecNum 
(
  uint32_t adr
)  
{
  uint32_t n;
  /* only bits 12..19 */
  n = (adr >> 12) & 0x000FF;                             
  if    (n >= 0x20) 
  {
    /* 128kB Sector */
    n = 4 + (n >> 5);                                    
  }
  else if (n >= 0x10) 
  {
    /* 64kB Sector */
    n = 3 + (n >> 4);                                   
  }
  else            
  {
    /* 16kB Sector */
    n = 0 + (n >> 2);                                    
  }
  /*  sector in second half */
  if (adr & 0x00100000)
  {
    n |=0x00000010;                                      
  }
  else
  {
    /* do nothing */
  }
  return n;
}
/*******************************************************************************
** Function name: Fls_Unlock( uint32_t Address, uint32_t Clock, 
**                                 Fls_FunctionCodeType FunctionCode)
** Description  : The function called to unlock flash
** Parameter    : Address - Device Base Address
**                Clock   - Frequency of Clock System
**                FunctionCode - Select function for erase, program or verify
** Return value : None
** Remarks      : Called by IF_FLASH
*******************************************************************************/
void Fls_Unlock
( 
  uint32_t             Address, 
  uint32_t             Clock,
  Fls_FunctionCodeType FunctionCode
)
{
  /* Unlock Flash */
  FLASH->KEYR = FLASH_KEY1;                             
  FLASH->KEYR = FLASH_KEY2;
  /* Zero Wait State, no Cache, no Prefetch */
  FLASH->ACR  = 0x00000000;                             
  /* Reset Error Flags */
  FLASH->SR  |= FLASH_PGERR;                           
}
/*******************************************************************************
** Function name: Fls_Lock(Fls_FunctionCodeType FunctionCode)
** Description  : The function called for lock flash
** Parameter    : FunctionCode - Select function for erase, program or verify
** Return value : None
** Remarks      : Called by IF_FLASH
*******************************************************************************/
void Fls_Lock
(
  Fls_FunctionCodeType FunctionCode
)
{
  /* Lock Flash */
  FLASH->CR |=  FLASH_LOCK;                             
}
/*******************************************************************************
** Function name: Fls_EraseSector(uint32_t Address)
** Description  : The function called for erase sector selected
** Parameter    : Address    - Sector Address
** Return value : FLS_OK     - if erase success
**                FLS_FAILED - if erase fail
** Remarks      : Called by IF_FLASH
*******************************************************************************/
Fls_ReturnType Fls_EraseSector
(
  uint32_t Address
)
{
  Fls_ReturnType retVal = FLS_OK;
  uint32_t n;
  /* Get Sector Number */
  n = GetSecNum(Address);                               
  /* Reset Error Flags */  
  FLASH->SR |= FLASH_PGERR;                             
  /* Sector Erase Enabled  */
  FLASH->CR  =  FLASH_SER;                              
  /* Sector Number */
  FLASH->CR |=  ((n << FLASH_SNB_POS) & FLASH_SNB_MSK); 
  /* Start Erase */
  FLASH->CR |=  FLASH_STRT;                             
  /* Reload IWDG */
  while (FLASH->SR & FLASH_BSY) 
  {
    ;                              
  }
  /* Page Erase Disabled  */
  FLASH->CR &= ~FLASH_SER;                             
  /* Check for Error */
  if (FLASH->SR & FLASH_PGERR)
  {                       
    /* Reset Error Flags */
    FLASH->SR |= FLASH_PGERR;                           
    retVal = FLS_FAILED;
  }
  else
  {
    /* do nothing */
  }
  return retVal;
}
/*******************************************************************************
** Function name: Fls_ProgramPage(uint32_t Address, uint32_t PageSize, 
**                                uint8_t* DataPtr)
** Description  : The function called for program data to on chip flash
** Parameter    : Address      - Page Start Address
**                ProgramSize  - Size of data program
**                DataPtr      - Point to Data write
** Return value: FLS_OK     - if program success
**               FLS_FAILED - if program fail
** Remarks: Called by IF_FLASH
*******************************************************************************/
Fls_ReturnType Fls_ProgramPage
(
  uint32_t Address, 
  uint32_t ProgramSize, 
  uint8_t  *DataPtr
)
{
  Fls_ReturnType retVal = FLS_OK;
  /* Adjust size for Words */
  ProgramSize = (ProgramSize + 3) & ~3;                                   
  /* Reset Error Flags */
  FLASH->SR |= FLASH_PGERR;                             
  /* reset CR */
  FLASH->CR  =  0;                                       
  while (ProgramSize) 
  {
    /* Programming Enabled & Programming Enabled (Word) */
    FLASH->CR |= (FLASH_PG | FLASH_PSIZE_Word);         
    /* Program Double Word */
    M32(Address) = *((u32 *)DataPtr);                           
    while (FLASH->SR & FLASH_BSY);
    /* Programming Disabled */
    FLASH->CR &= ~FLASH_PG;                            
    /* Check for Error */
    if (FLASH->SR & FLASH_PGERR) 
    {                      
      /* Reset Error Flags */
      FLASH->SR |= FLASH_PGERR;                        
      ProgramSize = 0;
      retVal = FLS_FAILED;
    }
    else
    {
      /* Go to next Word */
      Address += 4;                                          
      DataPtr += 4;
      ProgramSize  -= 4;
    }
  }
  return retVal;
}
/*******************************************************************************/
/*                      End of File                                            */
/*******************************************************************************/
