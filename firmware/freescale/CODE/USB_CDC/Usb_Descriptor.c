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
 * File name   : Usb_Description.c
 * Description : This file defines the USB descriptor and intizlize them 
 * 
 *************************************************************************************************/

#include "USB.h"
#include "typedef.h"
#include "Usb_Descriptor.h"
#include "Usb_Config.h"
#include "Usb_Ep_Handlers.h"

  
  
const USB_DEV_DSC Device_Dsc =
{    
    sizeof(USB_DEV_DSC),    /* Size of this descriptor in bytes     */
    DSC_DEV,                /* DEVICE descriptor type               */
    0x0002,                 /* USB Spec Release Number in BCD format*/
    VENDOR_SPECIFIC_DEVICE, /* Class Code                           */
    0x00,                   /* Subclass code                        */
    0x00,                   /* Protocol code                        */
    EP0_BUFF_SIZE,          /* Max packet size for EP0,             */
    0x0000,                 /* Vendor ID		//little-endian for USB */
    0x0100,                 /* Product ID:	//little-endian for USB */
    0x3101,                 /* Device release number in BCD format  */
    0x01,                   /* Manufacturer string index            */
    0x02,                   /* Product string index                 */
    0x03,                   /* Device serial number string index    */
    0x01                    /* Number of possible configurations    */
};

 const USB_CFG  Cfg_01={
    {
    sizeof(USB_CFG_DSC),    /* Size of this descriptor in bytes   */
    DSC_CFG,                /* CONFIGURATION descriptor type      */
    (word) ((sizeof(USB_CFG)<<8) | (sizeof(USB_CFG)>>8)),          /* Total length of data for this cfg*/
    1,                      /* Number of interfaces in this cfg   */
    1,                      /* Index value of this configuration  */
    0,                      /* Configuration string index         */
    _DEFAULT,               /* Attributes, see usbdefs_std_dsc.h  */
    50                      /* Max power consumption (2X mA)      */
    },
    /* Interface Descriptor */
    {
    sizeof(USB_INTF_DSC),   /* Size of this descriptor in bytes   */
    DSC_INTF,               /* INTERFACE descriptor type          */
    0,                      /* Interface Number                   */
    0,                      /* Alternate Setting Number           */
    3,                      /* Number of endpoints in this intf   */
    CLASS_NULL,             /* Class code                         */
    0,                      /* Subclass code                      */
    0,                      /* Protocol code                      */
    0                       /* Interface string index             */
    },
    
    /* Endpoint Descriptor*/
    {sizeof(USB_EP_DSC),DSC_EP,_EP02_OUT,_BULK,(word)(OUT_EP_SIZE << 8),0},
    {sizeof(USB_EP_DSC),DSC_EP,_EP03_IN,_BULK,(word)(IN_EP_SIZE << 8),0},    
    {sizeof(USB_EP_DSC),DSC_EP,_EP04_OUT,_BULK,(word)(OUT4_EP_SIZE << 8),0}   
};

struct
{
  byte bLength;
  byte bDscType;
  word string[1];
} const sd000 
= {sizeof(sd000),DSC_STR,0x0904};

struct
{
  byte bLength;
  byte bDscType;
  byte string[18*2]; //'rapidradio project'
} const sd001
  ={
    sizeof(sd001),DSC_STR,
    'r',0,'a',0,'p',0,'i',0,'d',0,'r',0,'a',0,'d',0,'i',0,'o',0,' ',0,'p',0,'r',0,'o',0,'j',0,'e',0,'c',0,'t',0
   }; 
       
struct
{
  byte bLength;
  byte bDscType;
  byte string[14*2]; //'rapidradio USB'
} const sd002
={
    sizeof(sd002),DSC_STR,
    'r',0,'a',0,'p',0,'i',0,'d',0,'r',0,'a',0,'d',0,'i',0,'o',0,' ',0,'U',0,'S',0,'B',0
   }; 
   
struct
{
  byte bLength;
  byte bDscType;
  byte string[6*2]; //'v1.3.1'
} const sd003
={
    sizeof(sd003),DSC_STR,
    'v','0','1','0','.','0','3','0','.','0','1','0'
   }; 
   
    
unsigned char* Str_Des[] = 
    { (unsigned char*)&sd000,(unsigned char*)&sd001,(unsigned char*)&sd002,(unsigned char*)&sd003};   
    
unsigned char* Cfg_Des[] =
    {(unsigned char*)&Cfg_01,(unsigned char*)&Cfg_01 };
    
pFunc Class_Req_Handler[1] = { (pFunc)0 };


