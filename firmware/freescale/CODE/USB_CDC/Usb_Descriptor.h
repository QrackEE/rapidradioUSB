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
 * File name   : UsbDescriptor.h
 *
 * Description : This file defines the USB standard descriptor 
 * 
 *************************************************************************************************/
#include "typedef.h"
#include "Usb_Ep_Handlers.h"
#include "Usb_Config.h"


#ifndef USBDEFS_STD_DSC_H
#define USBDEFS_STD_DSC_H



/* Descriptor Types*/
#define DSC_DEV     0x01
#define DSC_CFG     0x02
#define DSC_STR     0x03
#define DSC_INTF    0x04
#define DSC_EP      0x05


 /* USB standard endpoint address format: dir:X:X:X:EP3:EP2:EP1:EP0*/
#define _EP01_OUT   0x01
#define _EP01_IN    0x81
#define _EP02_OUT   0x02
#define _EP02_IN    0x82
#define _EP03_OUT   0x03
#define _EP03_IN    0x83
#define _EP04_OUT   0x04
#define _EP04_IN    0x84
#define _EP05_OUT   0x05
#define _EP05_IN    0x85
#define _EP06_OUT   0x06
#define _EP06_IN    0x86

/* Endpoint transfer type*/
#define _CTRL       0x00            //control transfer
#define _ISO        0x01            //isochronous transfer
#define _BULK       0x02            //bulk transfer
#define _INT        0x03            //interrupt transfer

/*isochronous endpoint synchronization type */
#define _NS         0x00<<2         //No Synchronization
#define _AS         0x01<<2         //Asynchronous
#define _AD         0x02<<2         //Adaptive
#define _SY         0x03<<2         //Synchronous

/*sochronous Endpoint Usage Type */
#define _DE         0x00<<4         //Data endpoint
#define _FE         0x01<<4         //Feedback endpoint
#define _IE         0x02<<4         //Implicit feedback Data endpoint


/*USB device descriptor structure*/
typedef struct _USB_DEV_DSC
{
    byte bLength;
    byte bDscType;
    word bcdUSB;
    byte bDevCls;
    byte bDevSubCls;
    byte bDevProtocol;
    byte bMaxPktSize0;
    word idVendor;
    word idProduct;
    word bcdDevice;
    byte iMFR;
    byte iProduct;
    byte iSerialNum;
    byte bNumCfg;
} USB_DEV_DSC;


/*USB configuration descriptor structure*/
typedef struct _USB_CFG_DSC
{
    byte bLength;
    byte bDscType;
    word wTotalLength;
    byte bNumIntf;
    byte bCfgValue;
    byte iCfg;
    byte bmAttributes;
    byte bMaxPower;
} USB_CFG_DSC;

/* USB interface descriptor structure*/
typedef struct _USB_INTF_DSC
{
    byte bLength;
    byte bDscType;
    byte bIntfNum;
    byte bAltSetting;
    byte bNumEPs;
    byte bIntfCls;
    byte bIntfSubCls;
    byte bIntfProtocol;
    byte iIntf;
} USB_INTF_DSC;

/*USB endpoint descriptor structure*/
typedef struct _USB_EP_DSC
{
    byte bLength;
    byte bDscType;
    byte bEPAdr;
    byte bmAttributes;
    word wMaxPktSize;
    byte bInterval;
} USB_EP_DSC;



typedef struct _USB_CFG                            
{   
    USB_CFG_DSC     Cfg_Dsc_01;                  
    USB_INTF_DSC    Int_Dsc_Intf00Alt00;                
    //CDC_HEADER_FN_DSC   Cdc_Header_Fn_Intf01Alt00;
    //CDC_ACM_FN_DSC      Cdc_Acm_Fn_Intf01Alt00;
    //CDC_UNION_FN_DSC    Cdc_Union_Fn_Intf01Alt00;
    //CDC_CALL_MGT_FN_DSC Cdc_Call_Mgt_Fn_Intf01Alt00;
    //USB_EP_DSC      Ep3_Dsc_Intf01Alt00;
    //USB_EP_DSC      Ep4_Dsc_Intf01Alt00;
    //USB_INTF_DSC    Int_Dsc_Intf02Alt00;                
    USB_EP_DSC      Ep2_Dsc_Intf01Alt00;          
    USB_EP_DSC      Ep3_Dsc_Intf01Alt00;
    USB_EP_DSC      Ep4_Dsc_Intf01Alt00;
    //USB_EP_DSC      Ep3_Dsc_Intf02Alt00;          
    //USB_EP_DSC      Ep4_Dsc_Intf02Alt00;          
}USB_CFG;


extern unsigned char* Str_Des[];
extern unsigned char* Cfg_Des[];
extern const USB_CFG  Cfg_01;
extern const USB_DEV_DSC Device_Dsc;
extern pFunc Class_Req_Handler[1] ;



#endif 