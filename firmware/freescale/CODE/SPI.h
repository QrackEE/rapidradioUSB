/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : SPI.h
**     Project   : rapidradioUSB
**     Processor : MC9S08JS16CWJ
**     Component : SynchroMaster
**     Version   : Component 02.335, Driver 01.29, CPU db: 3.00.015
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 2015-10-18, 22:32
**     Abstract  :
**         This component "SynchroMaster" implements MASTER part of synchronous
**         serial master-slave communication.
**     Settings  :
**         Synchro type                : MASTER
**
**         Serial channel              : SPI
**
**         Protocol
**             Init baud rate          : 6MHz
**             Clock edge              : falling
**             Width                   : 8 bits
**             Empty character         : 0
**             Empty char. on input    : RECEIVED
**
**         Registers
**             Input buffer            : SPIDL     [$0035]
**             Output buffer           : SPIDL     [$0035]
**             Control register        : SPIC1     [$0030]
**             Mode register           : SPIC2     [$0031]
**             Baud setting reg.       : SPIBR     [$0032]
**
**
**
**         Used pins                   :
**         ----------------------------------------------------------
**              Function    | On package |    Name
**         ----------------------------------------------------------
**               Input      |     9      |  PTA1_KBIP1_MISO
**               Output     |     10     |  PTA2_KBIP2_MOSI
**               Clock      |     11     |  PTA3_KBIP3_SPSCK
**         ----------------------------------------------------------
**
**     Contents  :
**         RecvChar - byte SPI_RecvChar(SPI_TComData *Chr);
**         SendChar - byte SPI_SendChar(SPI_TComData Chr);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef __SPI
#define __SPI

/* MODULE SPI. */

#include "Cpu.h"



#define SPI_EOF 0x00                   /* Empty character */

#ifndef __BWUserType_SPI_TComData
#define __BWUserType_SPI_TComData
  typedef byte SPI_TComData;           /* User type for communication. */
#endif

#ifndef __BWUserType_SPI_TError
#define __BWUserType_SPI_TError
  typedef union {
    byte err;
    struct {
      bool OverRun  : 1;   /* OverRun error flag - the data overflow on the input has been detected. Both hardware detection (if supported) and software detection (when the value of Input buffer size property is 0) is used. */
      bool RxBufOvf : 1;   /* Rx buffer full error flag - the input circular buffer defined by the Input buffer size property has overrun. */
      bool FaultErr : 1;   /* Fault mode error flag - only if supported by hardware */
    }errName;
  } SPI_TError;                        /* Error flags. For languages which don't support bit access is byte access only to error flags possible.  */
#endif




#define SPI_RecvChar(Chr) \
  ((byte) (SPIS_SPRF? ((*(SPI_TComData*)(Chr)) = SPIDL),(byte)ERR_OK:(byte)ERR_RXEMPTY))
/*
** ===================================================================
**     Method      :  SPI_RecvChar (component SynchroMaster)
**
**     Description :
**         If any data is received, this method returns one character,
**         otherwise it returns an error code (it does not wait for
**         data). 
**         For information about SW overrun behavior please see
**         <General info page>.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - A pointer to the received character
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK - The valid data is received.
**                           ERR_SPEED - This device does not work in
**                           the active speed mode.
**                           ERR_RXEMPTY - No data in receiver.
**                           ERR_OVERRUN - Overrun error was detected
**                           from the last char or block received. In
**                           polling mode, this error code is returned
**                           only when the hardware supports detection
**                           of the overrun error. The valid data is
**                           also received.
**                           ERR_FAULT - Fault error was detected from
**                           the last char or block received. In the
**                           polling mode the ERR_FAULT is return until
**                           the user clear the fault flag bit, but in
**                           the interrupt mode ERR_FAULT is returned
**                           only once after the fault error occured.
**                           This error is supported only on the CPUs
**                           supports the faul mode function - where
**                           <Fault mode> property is available.
** ===================================================================
*/

#define  SPI_SendChar(Chr) \
 ((byte) (SPIS_SPTEF?(SPIDL = (SPI_TComData)(Chr)), (byte)ERR_OK:(byte)ERR_TXFULL))
/*
** ===================================================================
**     Method      :  SPI_SendChar (component SynchroMaster)
**
**     Description :
**         Sends one character to the channel.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Chr             - Character to send
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - Device is disabled (only if
**                           output DMA is supported and enabled)
**                           ERR_TXFULL - Transmitter is full
** ===================================================================
*/

void SPI_Init(void);
/*
** ===================================================================
**     Method      :  SPI_Init (component SynchroMaster)
**
**     Description :
**         Initializes the associated peripheral(s) and the component 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/


/* END SPI. */

#endif /* ifndef __SPI */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
