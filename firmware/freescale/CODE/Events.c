/** ###################################################################
**     Filename  : Events.c
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
/* MODULE Events */


#include "Cpu.h"
#include "Events.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

/*
	The rapidradio project
	
	Author: Michal Okulski, The RapidRadio Team
	Website: http://rapidradio.pl
	Email: michal@rapidradio.pl
	Created: 11/2015
	
	Inspired by AVR's RFM70 libraries. 
	
	------------------------------------------------------------------------------------
	The MIT License (MIT)

	Copyright (c) 2015 Michal Okulski (micas.pro)

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
	------------------------------------------------------------------------------------
*/

#include "cmd_parser.h"
#include "settings.h"
#include "Led.h"

volatile bool RFM_IRQ;
volatile uint32_t Millis;

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
/*void SPI_OnRxChar(void)
{
  SPI_receivedFlag = 1;
}
*/

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
void TI1_OnInterrupt(void)
{  
  static bool ledOn = false;
  
  Millis += 50;
  if (settings.state == RFM_IDLE) {
    
    if (ledOn) {
      if (Millis >= 100UL) {        
        Led_ClrVal();
        Millis = 0UL;
        ledOn = false;
      }      
    } else {
      if (Millis >= 2000UL) {        
        Led_SetVal();
        Millis = 0UL;
        ledOn = true;
      }
    }
  }
}

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
void RfmIrq_OnInterrupt(void)
{
  /* place your RfmIrq interrupt procedure body here*/
  RFM_IRQ = true;
  KBIES_KBEDG0 = 0;
}

/* END Events */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
