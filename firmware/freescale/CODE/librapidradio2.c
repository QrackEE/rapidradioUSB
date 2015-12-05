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

#include <stdio.h>
#include "rapidradio.h"
#include "SPI.h"
#include "Events.h"
#include "Usb_Ep_Handlers.h"

const char *Str_Error_MaxRT = "Transmit Error: MAX_RT";
const char *Str_Error_TxFull = "Transmit Error: TX_FULL";
const char *Str_Error_Retrying = "Retrying...";
const char *Str_Error_Cant_Send = "Unable to send packet";

uint8_t getMode(void) 
{
	return readRegVal(RFM7x_REG_CONFIG) & RFM7x_PIN_PRIM_RX;
}

// Important! adr has to be 5-bytes length even if MSB bytes are unused
uint8_t configRxPipe(uint8_t pipe_nr, uint8_t *adr, uint8_t plLen, uint8_t en_aa)
{

	uint8_t tmp;
	uint8_t nr = pipe_nr -1;

	if(plLen > 32 || nr > 5 || en_aa > 1)
		return 0;

	// write address
	if(nr<2)      // full length for rx pipe 0 an 1
		writeRegCmdBuf(RFM7x_CMD_WRITE_REG | (RFM7x_REG_RX_ADDR_P0 + nr), adr, 5);
	else // only LSB for pipes 2..5
		writeRegVal(RFM7x_CMD_WRITE_REG | (RFM7x_REG_RX_ADDR_P0 + nr), adr[0]); 

	// static
	if (plLen) {
		// set payload len
		writeRegVal(RFM7x_CMD_WRITE_REG | (RFM7x_REG_RX_PW_P0 + nr), plLen);
		// set EN_AA bit
		tmp = readRegVal(RFM7x_REG_EN_AA);
		if (en_aa)
		tmp |= 1 << nr;
		else
		tmp &= ~(1 << nr);
		writeRegVal(RFM7x_CMD_WRITE_REG | RFM7x_REG_EN_AA, tmp);
		// clear DPL bit
		tmp = readRegVal(RFM7x_REG_DYNPD);
		tmp &= ~(1 << nr);
		writeRegVal(RFM7x_CMD_WRITE_REG | RFM7x_REG_DYNPD, tmp);        
		// set Enable pipe bit
		enableRxPipe(nr);
	}
	// dynamic
	else 
	{
		// set payload len to default
		writeRegVal(RFM7x_CMD_WRITE_REG | (RFM7x_REG_RX_PW_P0 + nr), 0x20);
		// set EN_AA bit
		tmp = readRegVal(RFM7x_REG_EN_AA);
		tmp |= 1 << nr;
		writeRegVal(RFM7x_CMD_WRITE_REG | RFM7x_REG_EN_AA, tmp);
		// set DPL bit
		tmp = readRegVal(RFM7x_REG_DYNPD);
		tmp |= 1 << nr;
		writeRegVal(RFM7x_CMD_WRITE_REG | RFM7x_REG_DYNPD, tmp);
		// set Enable pipe bit
		enableRxPipe(nr);
	}
	return 1;
}

void enableRxPipe(uint8_t pipe_nr) 
{
	uint8_t nr = pipe_nr - 1;
	uint8_t tmp;
	
	if (nr > 5) return;
	
	// set Enable pipe bit
	tmp = readRegVal(RFM7x_REG_EN_RXADDR);
	tmp |= 1 << nr;
	writeRegVal(RFM7x_CMD_WRITE_REG | RFM7x_REG_EN_RXADDR, tmp);
}

void disableRxPipe(uint8_t pipe_nr) 
{
	uint8_t nr = pipe_nr - 1;
	uint8_t tmp;
	
	if (nr > 5) return;
	
	// set Enable pipe bit
	tmp = readRegVal(RFM7x_REG_EN_RXADDR);
	tmp &= ~(1 << nr);
	writeRegVal(RFM7x_CMD_WRITE_REG | RFM7x_REG_EN_RXADDR, tmp);
}

void StartSending(const Settings *s) {
  uint8_t adr[5];
  
  setModeTX();
  
  memcpy(adr, s->address, 4);
	adr[4] = 0;
	configTxPipe(adr, TX_DPL);
  
  setChannel(s->channel);    
}

void StartListening(const Settings *s) {
  uint8_t adr[5];
  
  setModeRX();
    		
	(void)memcpy(adr, s->address, 4);
	adr[4] = 0;
	if (!configRxPipe(1, adr, 0, 1))
	{
	  PrintError("Rx Pipe configure error");
	}
	
	setChannel(s->channel);	
	
	RFM_IRQ = false;
}

bool Received(uint8_t *buff, uint8_t *length) {
  *length = receivePayload(buff);

	if (*length > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

TransmitStatus Send(const uint8_t *buff, const uint8_t length) {
  uint8_t status;
  uint32_t c = 0UL;
  uint32_t c2 = 0UL;
  uint8_t useAck = 0; 
 
  CE_HIGH;
  
	RFM_IRQ = false;			
		
	status = readRegVal(RFM7x_REG_STATUS);
	
	if (status & (RFM7x_IRQ_STATUS_MAX_RT | RFM7x_IRQ_STATUS_TX_FULL | RFM7x_IRQ_STATUS_TX_DS))
	{
    writeRegVal(RFM7x_CMD_WRITE_REG | RFM7x_REG_STATUS, status);
	}
	
	if (settings.ack) {
	  useAck = 1;
	}
	
	sendPayload(buff, length, useAck);	
	
	while(!RFM_IRQ && ++c2 < 10000UL) {
	  c = 0UL;	  
	  while(!RFM_IRQ && ++c < 10UL) {}
	  status = readRegVal(RFM7x_REG_STATUS);		
	  if (status & (RFM7x_IRQ_STATUS_MAX_RT | RFM7x_IRQ_STATUS_TX_FULL | RFM7x_IRQ_STATUS_TX_DS)) break;
	}
	
	CE_LOW;
	
	if (RFM_IRQ) {	  
	  status = readRegVal(RFM7x_REG_STATUS);		
	}
	
	// clear error flags
	if (status & (RFM7x_IRQ_STATUS_MAX_RT | RFM7x_IRQ_STATUS_TX_FULL))
	{
		writeRegVal(RFM7x_CMD_WRITE_REG | RFM7x_REG_STATUS, status);
	}	
	
	if (status & RFM7x_IRQ_STATUS_MAX_RT) {
	  return MaxRT;
	}
	
	if (status & RFM7x_IRQ_STATUS_TX_FULL) {
	  return FifoFull;
	}
			
	return Success;
}

void SendWithRetry(const int buffAdr, const uint8_t len) {
  bool sentSuccessfully = false;
  uint8_t i;
  uint8_t *buff = (uint8_t *)buffAdr;
  
  for(i=0;i<settings.retries;i++) {  
    
    if (i>0 && settings.verbose) {
      PrintError(Str_Error_Retrying);
      SendWait();
    }
  
    switch(Send(buff, len)) {
      case MaxRT:
        sentSuccessfully = false;
        flushTxFIFO();
        if (settings.verbose) {
          PrintError(Str_Error_MaxRT);
        }
      break;
      
      case FifoFull:
        sentSuccessfully = false;
        flushTxFIFO();
        if (settings.verbose) {
          PrintError(Str_Error_TxFull);
        }
      break;
      
      case Success:
        sentSuccessfully = true;
      break;
    }
    
    if (sentSuccessfully) break;
    
    if (settings.verbose) {      
      SendWait();    
    }
    
    Cpu_Delay100US(1000);
  }
  
  if (!sentSuccessfully && settings.verbose) {
    PrintError(Str_Error_Cant_Send);
    SendWait(); 
  }
}


