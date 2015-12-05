/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : CSN.c
**     Project   : rapidradioUSB
**     Processor : MC9S08JS16CWJ
**     Component : BitIO
**     Version   : Component 02.075, Driver 03.24, CPU db: 3.00.015
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 2015-09-19, 00:58
**     Abstract  :
**         This component "BitIO" implements an one-bit input/output.
**         It uses one bit/pin of a port.
**         Note: This component is set to work in Output direction only.
**         Methods of this component are mostly implemented as a macros
**         (if supported by target language and compiler).
**     Settings  :
**         Used pin                    :
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       20            |  PTA7_KBIP7_TxD
**             ----------------------------------------------------
**
**         Port name                   : PTA
**
**         Bit number (in port)        : 7
**         Bit mask of the port        : $0080
**
**         Initial direction           : Output (direction cannot be changed)
**         Initial output value        : 0
**         Initial pull option         : off
**
**         Port data register          : PTAD      [$0000]
**         Port control register       : PTADD     [$0001]
**
**         Optimization for            : speed
**     Contents  :
**         GetVal - bool CSN_GetVal(void);
**         PutVal - void CSN_PutVal(bool Val);
**         ClrVal - void CSN_ClrVal(void);
**         SetVal - void CSN_SetVal(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

/* MODULE CSN. */

#include "CSN.h"
  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"


/*
** ===================================================================
**     Method      :  CSN_GetVal (component BitIO)
**
**     Description :
**         This method returns an input value.
**           a) direction = Input  : reads the input value from the
**                                   pin and returns it
**           b) direction = Output : returns the last written value
**         Note: This component is set to work in Output direction only.
**     Parameters  : None
**     Returns     :
**         ---             - Input value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)

** ===================================================================
*/
/*
bool CSN_GetVal(void)

**  This method is implemented as a macro. See CSN.h file.  **
*/

/*
** ===================================================================
**     Method      :  CSN_PutVal (component BitIO)
**
**     Description :
**         This method writes the new output value.
**     Parameters  :
**         NAME       - DESCRIPTION
**         Val             - Output value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)
**     Returns     : Nothing
** ===================================================================
*/
void CSN_PutVal(bool Val)
{
  if (Val) {
    setReg8Bits(PTAD, 0x80);           /* PTAD7=0x01 */
  } else { /* !Val */
    clrReg8Bits(PTAD, 0x80);           /* PTAD7=0x00 */
  } /* !Val */
}

/*
** ===================================================================
**     Method      :  CSN_ClrVal (component BitIO)
**
**     Description :
**         This method clears (sets to zero) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void CSN_ClrVal(void)

**  This method is implemented as a macro. See CSN.h file.  **
*/

/*
** ===================================================================
**     Method      :  CSN_SetVal (component BitIO)
**
**     Description :
**         This method sets (sets to one) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void CSN_SetVal(void)

**  This method is implemented as a macro. See CSN.h file.  **
*/


/* END CSN. */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
