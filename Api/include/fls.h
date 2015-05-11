/*==============================================================================
**                                  STM32BOOT
**                      Proprietary - Copyright (C) 2014
**------------------------------------------------------------------------------
** Supported MCUs      : MCU STM32
** Supported Compilers : KeilC
**------------------------------------------------------------------------------
** File name         : Fls.h
**  
** Module name       : Flash Driver
**
** Summary: Header file of Flash driver module
**
**= History ===================================================================
** 00.00.01  6/1/2014  Cuong Chu
** - Creation
==============================================================================*/
#ifndef __DRV_FLS_H__
#define __DRV_FLS_H__
/******************************************************************************/
/* INCLUSIONS                                                                 */
/******************************************************************************/
/* types defined */
#include <stdint.h>
/******************************************************************************/
/* DEFINITION OF TYPES                                                        */
/******************************************************************************/
/* Flash function code */
typedef enum
{
  FLS_ERASE   = 1,
  FLS_PROGRAM = 2,
  FLS_VERIFY  = 3
}Fls_FunctionCodeType; /* end define */
/* Flash returns */
typedef enum
{
  FLS_OK      = 0,
  FLS_FAILED  = 1
}Fls_ReturnType;      /* end define */
/******************************************************************************/
/* DECLARATION OF GLOBAL FUNCTIONS                                            */
/******************************************************************************/
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
);
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
);
/*******************************************************************************
** Function name: Fls_BlankCheck(uint32_t Address, uint32_t SizeCheck, 
**                               uint8_t BlockPattern)
** Description  : The function called for blank check block pattern
** Parameter    : Address - Block Start Address
**                SizeCheck - Block Size
**                BlockPattern - Block Pattern
** Return value : FLS_OK - if process success
**                FLS_FAILED - if process fail
** Remarks      : Called by IF_FLASH
*******************************************************************************/
#if (0) /* not support in this version */
Fls_ReturnType BlankCheck 
(
  uint32_t Address, 
  uint32_t SizeCheck, 
  uint8_t  BlockPattern
);
#endif
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
);
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
);
/*******************************************************************************
** Function name: Fls_Verify(uint32_t Address, uint32_t PageSize, 
**                           uint8_t* DataPtr)
** Description  : The function called for blank check block pattern
** Parameter    : Address      - Page Start Address
**                PageSize     - Page Size
**                DataPtr      - Point to Data verify
** Return value : FLS_OK     - if write success
**                FLS_FAILED - if write fail
** Remarks      : Called by IF_FLASH
*******************************************************************************/
#if (0) /* not support in this version */
uint32_t Fls_Verify      
(
  uint32_t Address,   
  uint32_t PageSize,
  uint8_t  *DataPtr
);
#endif /* define of Verify function */
#endif /* __DRV_FLS_H__ */                                  
