/*
	The rapidradio project
	
	Author: Michal Okulski, the rapidradio team
	Website: www.rapidradio.pl
	Email: michal@rapidradio.pl
	
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

#ifndef CMD_PARSER_H
#define CMD_PARSER_H

#include "typedef.h"
#include <stddef.h>
#include "PE_Types.h"

// cmd + newline separator
#define LONGEST_CMD_LENGTH      23

#define CONFIG_TOKEN_TIMEOUT        5000
#define CONFIG_TOKEN_MIN_TIMEOUT    1000
#define CONFIG_TIMEOUT              30000
#define WAIT_BEFORE_SEND            100

/*
Allowed commands:

rr_channel=10
rr_tx_addr=0x12345678
rr_rx_addr=0x12345678
rr_verbose=1
rr_packnum=1
rr_use_ack=1
rr_exit
rr_status

*/

typedef enum
{
	RegularMode,
	ConfigTokenFound,
	ConfigMode
} OperationMode;

extern volatile OperationMode Mode;
extern volatile uint32_t Millis;

extern void InitParser();
extern void ParseBytes(const uint8_t *buff, const uint8_t length);
extern void TimeTick();
extern void SendToRapidradioIfNeeded();

#endif