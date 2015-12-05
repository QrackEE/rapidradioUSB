/** ###################################################################
**     Filename  : rapidradioUSB.c
**     Project   : rapidradioUSB
**     Processor : MC9S08JS16CWJ
**     Version   : Driver 01.11
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 2015-08-19, 12:42
**     Abstract  :
**         Main module.
**         This module contains user's application code.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/
/* MODULE rapidradioUSB */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Led.h"
#include "SPI.h"
#include "USB.h"
#include "CSN.h"
#include "RfmIrq.h"
#include "TI1.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

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

#include "Usb_Drv.h"
#include "SPI.h"
#include "rapidradio.h"
#include <string.h>
#include "Usb_Ep_Handlers.h"
#include "settings.h"
#include "IFsh1.h"

										                    
										                    

void processUSB() {
  Check_USBBus_Status();
  if (Usb_Device_State == CONFIGURED_STATE) 
  {
    SendWait();
  }
}

void main(void) {
  
  /* Write your local variable definition here */
  bool initOk = false;
  uint8_t packet[32];
  uint8_t receivedBytes;  
  unsigned long int c;
  uint32_t packetsCount = 0;
  bool settingsUpdatedFlag = false;
  
  
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/  

  /* Write your code here */
  /* For example: for(;;) { } */
  
  __DI();
  TI1_DisableEvent();
  IFsh1_Init();
  __EI();
  
  KBIES_KBEDG0 = 0;
  SetDefaultSettings();
  
  /* Initialize USB Module */
  Initialize_USBModule(); 
  
  
  Led_SetVal();
  Cpu_Delay100US(1000);
  Led_ClrVal();
  Cpu_Delay100US(1000);
  Led_SetVal();
  Cpu_Delay100US(1000);
  Led_ClrVal();

  for(;;){
  
    if (!((++c) & ((1<<10)-1))) {
      if (!initOk) {      
        initOk = rfm_init();
        StartListening(&settings);
        Cpu_Delay100US(1000);
        TI1_EnableEvent();
      }
    }
    
    if (initOk) {      
    
      __DI();
      settingsUpdatedFlag = settingsUpdated;
      if (settingsUpdatedFlag) {        
        packetsCount = 0;
        
        switch (settings.state) {
          case RFM_LISTENING:
            TI1_DisableEvent();
            StartListening(&settings);
          break;
          
          case RFM_SENDING:
            TI1_DisableEvent();
            StartSending(&settings);
          break;
          
          case RFM_IDLE:
            TI1_EnableEvent();
          break;
        }
      
        settingsUpdated = false;
      }
      __EI();
      
      if (!settingsUpdatedFlag) {        
        switch (settings.state) {
          case RFM_LISTENING:
            if (RFM_IRQ) {
              RFM_IRQ = false;
              while (Received(packet, &receivedBytes)) {
                packetsCount++;
                Output(packet, receivedBytes);
                processUSB();
              }
            }
          break;
          
          case RFM_SENDING:
            if (usbRxLength > 0) {   
              SendWithRetry((int)usbRxBuff, usbRxLength);
              usbRxLength = 0;
              BD_BULKOUT.Cnt = OUT_EP_SIZE;
              USB_Buf_Rdy(&BD_BULKOUT);
            } else {
              
            }
          break;
          
          case RFM_IDLE:
            // do nothing
          break;
        }                  
      }
    }
  
    processUSB();
    
    
  }

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END rapidradioUSB */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
