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


#include "derivative.h"
#include "typedef.h"
#include "Usb_Config.h"
#include "Usb_Bdt.h"
#include "Usb_Ep0_Handler.h"
#include "Usb_Ep_Handlers.h"
#include "Usb_Descriptor.h"
#include "Usb_Drv.h"
#include <stdio.h>
#include <string.h>
#include "cmd_parser.h"
#include "settings.h"
#include "rapidradio.h"

/* Local variable definition */
char Bulk_In_Buf[IN_EP_SIZE] @0x18E0;
char Bulk_Out_Buf[OUT_EP_SIZE] @0x18C0;
char Bulk_Out4_Buf[OUT4_EP_SIZE] @0x18A0;

volatile char *usbRxBuff = (volatile char *)0x1900;
volatile uint8_t usbRxLength = 0;

char TxBuf[32];
char Tx_Index;


/* Global function definition*/
void Bulk_Data_Transfer(char *buffer, byte len);

void SendWait() {
  uint32_t c = 0;

  while (Tx_Index && ++c < 100000UL) {    
    Bulk_Data_Process();
  }
    
  if (c >= 100000UL) {
    // force it!
    Tx_Index = 0;
  }
}

void Bulk_Data_Process(void)
{
  if(! BD_BULKIN.Stat.McuCtlBit.OWN) 
  {
    if(Tx_Index > 0) 
    {
      Bulk_Data_Transfer(TxBuf, Tx_Index);
      Tx_Index = 0;
    }
  }
}
  
void Print(const char *txt) {
  uint8_t len = strlen(txt);

  (void)strncpy(TxBuf+Tx_Index, txt, len);
  TxBuf[31] = 0;
  Tx_Index += len;  
}

void PrintError(const char *txt) {
  uint8_t len = strlen(txt);

  (void)strncpy(TxBuf, "[ERR][", 6);
  (void)strncpy(TxBuf+6, txt, len);
  (void)strncpy(TxBuf+6+len, "]\r\n", 3);
  TxBuf[6+len+3] = 0;
  Tx_Index = 6+len+3;
}

void PrintInfo(const char *txt) {
  uint8_t len = strlen(txt);

  (void)strncpy(TxBuf, "[INF][", 6);
  (void)strncpy(TxBuf+6, txt, len);
  (void)strncpy(TxBuf+6+len, "]\r\n", 3);
  TxBuf[6+len+3] = 0;
  Tx_Index = 6+len+3;
} 

void Output(const uint8_t *bytes, const uint8_t count) {
  (void)memcpy(TxBuf+Tx_Index, bytes, count);
  Tx_Index += count;
}

void Init_EP(void)
{   
    UEP_BULKIN_CTL = EP_IN|HSHK_EN;                
    UEP_BULKOUT_CTL = EP_OUT|HSHK_EN;
    UEP_BULKOUT4_CTL = EP_OUT|HSHK_EN;
    
   
    BD_BULKIN.Addr = 0x20;      
    BD_BULKIN.Stat._byte = _CPU|_DATA1;   
    
    BD_BULKOUT.Cnt = sizeof(Bulk_Out_Buf);    
    BD_BULKOUT.Addr =  0x18;    
    BD_BULKOUT.Stat._byte = _SIE|_DATA0|_DTS; 
        
    BD_BULKOUT4.Cnt = sizeof(Bulk_Out4_Buf);
    BD_BULKOUT4.Addr = 0x10;       
    BD_BULKOUT4.Stat._byte = _SIE|_DATA0|_DTS;                  
    
    Tx_Index = 0;
    
    return;
}


void Bulk_Data_Transfer(char *buffer, byte len)
{
  byte i;

  if(len > IN_EP_SIZE)   
    len = IN_EP_SIZE;

  /* Copy data from user's buffer to dual-ram buffer*/
  for (i = 0; i < len; i++)
    Bulk_In_Buf[i] = buffer[i];

  BD_BULKIN.Cnt = len;
  USB_Buf_Rdy(&BD_BULKIN);
}


void Receive_Bulk2_Data(void)
{
  byte Rx_Len;
            
  if(!BD_BULKOUT.Stat.McuCtlBit.OWN)
  {
    Rx_Len = BD_BULKOUT.Cnt;
    memcpy(usbRxBuff, Bulk_Out_Buf, Rx_Len);  
    usbRxLength = Rx_Len;
  }         
}

void Receive_Bulk4_Data(void)
{
  byte Rx_Len;
  Settings settingsCopy;
  RFM_Register rfmReg;
  uint8_t regVal[12];
            
  if(!BD_BULKOUT4.Stat.McuCtlBit.OWN)
  {      
    Rx_Len = BD_BULKOUT4.Cnt;
    
    // check if settings have been transmitted in this endpoint
    if (Rx_Len == sizeof(Settings)) {
    
      memcpy(&settingsCopy, Bulk_Out4_Buf, Rx_Len);
      if (settingsCopy._unused_start == 0xA1 && settingsCopy._unused_end == 0xB2) {        
        SaveSettings(&settingsCopy);
      }
      
    // or maybe registers  
    } else if (Rx_Len == sizeof(RFM_Register)) {
      memcpy(&rfmReg, Bulk_Out4_Buf, Rx_Len);
      if (rfmReg._unused_start == 0xA2 && rfmReg._unused_end == 0xB3) {
      
        // select bank
        if (rfmReg.bank < 2 && rfmReg.valueLength < 12) {
          selectBank(rfmReg.bank);
        
          regVal[0] = RFM7x_CMD_WRITE_REG | rfmReg.reg;
          memcpy(regVal + 1, rfmReg.value, rfmReg.valueLength);
          writeRegPgmBuf(regVal, rfmReg.valueLength + 1);
        
          selectBank(0);
        }      
      }
    }

    BD_BULKOUT4.Cnt = OUT4_EP_SIZE;
    USB_Buf_Rdy(&BD_BULKOUT4);                
  }
}



