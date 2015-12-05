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
 * File name   : Usb_Drv.h
 *
 *
 * Description : The header file for USB_Drv.h
 *               
 * 
 *************************************************************************************************/

#ifndef _USBDRV_H
#define _USBDRV_H

#include "typedef.h"
#include "Usb_Bdt.h"


#define USB_BUS_ATTACHED    1
#define USB_BUS_DETACHED    0

/* USBCTL0 Initialization Parameters */
#define _USBPHYEN           0x01            /* Use internal transceiver*/
#define _USBPUEN            0x40            /* Use internal pull-up resistor*/
#define _USBREGEN           0x04            /* Use the internal regulator*/
#define _USBRESET           0x80

#define USB_RST             _USBRESET
#define UCFG_VAL            _USBPUEN|_USBPHYEN |_USBREGEN

/******************************************************************************
 * USB - STAT Register
 *       bit 7 6 5 4 3 2 1 0
 *           | | | | | | | |
 *           | | | | | |
 *           | | | | | |_ODD
 *           | | | | |___IN
 *           |_|_|_|_____ENDP 
 *
 *****************************************************************************/
#define EP_CTRL     0x0C            /* Cfg Control pipe for this endpoint */
#define EP_OUT      0x08            /* Cfg OUT only pipe for this endpoint*/
#define EP_IN       0x04            /* Cfg IN only pipe for this endpoint */
#define HSHK_EN     0x01            /* Enable handshake packet              */
                                    /* Handshake should be disable for isochorous transfer*/


#define OUT         0
#define IN          1

#define EP_NUM_MASK   0b11110000
#define EP_DIR_MASK   0b00001000
#define EP_EVEN_MASK  0b00000100

#define EP00_OUT    ((0x00<<4)|(OUT<<3))
#define EP00_IN     ((0x00<<4)|(IN<<3))
#define EP01_OUT    ((0x01<<4)|(OUT<<3))
#define EP01_IN     ((0x01<<4)|(IN<<3))
#define EP02_OUT    ((0x02<<4)|(OUT<<3))
#define EP02_IN     ((0x02<<4)|(IN<<3))
#define EP03_OUT    ((0x03<<4)|(OUT<<3))
#define EP03_IN     ((0x03<<4)|(IN<<3))
#define EP04_OUT    ((0x04<<4)|(OUT<<3))
#define EP04_IN     ((0x04<<4)|(IN<<3))
#define EP05_OUT    ((0x05<<4)|(OUT<<3))
#define EP05_IN     ((0x05<<4)|(IN<<3))
#define EP06_OUT    ((0x06<<4)|(OUT<<3))
#define EP06_IN     ((0x06<<4)|(IN<<3))

#define WAKE_UP_BY_RESET      0x01
#define SET_ADDR_IN_CONFIG    0x02


extern byte Usb_Device_State;
extern USB_DEVICE_STATUS Usb_Stat;
extern byte Usb_Active_Cfg;
extern byte Usb_Alt_Intf[MAX_NUM_INTF];

extern  CTRL_TRANSFER_SETUP Setup_Pkt;
extern  CTRL_TRANSFER_DATA CtrlTrf_Data;
extern  BDTMAP Bdtmap;		

extern byte Usb_State_Flag;

extern pFunc Ep0_CallBackFunc;

extern void  Initialize_USBModule(void);
extern void  USB_Buf_Rdy(BUFF_DSC *buffer_dsc);




void Check_USBBus_Status(void);
void USB_Remote_Wakeup(void);
void USB_Soft_Detach(void); 

void Clear_Mem(byte* startAdr,byte count);

#endif