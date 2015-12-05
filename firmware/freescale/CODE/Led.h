/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : Led.h
**     Project   : rapidradioUSB
**     Processor : MC9S08JS16CWJ
**     Component : BitIO
**     Version   : Component 02.075, Driver 03.24, CPU db: 3.00.015
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 2015-10-07, 09:57
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
**                       12            |  PTA4_KBIP4_SS
**             ----------------------------------------------------
**
**         Port name                   : PTA
**
**         Bit number (in port)        : 4
**         Bit mask of the port        : $0010
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
**         PutVal - void Led_PutVal(bool Val);
**         ClrVal - void Led_ClrVal(void);
**         SetVal - void Led_SetVal(void);
**         NegVal - void Led_NegVal(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef Led_H_
#define Led_H_

/* MODULE Led. */

  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"


/*
** ===================================================================
**     Method      :  Led_PutVal (component BitIO)
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
void Led_PutVal(bool Val);

/*
** ===================================================================
**     Method      :  Led_ClrVal (component BitIO)
**
**     Description :
**         This method clears (sets to zero) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define Led_ClrVal() ( \
    (void)clrReg8Bits(PTAD, 0x10)      /* PTAD4=0x00 */ \
  )

/*
** ===================================================================
**     Method      :  Led_SetVal (component BitIO)
**
**     Description :
**         This method sets (sets to one) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define Led_SetVal() ( \
    (void)setReg8Bits(PTAD, 0x10)      /* PTAD4=0x01 */ \
  )

/*
** ===================================================================
**     Method      :  Led_NegVal (component BitIO)
**
**     Description :
**         This method negates (inverts) the output value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define Led_NegVal() ( \
    (void)invertReg8Bits(PTAD, 0x10)   /* PTAD4=invert */ \
  )



/* END Led. */
#endif /* #ifndef __Led_H_ */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
