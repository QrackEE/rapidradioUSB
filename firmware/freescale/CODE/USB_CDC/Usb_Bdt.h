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

/*************************************************************************************************
 * File name   : Usb_Bdt.h
 *
 *
 * Description : The BDT definition for USB module 
 * 
 *************************************************************************************************/

#ifndef _USBBDT_H
#define _USBBDT_H

#include "typedef.h"
#include "Usb_Config.h"
#include "Usb_Ep0_Handler.h"
#include "Usb_Ep_Handlers.h"


/* Buffer Descriptor Status Register Initialization Parameters */
#define _BDTSTALL      0x04                /*Buffer Stall enable*/
#define _DATA0         0x00                /*DATA0 packet expected next*/
#define _DATA1         0x40                /*DATA1 packet expected next*/
#define _DTS           0x08                /*DTS Mask*/
#define _SIE           0x80                /*SIE owns buffer*/
#define _CPU           0x00                /*CPU owns buffer*/

 
typedef union _USB_DEVICE_STATUS
{
    byte _byte;
    struct
    {
        unsigned Self_Power  :1;
        unsigned RemoteWakeup:1;        /*[0]Disabled [1]Enabled: */
    }BitCtl;
} USB_DEVICE_STATUS;

typedef union _BD_STAT
{
    byte _byte;
    struct{
        unsigned :1;
        unsigned :1;
        unsigned BDTSTALL:1;            /*Buffer Stall Enable*/
        unsigned DTS:1;                 /*Data Toggle Synch Enable*/
        unsigned :1;                    /*Address Increment Disable*/
        unsigned :1;                    /*BD Keep Enable*/
        unsigned DATA:1;                /*Data Toggle Synch Value*/
        unsigned OWN:1;                 /*USB Ownership*/
    }McuCtlBit;
    struct{
        unsigned :1;
        unsigned :1;
        unsigned PID0:1;
        unsigned PID1:1;
        unsigned PID2:1;
        unsigned PID3:1;
        unsigned :1;
        unsigned OWN:1;
    }SieCtlBit;
    struct{
        unsigned    :2;
        unsigned PID:4;                 /*Packet Identifier*/
        unsigned    :2;
    }RecPid;
} BD_STAT;                              /*Buffer Descriptor Status Register*/

typedef struct _BUFF_DSC
{
        BD_STAT Stat;
        byte Cnt;
        byte Addr;                      /*Buffer Address */
} BUFF_DSC;                                  /*Buffer Descriptor Table*/


typedef struct _BDTMAP
{ 
  BUFF_DSC ep0Bi;              /*Endpoint 0 BD In*/
  BUFF_DSC ep0Bo;              /*Endpoint 1 BD Out*/

  BUFF_DSC ep1Bio;             /*Endpoint 1 BD IN or OUT*/
  BUFF_DSC ep2Bio;             /*Endpoint 2 BD IN or OUT*/
  BUFF_DSC ep3Bio;             /*Endpoint 3 BD IN or OUT*/
  BUFF_DSC ep4Bio;             /*Endpoint 4 BD IN or OUT*/
  BUFF_DSC ep5Bio_Even;        /*Endpoint 5 BD IN or OUT  Even*/
  BUFF_DSC ep5Bio_Odd;         /*Endpoint 5 BD IN or OUT  Odd */
  BUFF_DSC ep6Bio_Even;        /*Endpoint 6 BD IN or OUT  Even */
  BUFF_DSC ep6Bio_Odd;         /*Endpoint 6 BD IN or OUT  Odd*/

  WORD Reserved;
}BDTMAP; 


#define CAL_BD_OFFSET(address)       ((byte)((address - 0x1860) >> 2))

#endif 