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

#ifndef _TYPEDEF_H
#define _TYPEDEF_H

typedef unsigned char   byte;           // 8-bit
typedef unsigned int    word;           // 16-bit
typedef unsigned long   dword;          // 32-bit

typedef union _BYTE
{
    byte _byte;
    struct
    {
        unsigned b0:1;
        unsigned b1:1;
        unsigned b2:1;
        unsigned b3:1;
        unsigned b4:1;
        unsigned b5:1;
        unsigned b6:1;
        unsigned b7:1;
    }Bit;
} BYTE;

typedef union _WORD
{
    word _word;
    struct
    {
        byte byte1;
        byte byte0;
    }_byte;
    struct
    {
        BYTE HighB;
        BYTE LowB;
    }_Byte;
} WORD;

#define LSB(a)      ((a)._byte.byte0)
#define MSB(a)      ((a)._byte.byte1)

typedef union _DWORD
{
    dword _dword;
    struct
    {
        byte byte3;
        byte byte2;
        byte byte1;
        byte byte0;
    }_byte;
    struct
    {
        word word1;
        word word0;
    }_word;
    struct
    {
        BYTE Byte3;
        BYTE Byte2;
        BYTE Byte1;
        BYTE Byte0;
    }_Byte;
    struct
    {
        WORD Word1;
        WORD Word0;
    }_Word;
} DWORD;

#define LOWER_LSB(a)    ((a)._byte.byte0)
#define LOWER_MSB(a)    ((a)._byte.byte1)
#define UPPER_LSB(a)    ((a)._byte.byte2)
#define UPPER_MSB(a)    ((a)._byte.byte3)

typedef void(* pFunc)(void);

typedef union _POINTER
{
    struct
    {
        byte bHigh;
        byte bLow;
    }_byte;
    
    word _word;                   // bLow & bHigh
    byte* bMem;                   // 2 bytes pointer pointing
    word* wMem;                   // 2 bytes poitner pointing
                                 
} PTR_TYPE;

#define bool byte
#define true (1)
#define false (0)

#endif            
