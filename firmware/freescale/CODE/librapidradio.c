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

bool initRegisters();

//commands
const uint8_t PROGMEM RFM7x_cmd_adrRX0[] = { (0x20 | 0x0A), 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
const uint8_t PROGMEM RFM7x_cmd_adrTX[]  = { (0x20 | 0x10), 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
const uint8_t PROGMEM RFM7x_cmd_adrRX1[] = { (0x20 | 0x0B), 0x35, 0x43, 0x10, 0x10, 0x02};
const uint8_t PROGMEM RFM7x_cmd_switch_cfg[] = { 0x50, 0x53 }; // 
const uint8_t PROGMEM RFM7x_cmd_flush_rx[] = { 0xe2, 0x00 }; // flush RX FIFO
const uint8_t PROGMEM RFM7x_cmd_flush_tx[] = { 0xe1, 0x00 }; // flush TX FIFO
const uint8_t PROGMEM RFM7x_cmd_activate[] = { 0x50, 0x73 }; // 
const uint8_t PROGMEM RFM7x_cmd_tog1[] = { (0x20 | 0x04), 0xF9 | 0x06, 0x96, 0x82, 0xDB };
const uint8_t PROGMEM RFM7x_cmd_tog2[] = { (0x20 | 0x04), 0xF9, 0x96, 0x82, 0xDB}; 
//const uint8_t PROGMEM RFM7x_cmd_tog1[] = { (0x20 | 0x04), 0xDB | 0x06, 0x82, 0x96, 0xF9 };
//const uint8_t PROGMEM RFM7x_cmd_tog2[] = { (0x20 | 0x04), (0xDB | 0x06) & 0xF9, 0x82, 0x96, 0xF9};

//RFM75
//************ Bank0 register initialization commands
const uint8_t PROGMEM RFM7x_bank0Init[][2] = {
  // address data
  { (0x20 | 0x00), 0x0F }, //Disable CRC ,CRC=1byte, POWER UP, RX
  { (0x20 | 0x01), 0x3F }, //Enable auto acknowledgement data pipe0-5
  { (0x20 | 0x02), 0x3F }, //Enable RX Addresses pipe0-5
  { (0x20 | 0x03), 0x03 }, //RX/TX address field width 4byte
  { (0x20 | 0x04), 0xFF }, //4ms, 15 retries
  { (0x20 | 0x05), 0x17 }, //channel = 0x17
  { (0x20 | 0x06), 0x2F }, //init register 6 (2M, LNA gain high, 5dBM) //2F
  { (0x20 | 0x07), 0x07 }, //
  { (0x20 | 0x08), 0x00 }, //
  { (0x20 | 0x09), 0x00 }, //
  { (0x20 | 0x0C), 0xc3 }, //LSB Addr pipe 2
  { (0x20 | 0x0D), 0xc4 }, //LSB Addr pipe 3
  { (0x20 | 0x0E), 0xc5 }, //LSB Addr pipe 4
  { (0x20 | 0x0F), 0xc6 }, //LSB Addr pipe 5
  { (0x20 | 0x11), 0x20 }, //Payload len pipe0
  { (0x20 | 0x12), 0x20 }, //Payload len pipe0
  { (0x20 | 0x13), 0x20 }, //Payload len pipe0
  { (0x20 | 0x14), 0x20 }, //Payload len pipe0
  { (0x20 | 0x15), 0x20 }, //Payload len pipe0
  { (0x20 | 0x16), 0x20 }, //Payload len pipe0
  { (0x20 | 0x17), 0x00 }, //0x20
  { (0x20 | 0x1C), 0x3F }, //Enable dynamic payload legth data pipe0-5
  { (0x20 | 0x1D), 0x07 } //Enables Dynamic Payload Length,Enables Payload with ACK
};

//************ Bank1 register initialization commands
const uint8_t PROGMEM RFM7x_bank1Init[][5] = {
	{ (0x20 | 0x00), 0x40, 0x4B, 0x01, 0xE2 },
	{ (0x20 | 0x01), 0xC0, 0x4B, 0x00, 0x00 },
	{ (0x20 | 0x02), 0xD0, 0xFC, 0x8C, 0x02 },
	{ (0x20 | 0x03), 0x99, 0x00, 0x39, 0x41 },
	//{ (0x20 | 0x04), 0xDB, 0x82, 0x96, 0xF9 },	//-- ?
	{ (0x20 | 0x04), 0xF9, 0x96, 0x82, 0xDB },
	//{ (0x20 | 0x05), 0xB6, 0x0F, 0x06, 0x24 },  //-- ?
	{ (0x20 | 0x05), 0x24, 0x06, 0x0F, 0xB6 }, 
	{ (0x20 | 0x06), 0x00, 0x00, 0x00, 0x00 },
	{ (0x20 | 0x07), 0x00, 0x00, 0x00, 0x00 },
	{ (0x20 | 0x08), 0x00, 0x00, 0x00, 0x00 },
	{ (0x20 | 0x09), 0x00, 0x00, 0x00, 0x00 },
	{ (0x20 | 0x0a), 0x00, 0x00, 0x00, 0x00 },
	{ (0x20 | 0x0b), 0x00, 0x00, 0x00, 0x00 },
	{ (0x20 | 0x0C), 0x00, 0x12, 0x73, 0x05 },
	{ (0x20 | 0x0D), 0x36, 0xb4, 0x80, 0x00 }
};

//Bank1 register 14
const uint8_t PROGMEM RFM7x_bank1R0EInit[] = {
  (0x20 | 0x0E), 0x41, 0x20, 0x08, 0x04, 0x81, 0x20, 0xCF, 0xF7, 0xFE, 0xFF, 0xFF
};


void SPI_MasterInit(void)
{	
	
}

bool rfm_init()
{

	
	// doesn't work on my RPi with Raspbian
	//bcm2835_gpio_fen(IRQ);
	
	CE_LOW;
	CSN_HIGH;
	
	SPI_MasterInit();
	
	_delay_ms(RFM7x_BEGIN_INIT_WAIT_MS);
	return initRegisters();
}

bool initRegisters()
{
  uint8_t test = 1;
  int i;

	// init bank 0 registers
	selectBank(0);

	// !! The last two regs in the bank0Init list will be handled later
	
	for (i = 0; i < 20; i++)
	writeRegVal(pgm_read_byte(&RFM7x_bank0Init[i][0]), pgm_read_byte(&RFM7x_bank0Init[i][1]));

	// init address registers in bank 0
	writeRegPgmBuf((uint8_t *)RFM7x_cmd_adrRX0, sizeof(RFM7x_cmd_adrRX0));
	writeRegPgmBuf((uint8_t *)RFM7x_cmd_adrRX1, sizeof(RFM7x_cmd_adrRX1));
	writeRegPgmBuf((uint8_t *)RFM7x_cmd_adrTX, sizeof(RFM7x_cmd_adrTX));

	// activate Feature register
	if(!readRegVal(RFM7x_REG_FEATURE))
	writeRegPgmBuf((uint8_t *)RFM7x_cmd_activate, sizeof(RFM7x_cmd_activate));

	// now set Registers 1D and 1C
	writeRegVal(pgm_read_byte(&RFM7x_bank0Init[22][0]), pgm_read_byte(&RFM7x_bank0Init[22][1]));
	writeRegVal(pgm_read_byte(&RFM7x_bank0Init[21][0]), pgm_read_byte(&RFM7x_bank0Init[21][1]));

	// init bank 1 registers
	selectBank(1);

	for (i=0; i < 14; i++)
	writeRegPgmBuf((uint8_t *)RFM7x_bank1Init[i], sizeof(RFM7x_bank1Init[i]));

	// set ramp curve
	writeRegPgmBuf((uint8_t *)RFM7x_bank1R0EInit, sizeof(RFM7x_bank1R0EInit));

	// do we have to toggle some bits here like in the example code?
	writeRegPgmBuf((uint8_t *)RFM7x_cmd_tog1, sizeof(RFM7x_cmd_tog1));
	writeRegPgmBuf((uint8_t *)RFM7x_cmd_tog2, sizeof(RFM7x_cmd_tog2));

	_delay_ms(RFM7x_END_INIT_WAIT_MS);		

	selectBank(0);
	setModeRX();
	
	selectBank(1);	
	
	//Check the ChipID
	if (readRegVal(0x08) != 0x63)
	{
		return false;
	}
	
	selectBank(0);
	
	return true;
}

uint8_t transmitSPI(uint8_t val)
{	
  uint8_t read = 0; 
  while (!(SPIS & 0x20));
  SPID = val;
  while (!(SPIS & 0x80));
  read = SPID;
  return read;
}

void selectBank(uint8_t bank) 
{
	uint8_t tmp = readRegVal(0x07) & 0x80;
	if(bank) 
	{
		if(!tmp)
			writeRegPgmBuf((uint8_t *)RFM7x_cmd_switch_cfg, sizeof(RFM7x_cmd_switch_cfg));
	} 
	else 
	{
		if(tmp)
			writeRegPgmBuf((uint8_t *)RFM7x_cmd_switch_cfg, sizeof(RFM7x_cmd_switch_cfg));
	}
}

void setModeRX(void)
{
	uint8_t val;
	writeRegPgmBuf((uint8_t *)RFM7x_cmd_flush_rx, sizeof(RFM7x_cmd_flush_rx)); 
	val = readRegVal(RFM7x_REG_STATUS);
	writeRegVal(RFM7x_CMD_WRITE_REG | RFM7x_REG_STATUS, val); 
	CE_LOW;
	val=readRegVal(RFM7x_REG_CONFIG);
	val |= RFM7x_PIN_PRIM_RX;
	writeRegVal(RFM7x_CMD_WRITE_REG | RFM7x_REG_CONFIG, val); 
	CE_HIGH;
}


void setModeTX(void) 
{
	uint8_t val;
	writeRegPgmBuf((uint8_t *)RFM7x_cmd_flush_tx, sizeof(RFM7x_cmd_flush_tx));
	val = readRegVal(RFM7x_REG_STATUS); 
	writeRegVal(RFM7x_CMD_WRITE_REG | RFM7x_REG_STATUS, val);
	CE_LOW;
	val=readRegVal(RFM7x_REG_CONFIG);
	val &= ~RFM7x_PIN_PRIM_RX;
	writeRegVal(RFM7x_CMD_WRITE_REG | RFM7x_REG_CONFIG, val); 
	
	CE_HIGH;
}



void setChannel(uint8_t cnum) 
{
	writeRegVal( RFM7x_CMD_WRITE_REG | RFM7x_REG_RF_CH, cnum);
}



void setPower(uint8_t pwr) 
{
  uint8_t tmp;
	if (pwr > 3) return;
	tmp = readRegVal(RFM7x_REG_RF_SETUP);
	tmp &= 0xF9;
	tmp |= pwr << 1;
	
	writeRegVal(RFM7x_CMD_WRITE_REG | RFM7x_REG_RF_SETUP, tmp);
}

uint8_t readRegVal(uint8_t cmd)  
{
	uint8_t res = 1;
	CSN_LOW;
	_delay_ms(RFM7x_CS_DELAY);
	transmitSPI(cmd);
	res = transmitSPI(0);
	CSN_HIGH;
	_delay_ms(RFM7x_CS_DELAY);
	return res;
}

uint8_t writeRegVal(uint8_t cmd, uint8_t val) 
{	
	CSN_LOW;
	_delay_ms(RFM7x_CS_DELAY);
	transmitSPI(cmd);
	transmitSPI(val);
	CSN_HIGH;
	_delay_ms(RFM7x_CS_DELAY);
	return 1;
}

void readRegBuf(uint8_t reg, uint8_t * buf, uint8_t len) 
{
	uint8_t byte_ctr;
	CSN_LOW;
	_delay_ms(RFM7x_CS_DELAY);
	transmitSPI(reg); 
	for(byte_ctr = 0; byte_ctr < len; byte_ctr++)
		buf[byte_ctr] = transmitSPI(0); 
	CSN_HIGH;
	_delay_ms(RFM7x_CS_DELAY);
}



uint8_t writeRegPgmBuf(uint8_t * cmdbuf, uint8_t len) 
{
	CSN_LOW;
	_delay_ms(RFM7x_CS_DELAY);
	while(len--) transmitSPI(pgm_read_byte(cmdbuf++));
	CSN_HIGH;
	_delay_ms(RFM7x_CS_DELAY);
	return 1;
}

uint8_t writeRegCmdBuf(uint8_t cmd, uint8_t * buf, uint8_t len)
{

	CSN_LOW;
	_delay_ms(RFM7x_CS_DELAY);
	transmitSPI(cmd);
	while(len--) transmitSPI(*(buf++));	
	CSN_HIGH;
	_delay_ms(RFM7x_CS_DELAY);
	return 1;
}



uint8_t sendPayload(const uint8_t * payload, const uint8_t _len, const uint8_t toAck)
{
  uint8_t status;
  uint8_t len;
	// turn on the radio
	CE_HIGH;
	
	// check TX_FIFO
	
	status = readRegVal(RFM7x_REG_FIFO_STATUS); 
	if (status & RFM7x_FIFO_STATUS_TX_FULL) 
	{
		return 0;
	}

	// send payload
	CSN_LOW;
	_delay_ms(RFM7x_CS_DELAY);
	if(toAck == -1)
		transmitSPI(RFM7x_CMD_W_ACK_PAYLOAD);
	else if (toAck == 0)
		transmitSPI(RFM7x_CMD_W_TX_PAYLOAD_NOACK);
	else
		transmitSPI(RFM7x_CMD_WR_TX_PLOAD);
		
	len = _len;
	while(len--) transmitSPI(*(payload++));
	CSN_HIGH;
	_delay_ms(RFM7x_CS_DELAY);
	return 1;
}

uint8_t receivePayload(uint8_t *payload)
{
	uint8_t len;
	// check RX_FIFO
	uint8_t status;
	status = readRegVal(RFM7x_REG_STATUS);
	if (status & RFM7x_IRQ_STATUS_RX_DR) { // RX_DR
		
		uint8_t fifo_sta;
		len = readRegVal(RFM7x_CMD_RX_PL_WID); // Payload width
		readRegBuf(RFM7x_CMD_RD_RX_PLOAD, payload, len);
		fifo_sta = readRegVal(RFM7x_REG_FIFO_STATUS);
		
		if (fifo_sta & RFM7x_FIFO_STATUS_RX_EMPTY) {
			status|= 0x40 & 0xCF; // clear status bit rx_dr
			writeRegVal(RFM7x_CMD_WRITE_REG | RFM7x_REG_STATUS, status); 
		}
		return len;
	}
	else
	{		
		return 0;
	}
}

void flushTxFIFO() 
{
	writeRegPgmBuf((uint8_t *)RFM7x_cmd_flush_tx, sizeof(RFM7x_cmd_flush_tx));
}

void flushRxFIFO() 
{
	writeRegPgmBuf((uint8_t *)RFM7x_cmd_flush_rx, sizeof(RFM7x_cmd_flush_rx));
}

void setCE()
{
	CE_HIGH;
}

void resetCE()
{
	CE_LOW;
}

void setCSN()
{
	CSN_HIGH;
}

void resetCSN()
{
	CSN_LOW;
}

void turnOn()
{
	uint8_t status = readRegVal(RFM7x_REG_STATUS);
	status |= PWR_BIT;
	writeRegVal(RFM7x_CMD_WRITE_REG | RFM7x_REG_STATUS, status);
}

void turnOff()
{
	uint8_t status = readRegVal(RFM7x_REG_STATUS);
	status &= ~PWR_BIT;
	writeRegVal(RFM7x_CMD_WRITE_REG | RFM7x_REG_STATUS, status);
	CE_LOW;
}


void configTxPipe(uint8_t * adr, uint8_t pltype) 
{
  uint8_t tmp;
	// write TX address
	writeRegCmdBuf(RFM7x_CMD_WRITE_REG | RFM7x_REG_TX_ADDR, adr, 5);
	// write RX0 address
	writeRegCmdBuf(RFM7x_CMD_WRITE_REG | RFM7x_REG_RX_ADDR_P0, adr, 5);
	// set static or dynamic payload
	
	tmp = readRegVal(RFM7x_REG_DYNPD);
	if(pltype == TX_DPL) // dynamic
	tmp |= 1;
	else  
	tmp &= ~(1 << 0);
	writeRegVal(RFM7x_CMD_WRITE_REG | RFM7x_REG_DYNPD, tmp);
}



