/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : IFsh1.h
**     Project     : ProcessorExpert
**     Processor   : MC9S08JS16CWJ
**     Component   : IntFLASH
**     Version     : Component 02.409, Driver 01.31, CPU db: 3.00.017
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2015-10-19, 15:13, # CodeGen: 61
**     Abstract    :
**         This component "IntFLASH" implements an access to internal FLASH.
**         The component support reading/writing data into FLASH, erasing of
**         selected sector.
**         The component supports events if the write interrupt is supported.
**         The component supports following modes of write operations:
**           - Write - writing without any test.
**           - Destructive write - sector is erased if necessary.
**           - Safe write - user event is invoked to save and resore data
**                          from the current sector.
**         The component requires on-chip FLASH memory (not used/allocated by
**         other components).
**     Settings    :
**         Total FLASH memory size       : 0x4000 bytes
**         Number of FLASH memory blocks : 1
**         Flash block
**           Address range               : 0xC000-0xFFFF
**           Size                        : 0x4000
**           Sector size                 : 0x0200
**         Write method                  : Safe write (with save & erase)
**         Wait in RAM                   : yes
**           Buffer type                 : Implemented by the component
**         Virtual page                  : Disabled
**         CPU clock/speed selection
**           FLASH clock                 : 187.5 kHz
**           High speed mode             : This component enabled
**     Contents    :
**         SetByteFlash  - byte IFsh1_SetByteFlash(IFsh1_TAddress Addr, byte Data);
**         SetWordFlash  - byte IFsh1_SetWordFlash(IFsh1_TAddress Addr, word Data);
**         SetLongFlash  - byte IFsh1_SetLongFlash(IFsh1_TAddress Addr, dword Data);
**         GetLongFlash  - byte IFsh1_GetLongFlash(IFsh1_TAddress Addr, dword *Data);
**         SetBlockFlash - byte IFsh1_SetBlockFlash(IFsh1_TDataAddress Source, IFsh1_TAddress Dest, word...
**         GetBlockFlash - byte IFsh1_GetBlockFlash(IFsh1_TAddress Source, IFsh1_TDataAddress Dest, word...
**         DataPtr2Addr  - IFsh1_TAddress IFsh1_DataPtr2Addr(void* Addr);
**         FuncPtr2Addr  - IFsh1_TAddress IFsh1_FuncPtr2Addr(void(*Addr)());
**
**     Copyright : 1997 - 2014 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file IFsh1.h
** @version 01.31
** @brief
**         This component "IntFLASH" implements an access to internal FLASH.
**         The component support reading/writing data into FLASH, erasing of
**         selected sector.
**         The component supports events if the write interrupt is supported.
**         The component supports following modes of write operations:
**           - Write - writing without any test.
**           - Destructive write - sector is erased if necessary.
**           - Safe write - user event is invoked to save and resore data
**                          from the current sector.
**         The component requires on-chip FLASH memory (not used/allocated by
**         other components).
*/         
/*!
**  @addtogroup IFsh1_module IFsh1 module documentation
**  @{
*/         

#ifndef __IFsh1
#define __IFsh1

/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
//#include "PE_Timer.h"
/* Include inherited components */

#include "Cpu.h"
/* MODULE IFsh1. */

/* Number of flash memory areas. */
#define IFsh1_AREA_COUNT                1
/* Flash area 0 start address (in the format used by component methods). */
#define IFsh1_AREA_0_START              0xC000U
/* Flash area 0 size (in bytes). */
#define IFsh1_AREA_0_SIZE               0x4000U
/* Flash area 0 last address (in the format used by component methods). */
#define IFsh1_AREA_0_END                0xFFFFU
/* Flash area 0 sector size: minimal erasable unit (in bytes). */
#define IFsh1_AREA_0_SECTOR_SIZE        0x0200U
/* Flash area sector size valid for all areas: minimal erasable unit (in bytes). */
#define IFsh1_AREA_SECTOR_SIZE          0x0200U
/* Size of programming phrase, i.e. number of bytes that must be programmed at once */
#define IFsh1_PROGRAMMING_PHRASE       (0x01U)
/* TRUE if the component setting allows clear bits of already programmed flash memory location without destruction of the value in surrounding addresses by sector erase. */
#define IFsh1_ALLOW_CLEAR              (TRUE)


#ifndef __BWUserType_IFsh1_TAddress
#define __BWUserType_IFsh1_TAddress
   typedef word IFsh1_TAddress;        /* A type for address to the FLASH.*/
#endif
#ifndef __BWUserType_IFsh1_TDataAddress
#define __BWUserType_IFsh1_TDataAddress
  typedef byte *IFsh1_TDataAddress;    /* A type for data address. */
#endif


byte IFsh1_SetByteFlash(IFsh1_TAddress Addr, byte Data);
/*
** ===================================================================
**     Method      :  IFsh1_SetByteFlash (component IntFLASH)
*/
/*!
**     @brief
**         Writes a byte to an address in FLASH. The operation of this
**         method depends on the "Write method" property and state of
**         the flash. Please see more details on general info page of
**         the help.
**     @param
**         Addr            - Address to FLASH.
**     @param
**         Data            - Data to write.
**     @return
**                         - Error code, possible codes: 
**                           - ERR_OK - OK 
**                           - ERR_NOTAVAIL - Desired program/erase
**                           operation is not available 
**                           - ERR_RANGE - Address is out of range 
**                           - ERR_VALUE - Read value is not equal to
**                           written value 
**                           - ERR_SPEED - This device does not work in
**                           the active speed mode 
**                           - ERR_BUSY - Device is busy 
**                           - ERR_PROTECT - Flash is write protect
*/
/* ===================================================================*/

byte IFsh1_SetWordFlash(IFsh1_TAddress Addr, word Data);
/*
** ===================================================================
**     Method      :  IFsh1_SetWordFlash (component IntFLASH)
*/
/*!
**     @brief
**         Writes a word to an address in FLASH. The operation of this
**         method depends on the "Write method" property and state of
**         the flash. Please see more details on general info page of
**         the help.
**     @param
**         Addr            - Address to FLASH.
**     @param
**         Data            - Data to write.
**     @return
**                         - Error code, possible codes: 
**                           - ERR_OK - OK 
**                           - ERR_NOTAVAIL - Desired program/erase
**                           operation is not available 
**                           - ERR_RANGE - Address is out of range 
**                           - ERR_VALUE - Read value is not equal to
**                           written value 
**                           - ERR_SPEED - This device does not work in
**                           the active speed mode 
**                           - ERR_BUSY - Device is busy 
**                           - ERR_PROTECT - Flash is write protect
*/
/* ===================================================================*/

byte IFsh1_SetLongFlash(IFsh1_TAddress Addr, dword Data);
/*
** ===================================================================
**     Method      :  IFsh1_SetLongFlash (component IntFLASH)
*/
/*!
**     @brief
**         Writes a long word to an address in FLASH. The operation of
**         this method depends on the "Write method" property and state
**         of the flash. Please see more details on general info page
**         of the help.
**     @param
**         Addr            - Address to FLASH.
**     @param
**         Data            - Data to write.
**     @return
**                         - Error code, possible codes: 
**                           - ERR_OK - OK 
**                           - ERR_NOTAVAIL - Desired program/erase
**                           operation is not available 
**                           - ERR_RANGE - The address is out of range 
**                           - ERR_VALUE - The read value is not equal
**                           to the written value 
**                           - ERR_SPEED - This device does not work in
**                           the active speed mode 
**                           - ERR_BUSY - Device is busy 
**                           - ERR_PROTECT - Flash is write protect
*/
/* ===================================================================*/

byte IFsh1_GetLongFlash(IFsh1_TAddress Addr, dword *Data);
/*
** ===================================================================
**     Method      :  IFsh1_GetLongFlash (component IntFLASH)
*/
/*!
**     @brief
**         Gets a long word from an address in FLASH.
**     @param
**         Addr            - Address to FLASH.
**     @param
**         Data            - A pointer to the returned 32-bit data.
**     @return
**                         - Error code, possible codes: 
**                           - ERR_OK - OK 
**                           - ERR_NOTAVAIL - Desired program/erase
**                           operation is not available 
**                           - ERR_RANGE - The address is out of range 
**                           - ERR_BUSY - Device is busy
*/
/* ===================================================================*/

byte IFsh1_SetBlockFlash(IFsh1_TDataAddress Source, IFsh1_TAddress Dest, word Count);
/*
** ===================================================================
**     Method      :  IFsh1_SetBlockFlash (component IntFLASH)
*/
/*!
**     @brief
**         Writes data to FLASH. The operation of this method depends
**         on the "Write method" property and state of the flash.
**         Please see more details on general info page of the help.
**     @param
**         Source          - Source address of the data.
**     @param
**         Dest            - Destination address in FLASH.
**     @param
**         Count           - Count of the data fields (in the
**                           smallest addressable units).
**     @return
**                         - Error code, possible codes: 
**                           - ERR_OK - OK 
**                           - ERR_NOTAVAIL - Desired program/erase
**                           operation is not available 
**                           - ERR_RANGE - The address is out of range 
**                           - ERR_BUSY - Device is busy 
**                           - ERR_SPEED - This device does not work in
**                           the active speed mode 
**                           - ERR_PROTECT - Flash is write protect 
**                           - ERR_VALUE - Read value is not equal to
**                           written value
*/
/* ===================================================================*/

byte IFsh1_GetBlockFlash(IFsh1_TAddress Source, IFsh1_TDataAddress Dest, word Count);
/*
** ===================================================================
**     Method      :  IFsh1_GetBlockFlash (component IntFLASH)
*/
/*!
**     @brief
**         Reads data from FLASH.
**     @param
**         Source          - Destination address in FLASH.
**     @param
**         Dest            - Source address of the data.
**     @param
**         Count           - Count of the data fields (in the
**                           smallest addressable units).
**     @return
**                         - Error code, possible codes: 
**                           - ERR_OK - OK 
**                           - ERR_NOTAVAIL - Desired program/erase
**                           operation is not available 
**                           - ERR_RANGE - The address is out of range 
**                           - ERR_BUSY - Device is busy
*/
/* ===================================================================*/

void IFsh1_Init(void);
/*
** ===================================================================
**     Method      :  IFsh1_Init (component IntFLASH)
**
**     Description :
**         Description_Init - Initializes the associated peripheral(s) 
**         and the component's internal variables. The method is called 
**         automatically as a part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

#define IFsh1_DataPtr2Addr(_Val) \
  /*lint -save  -e923 Disable MISRA rule (11.3) checking. */\
  ((IFsh1_TAddress)_Val) \
  /*lint -restore Enable MISRA rule (11.3) checking. */
/*
** ===================================================================
**     Method      :  IFsh1_DataPtr2Addr (component IntFLASH)
*/
/*!
**     @brief
**         This method converts data pointer to format of a component's
**         method address parameter. Generally a data pointer format is
**         different from format of a method Addr parameter.
**     @param
**         Addr            - Data pointer.
**     @return
**                         - Address in the format used in the component
**                           methods.
*/
/* ===================================================================*/

#define IFsh1_FuncPtr2Addr(_Val) \
  /*lint -save  -e923 Disable MISRA rule (11.3) checking. */\
  ((IFsh1_TAddress)_Val) \
  /*lint -restore Enable MISRA rule (11.3) checking. */
/*
** ===================================================================
**     Method      :  IFsh1_FuncPtr2Addr (component IntFLASH)
*/
/*!
**     @brief
**         This method converts function pointer to format of a
**         component's method address parameter. Generally a function
**         pointer format is different from format of a method Addr
**         parameter.
**     @param
**         Addr            - Function pointer.
**     @return
**                         - Address in the format used in the component
**                           methods.
*/
/* ===================================================================*/


byte IFsh1_NonDestructiveUnsecureWrite(IFsh1_TAddress src, IFsh1_TAddress dst, word size);
/*
** ===================================================================
**     Method      :  NonDestructiveUnsecureWrite (component IntFLASH)
**
**     Description :
**         This method performs Non-destructive unsecure write.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

byte IFsh1_WriteBlock(IFsh1_TAddress src, IFsh1_TAddress dst, word size);
/*
** ===================================================================
**     Method      :  WriteBlock (component IntFLASH)
**
**     Description :
**         The method writes block of data to a flash memory.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

/* END IFsh1. */

#endif /* ifndef __IFsh1 */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
