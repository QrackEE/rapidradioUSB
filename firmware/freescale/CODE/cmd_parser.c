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

#include "cmd_parser.h"
#include <string.h>
#include "cdc.h"
#include <stdlib.h>
#include "rapidradio.h"
#include "TI1.h"
#include "utils.h"

#define TMP_BUFF_LENGTH       63

volatile OperationMode Mode;
volatile char tmpBuff[TMP_BUFF_LENGTH] @(0x1900+RX_BUF_LENGTH);
volatile uint8_t tmpBuffLength;
volatile uint32_t Millis;
volatile char line[10];
volatile bool sendToRapidradio;

const char *Str_Current_settings = "Current settings:\r\n";
const char *Str_Tx_Address = "TX address: 0x";
const char *Str_Rx_Address = "RX address: 0x";
const char *Str_Channel = "Channel: ";
const char *Str_Retries = "Retries: ";
const char *Str_Verbose = "Verbose: ";
const char *Str_Numbering = "Packet numbering: ";
const char *Str_Ack = "TX ACK: ";
const char *Str_Newline = "\r\n";
const char *Str_Yes = "yes";
const char *Str_No = "no";
const char *Str_OK = "OK";
const char *Str_Invalid_command = "Invalid command";
const char *Str_rapidradio_config = "rapidradioUSB config";
const char *Str_allowed_commands = "Allowed config commands:";
const char *Str_command_prefix = "rr_";
const char *Str_Error_MaxRT = "Transmit Error: MAX_RT";
const char *Str_Error_TxFull = "Transmit Error: TX_FULL";
const char *Str_Error_Retrying = "Retrying...";
const char *Str_Error_Cant_Send = "Unable to send packet";

const char *Cmd_channel = "channel";
const char *Cmd_tx_addr = "tx_addr";
const char *Cmd_rx_addr = "rx_addr";
const char *Cmd_verbose = "verbose";
const char *Cmd_packnum = "packnum";
const char *Cmd_use_ack = "use_ack";
const char *Cmd_retries = "retries";
const char *Cmd_exit = "exit";
const char *Cmd_status = "status";

void TrimLeft(const uint8_t count);

void SendWithRetry(const uint8_t *buff, const uint8_t len) {
  bool sentSuccessfully = false;
  uint8_t i;
  
  for(i=0;i<settings.retries;i++) {  
    
    if (i>0 && settings.verbose) {
      PrintError(Str_Error_Retrying);
      SendWait();
    }
  
    switch(Send(buff, len)) {
      case MaxRT:
        sentSuccessfully = false;
        if (settings.verbose) {
          PrintError(Str_Error_MaxRT);
        }
      break;
      
      case FifoFull:
        sentSuccessfully = false;
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

void SendToRapidradio(const uint8_t maxLength) {
  uint8_t toSend = 0;  
  uint8_t toCopy = 0;
  uint8_t toSendBuff[32];
  uint8_t i;
  
  
  if (!maxLength) return;
  
  if (settings.usePacketNumbering) {
    if (tmpBuffLength >= 32 && MatchesTransmissionEndToken(tmpBuff)) {
      toCopy = 32;
      txPacketNumber = 0;
    } else {           
      toSendBuff[toSend++] = txPacketNumber++;
      if (tmpBuffLength > 31) {
        toCopy = 31;
      } else {
        toCopy = tmpBuffLength;
      }
    }
  } else {
    if (tmpBuffLength > 32) {
      toCopy = 32;
    } else {
      toCopy = tmpBuffLength;
    }
  }
  
  if (!toCopy) return;
  if (toCopy > maxLength) toCopy = maxLength;
  
  for(i=0;i<toCopy;i++) {
    toSendBuff[toSend++] = tmpBuff[i];
  }
  
  TrimLeft(toCopy);
  
  SendWithRetry(toSendBuff, toSend);
  
  if (transmissionEndRequested && !tmpBuffLength) {
    SendWithRetry(transmissionEndToken, 32);
    transmissionEndRequested = false;
  }
  
  if (!tmpBuffLength) {
    sendToRapidradio = false;
  }
  
  Millis = 0;
}

void SendToRapidradioIfNeeded() {  
  if (!sendToRapidradio && !transmissionEndRequested) return;
  
  if (Mode == RegularMode) {
    if (transmissionEndRequested && !tmpBuffLength) {
      SendWithRetry(transmissionEndToken, 32);
      transmissionEndRequested = false;
    } else {      
      SendToRapidradio(32);
    }
  }
}

void TimeTick() {
  if (Mode == RegularMode && Millis >= WAIT_BEFORE_SEND && tmpBuffLength > 0) {
    sendToRapidradio = true;
  }
}

void InitParser() {
  Mode = RegularMode;
  tmpBuffLength = 0;
  Millis = 0;
}

void TrimLeft(const uint8_t count) {
 
  uint8_t i;
  
  for (i=0;i<tmpBuffLength-count;i++) {
    tmpBuff[i] = tmpBuff[i+count];
  }
  
  tmpBuffLength -= count;
  
}

void PrintAddress(const uint8_t *address) {
  
  uint8_t i;
  
  for (i=0;i<4;i++) {    
    line[i*2] = '.';
    line[i*2+1] = '.';
    itoa(address[3-i], line + (i*2), 16, 2);
  }
  
  Output(line, 8);
}

void PrintSettings() {

  Settings settingsCopy = settings;
  
  Print(Str_Current_settings);
  SendWait();
  
  Print(Str_Tx_Address);
  PrintAddress(settingsCopy.tx_address);  
  Print(Str_Newline);
  SendWait();
  
  Print(Str_Rx_Address);
  PrintAddress(settingsCopy.rx_address);
  Print(Str_Newline);
  SendWait();
  
  Print(Str_Channel);
  itoa(settingsCopy.channel, line, 10, 2);
  Output(line, 2);
  Print(Str_Newline);
  SendWait();
  
  Print(Str_Verbose);
  if (settingsCopy.verbose) {
    Print(Str_Yes);
  } else {
    Print(Str_No);
  }
  Print(Str_Newline);
  SendWait();
  
  Print(Str_Numbering);
  if (settingsCopy.usePacketNumbering) {
    Print(Str_Yes);
  } else {
    Print(Str_No);
  }
  Print(Str_Newline);
  SendWait();
  
  Print(Str_Ack);
  if (settingsCopy.ack) {
    Print(Str_Yes);
  } else {
    Print(Str_No);
  }
  Print(Str_Newline);
  SendWait();
  
  Print(Str_Retries);
  itoa(settingsCopy.retries, line, 10, 3);
  Output(line, 3);
  
  Print(Str_Newline);
  Print(Str_Newline);
  SendWait();
  
  Print(Str_allowed_commands);
  Print(Str_Newline);
  SendWait();
  
  Print(Str_command_prefix);
  Print(Cmd_channel);
  Print(Str_Newline);  
  SendWait();
  Print(Str_command_prefix);
  Print(Cmd_tx_addr);
  Print(Str_Newline);
  SendWait();
  Print(Str_command_prefix);
  Print(Cmd_rx_addr);
  Print(Str_Newline);
  SendWait();
  Print(Str_command_prefix);
  Print(Cmd_verbose);
  Print(Str_Newline);
  SendWait();
  Print(Str_command_prefix);
  Print(Cmd_packnum);
  Print(Str_Newline);
  SendWait();
  Print(Str_command_prefix);
  Print(Cmd_use_ack);
  Print(Str_Newline);
  SendWait();
  Print(Str_command_prefix);
  Print(Cmd_retries);
  Print(Str_Newline);
  SendWait();
  Print(Str_command_prefix);
  Print(Cmd_status);
  Print(Str_Newline);
  SendWait();
  Print(Str_command_prefix);
  Print(Cmd_exit);
  Print(Str_Newline);
  SendWait();   
}

void OnCommand(const char *cmd, const uint8_t length) {
  bool parsed = false;
  DWORD adr4;
  uint8_t adr5[5];
  Settings tmpSettings = settings; 
  
  if (length >= 4 && length <= LONGEST_CMD_LENGTH) {
    
    if (!strncmp(cmd+3, Cmd_exit, 4)) {
      Mode = RegularMode;
      StartListening(&settings);
      parsed = true; 
    } else if (!strncmp(cmd+3, Cmd_status, 6)) {
      PrintSettings();
      parsed = true; 
    } else if (length > 10 && cmd[10] == '=') {
      
      if (!strncmp(cmd+3, Cmd_channel, 7)) {
        adr4._dword = strtoul(cmd+11, NULL, 10);
        if (adr4._dword >= 0 && adr4._dword <= 83) {
          tmpSettings.channel = (uint8_t)adr4._dword;
          SaveSettings(&tmpSettings);
        
          setChannel((uint8_t)adr4._dword);
          parsed = true;
        }
      } else if (!strncmp(cmd+3, Cmd_tx_addr, 7)) {
        adr4._dword = strtoul(cmd+11, NULL, 16);
        
        adr5[0] = adr4._byte.byte0;
        adr5[1] = adr4._byte.byte1;
        adr5[2] = adr4._byte.byte2;
        adr5[3] = adr4._byte.byte3;
        adr5[4] = 0;
        
        memcpy(tmpSettings.tx_address, adr5, 4);
        SaveSettings(&tmpSettings);
        
        configTxPipe(adr5, TX_DPL);
        
        parsed = true;
      } else if (!strncmp(cmd+3, Cmd_rx_addr, 7)) {
        adr4._dword = strtoul(cmd+11, NULL, 16);
        
        adr5[0] = adr4._byte.byte0;
        adr5[1] = adr4._byte.byte1;
        adr5[2] = adr4._byte.byte2;
        adr5[3] = adr4._byte.byte3;
        adr5[4] = 0;
        
        memcpy(tmpSettings.rx_address, adr5, 4);
        SaveSettings(&tmpSettings);
        
        configRxPipe(1, adr5, 0, 1);
        
        parsed = true;
      } else if (!strncmp(cmd+3, Cmd_verbose, 7)) {
        if (cmd[11] == '1') {
          tmpSettings.verbose = true;
        } else if (cmd[11] == '0') {
          tmpSettings.verbose = false;          
        }
        parsed = true;
        SaveSettings(&tmpSettings);
      } else if (!strncmp(cmd+3, Cmd_packnum, 7)) {
        if (cmd[11] == '1') {
          tmpSettings.usePacketNumbering = true;
        } else if (cmd[11] == '0') {
          tmpSettings.usePacketNumbering = false;
        }
        parsed = true;
        SaveSettings(&tmpSettings);
      } else if (!strncmp(cmd+3, Cmd_use_ack, 7)) {
        if (cmd[11] == '1') {
          tmpSettings.ack = true;
        } else if (cmd[11] == '0') {
          tmpSettings.ack = false;
        }
        parsed = true;
        SaveSettings(&tmpSettings);
      } else if (!strncmp(cmd+3, Cmd_retries, 7)) {
        adr4._dword = strtoul(cmd+11, NULL, 10);
        if (adr4._dword >= 1 && adr4._dword <= 255) {
          tmpSettings.retries = (uint8_t)adr4._dword;
          SaveSettings(&tmpSettings);
        
          parsed = true;
        }
      } 
    }
  } 
  
  if (!parsed) {
    PrintError(Str_Invalid_command);
  } else {
    PrintInfo(Str_OK);
  }
}

bool LookForConfigToken() {
  
  if (tmpBuffLength > 1 && tmpBuff[tmpBuffLength-1] == '\n') {
    if ((tmpBuff[tmpBuffLength-2] == '\r' && tmpBuffLength >= 5 && tmpBuff[tmpBuffLength-5] == 'r' && tmpBuff[tmpBuffLength-4] == 'r' && tmpBuff[tmpBuffLength-3] == 'u')
      || (tmpBuffLength >= 4 && tmpBuff[tmpBuffLength-4] == 'r' && tmpBuff[tmpBuffLength-3] == 'r' && tmpBuff[tmpBuffLength-2] == 'u')) {      
      return true;
    }
  }
  
  return false;
}

void ParseBytes(const uint8_t *buff, const uint8_t length) {
 
  uint8_t trimLeft = 0;
  uint8_t i;
  int8_t cmdStart = -1;
  int8_t cmdEnd = -1;
  bool foundCommand = false;
  OperationMode mode;
  bool eventEnabled = false;
  
  if (!length) return;
  
  TI1_DisableEvent();
  
  if (tmpBuffLength + length > TMP_BUFF_LENGTH) {
    trimLeft = tmpBuffLength + length - TMP_BUFF_LENGTH;
  }  
  
  if (Mode == RegularMode) {
    SendToRapidradio(trimLeft);
  } else {
    TrimLeft(trimLeft);
  }
  
  for (i=0;i<length;i++) {
    tmpBuff[tmpBuffLength++] = buff[i];
  }        
  
  if (Mode == ConfigTokenFound) {
    if (Millis < CONFIG_TOKEN_MIN_TIMEOUT || Millis > CONFIG_TOKEN_TIMEOUT) {
      Mode = RegularMode;
    } else {
      if (length > 0 && LookForConfigToken()) {        
        tmpBuffLength = 0;
        Mode = ConfigMode;
        PrintInfo(Str_rapidradio_config);
      }
    }
  }
  
  mode = Mode;
  TI1_EnableEvent(); 
  
  if (mode == RegularMode) {
    if (LookForConfigToken()) {      
      Mode = ConfigTokenFound;
      Millis = 0;
      
      // send everything what was before the 'rru' token
      if (tmpBuff[tmpBuffLength-2] == '\r') {
        SendToRapidradio(tmpBuffLength - 5); // everything except the "rru\r\n"
      } else {
        SendToRapidradio(tmpBuffLength - 4); // everything except the "rru\n"
      }
      
      return;
    } else {
      if (tmpBuffLength >= 32) {
        SendToRapidradio(32);
      }
    }
  }        
  
  if (mode == ConfigMode) {
    TI1_DisableEvent();
    Millis = 0;
    TI1_EnableEvent();
    do {   
      foundCommand = false;
      if (tmpBuffLength >= 3) {
        
        for (i=0;i<tmpBuffLength-2;i++) {
          if (tmpBuff[i] == 'r' && tmpBuff[i+1] == 'r' && tmpBuff[i+2] == '_') {
            cmdStart = i;
            break;
          }
        }
        
        if (cmdStart >= 0) {
          for (i=cmdStart;i<tmpBuffLength;i++) {
            if (tmpBuff[i] == '\n') {
              cmdEnd = i;
              break;
            }
          }
        }
      
        if (cmdStart > 0 && cmdEnd == -1) {
          TrimLeft(cmdStart);
          return;
        }
        
        if (cmdStart >= 0 && cmdEnd > cmdStart) {
          foundCommand = true;
          if (tmpBuff[cmdEnd-1] == '\r') {
            OnCommand(tmpBuff + cmdStart, cmdEnd - cmdStart - 1);
          } else {
            OnCommand(tmpBuff + cmdStart, cmdEnd - cmdStart);
          }
          
          TrimLeft(cmdEnd+1);
        }
    
      }
    
    } while (foundCommand);
  
  }
}