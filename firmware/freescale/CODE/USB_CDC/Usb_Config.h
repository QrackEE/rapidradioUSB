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
 * File name   : Usb_Config.h
 *
 * Description : This file includes the USB configuration inforation
 *               
 * 
 *************************************************************************************************/

#ifndef _USBCFG_H
#define _USBCFG_H

/* Configuration Attributes */
#define _SELFPOWER       0x01<<6         /*Self-powered  */
#define _REMOTEWAKEUP    0x01<<5         /*Remote Wakeup */
#define _DEFAULT         0x01<<7         /*Default Value */

#define EP0_BUFF_SIZE           8   /* can be 8, 16, 32, or 64 for full speed*/
#define MAX_NUM_INTF            2   /* maximum interface number*/


#define CLASS_NULL              0
#define STANDARD_CLASS_ID       1
#define CDC_CLASS_ID            2


/*USB state machine*/
#define POWERED_STATE           0
#define ATTACHED_STATE          1
#define DEFAULT_STATE           2
#define ADDR_PENDING_STATE      3
#define ADDRESS_STATE           4
#define CONFIGURED_STATE        5
#define USB_ENTER_SUSPEND       6
#define USB_SUSPEND             7


#endif 
