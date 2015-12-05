/** ###################################################################
**     Filename  : Events.h
**     Project   : rapidradioUSB
**     Processor : MC9S08JS16CWJ
**     Component : Events
**     Version   : Driver 01.02
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 2015-08-19, 12:42
**     Abstract  :
**         This is user's event module.
**         Put your event handler code here.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Led.h"
#include "SPI.h"
#include "USB.h"
#include "CSN.h"
#include "RfmIrq.h"
#include "TI1.h"
#include "typedef.h"

extern volatile bool RFM_IRQ;


void SPI_OnRxChar(void);
/*
** ===================================================================
**     Event       :  SPI_OnRxChar (module Events)
**
**     Component   :  SPI [SynchroMaster]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**         
**         [ Version specific information for Freescale HCS08 and RS08
**         and HCS12 and HCS12X and ColdFireV1 derivatives ] 
**         The <Bidirectional mode> property setting doesn't affect
**         this event.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void TI1_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  TI1_OnInterrupt (module Events)
**
**     Component   :  TI1 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void RfmIrq_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  RfmIrq_OnInterrupt (module Events)
**
**     Component   :  RfmIrq [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/* END Events */
#endif /* __Events_H*/

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
