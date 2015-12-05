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

#ifndef _USB_EP_HANDLERS_H_
#define _USB_EP_HANDLERS_H_

#include "USB.h"

#define VENDOR_SPECIFIC_DEVICE      0xFF

/* Functional Descriptors */
/* Type Values for the bDscType Field */
#define CS_INTERFACE                0x24
#define CS_ENDPOINT                 0x25

/* Bulk IN transfer states */
#define TX_READY                0
#define TX_BUSY                 1
#define TX_BUSY_ZLP             2       // ZLP: Zero Length Packet
#define TX_COMPLETING           3


/* Endpoints */
#define UEP_BULKOUT1_CTL    EPCTL1
#define BD_BULKOUT1         Bdtmap.ep1Bio

#define UEP_BULKOUT_CTL     EPCTL2
#define BD_BULKOUT          Bdtmap.ep2Bio
 
#define UEP_BULKIN_CTL      EPCTL3
#define BD_BULKIN           Bdtmap.ep3Bio

#define UEP_BULKOUT4_CTL    EPCTL4
#define BD_BULKOUT4         Bdtmap.ep4Bio

#define OUT_EP_SIZE     32
#define OUT4_EP_SIZE    16
#define IN_EP_SIZE      32


typedef struct _RFM_Register {

  uint8_t _unused_start;
  uint8_t bank;
  uint8_t reg;
  uint8_t value[11];
  uint8_t valueLength;
  uint8_t _unused_end;
  
} RFM_Register;


#define RX_BUF_LENGTH            (int8_t)(33)

extern volatile char *usbRxBuff;
extern volatile uint8_t usbRxLength;

extern void Bulk_Data_Process(void);
extern void Init_EP(void);
extern void Receive_Bulk2_Data(void);
extern void Receive_Bulk4_Data(void);

extern void Print(const char *txt);
extern void PrintError(const char *txt);
extern void PrintInfo(const char *txt);

// count has to be max 32 bytes
extern void Output(const uint8_t *bytes, const uint8_t count);

extern void SendWait();


#endif