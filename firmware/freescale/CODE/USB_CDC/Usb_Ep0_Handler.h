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
 * File name   : Usb_Setup_Handler.h
 *
 * Description : This file defines some structure for USB standard request 
 * 
 ************************s*************************************************************************/
#ifndef USBDEF_SETUP_PKT_H
#define USBDEF_SETUP_PKT_H

#include "typedef.h"
#include "Usb_Config.h"


/******************************************************************************
 * Standard Request Codes
 * USB 2.0 Spec Ref Table 9-4
 *****************************************************************************/
#define GET_STATUS  0
#define CLR_FEATURE 1
#define SET_FEATURE 3
#define SET_ADR     5
#define GET_DSC     6
#define SET_DSC     7
#define GET_CFG     8
#define SET_CFG     9
#define GET_INTF    10
#define SET_INTF    11
#define SYNCH_FRAME 12

/* Standard Feature Selectors */
#define DEVICE_REMOTE_WAKEUP    0x01
#define ENDPOINT_HALT           0x00

/* USB PID: Token Types - See chapter 8 in the USB specification*/
#define SETUP_TOKEN         0b00001101
#define OUT_TOKEN           0b00000001
#define IN_TOKEN            0b00001001

/* bmRequestType Definitions */
#define HOST_TO_DEV         0
#define DEV_TO_HOST         1

#define STANDARD            0x00
#define CLASS               0x01
#define VENDOR              0x02

#define RCPT_DEV            0
#define RCPT_INTF           1
#define RCPT_EP             2
#define RCPT_OTH            3

/*Control transfer state machine*/
#define WAIT_SETUP              0
#define CTL_TRF_DATA_TX         1
#define CTL_TRF_DATA_RX         2

/******************************************************************************
 * CTRL_TRANSFER_SETUP:
 *
 * There are 8 bytes for each setup packet. 
 * The buffer size can be  8, 16, 32, or 64, it is determine by EP0_BUFF_SIZE in
 * this siftware.
 *
 * These 8 bytes are defined to be directly addressable to improve access speed
 * and reduce code size.
 *
 *****************************************************************************/
typedef union _CTRL_TRANSFR_SETUP
{
    struct
    {
        byte _byte[EP0_BUFF_SIZE];
    };
    
    /* Standard Device Requests */
    struct
    {
        byte bmRequestType;
        byte bRequest;    
        word wValue;
        word wIndex;
        word wLength;
    }StdCtl;
    
    struct
    {
        unsigned :8;
        unsigned :8;
        WORD W_Value;
        WORD W_Index;
        WORD W_Length;
    }CtlPara;
    
    struct
    {
        unsigned Recipient:5;           /*Device,Interface,Endpoint,Other*/
        unsigned RequestType:2;         /*Standard,Class,Vendor,Reserved*/
        unsigned DataDir:1;             /*Host-to-device,Device-to-host*/
        unsigned :8;
        byte bFeature;                  /*DEVICE_REMOTE_WAKEUP,ENDPOINT_HALT*/
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
    }CtlReqT;
    
    struct
    {
        unsigned :8;
        unsigned :8;
        byte bDscIndex;                 /*For Configuration and String DSC Only*/
        byte bDscType;                  /*Device,Configuration,String*/
        word wLangID;                   /*Language ID*/
        unsigned :8;
        unsigned :8;
    }ReqWval;
    
    struct
    {
        unsigned :8;
        unsigned :8;
        BYTE bDevADR;                   /*Device Address 0-127*/
        byte bDevADRH;                  /*Must equal zero*/
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
    }CtlAdd;
    
    struct
    {
        unsigned :8;
        unsigned :8;
        byte bCfgValue;                 /*Configuration Value 0-255*/
        byte bCfgRSD;                   /*Must equal zero (Reserved)*/
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
    }CtrCfg;
    
    struct
    {
        unsigned :8;
        unsigned :8;
        byte bAltID;                    /*Alternate Setting Value 0-255*/
        byte bAltID_H;                  /*Must equal zero*/
        byte bIntfID;                   /*Interface Number Value 0-255*/
        byte bIntfID_H;                 /*Must equal zero*/
        unsigned :8;
        unsigned :8;
    }CtlIntf;
    struct
    {
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
        byte bEPID;                     /*Endpoint ID (Number & Direction)*/
        byte bEPID_H;                   /*Must equal zero*/
        unsigned :8;
        unsigned :8;
    }CtlEp;
    
    struct
    {
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned EPNum:4;               /*Endpoint Number 0-15*/
        unsigned :3;
        unsigned EPDir:1;               /*Endpoint Direction: 0-OUT, 1-IN*/
        unsigned :8;
        unsigned :8;
        unsigned :8;
    }EpND;
    
} CTRL_TRANSFER_SETUP;

/******************************************************************************
 * CTRL_TRANSFER_DATA:
 * The size can be 8, 16, 32, or 64. It is specified by the value of EP0_BUFF_SIZE 
 *
 * These 8 bytes are defined to be directly addressable to improve access speed
 * and reduce code size.
 *****************************************************************************/
typedef union _CTRL_TRANSFER_DATA
{
    /*Array for indirect addressing*/
    struct
    {
        byte _byte[EP0_BUFF_SIZE];
    }ByteArray;
    
    /*First 8-byte direct addressing */
    struct
    {
        byte _byte0;
        byte _byte1;
        byte _byte2;
        byte _byte3;
        byte _byte4;
        byte _byte5;
        byte _byte6;
        byte _byte7;
    }EachByte;
    
    struct
    {
        word _word0;
        word _word1;
        word _word2;
        word _word3;
    }Eachword;

} CTRL_TRANSFER_DATA;


extern byte Ctrl_Trf_Session_Owner;   
extern byte *pSrc; 
extern byte *pObj;
extern int Transfer_Cnt;

extern void USB_CtrlTrf_SetupStage_Processing(void);
extern void USB_CtrlTrf_OUT_Handler(void);
extern void USB_CtrlTrf_IN_Handler(void);
extern void USB_Prepare_Next_Trf(void);

#endif
