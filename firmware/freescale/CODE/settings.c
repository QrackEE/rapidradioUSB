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

#include "settings.h"
#include "IFsh1.h"

#define SETTINGS_FLASH_ADDR             0xC000 

volatile Settings settings @SETTINGS_FLASH_ADDR;
volatile bool settingsUpdated;

void SaveSettings(const Settings *s) {
  IFsh1_SetBlockFlash(s, SETTINGS_FLASH_ADDR, sizeof(Settings));
  settingsUpdated = true;
}

void SetDefaultSettings(void) {

  Settings defs;

  if (settings._unused_start != 0xA1 || settings._unused_end != 0xB2) {        
    
    defs._unused_start = 0xA1;
    
    defs.address[3] = 0xAA;
    defs.address[2] = 0xBB;
    defs.address[1] = 0x0;
    defs.address[0] = 0x01;   
    
    defs.channel = 55;
    defs.ack = true;
    defs.verbose = true;
    defs.retries = 10;
    defs.state = RFM_IDLE;
    
    defs._unused_end = 0xB2;
    
    // save defaults
    SaveSettings(&defs);
  }
  
  // force IDLE state at the beginning
  defs = settings;
  defs.state = RFM_IDLE;
  SaveSettings(&defs);
}