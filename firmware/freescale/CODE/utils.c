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

#include "utils.h"

/* A utility function to reverse a string  */
void reverse(char str[], int length)
{
    char a;

    int start = 0;
    int end = length -1;
    while (start < end)
    {
        a = *(str+start);
        *(str+start) = *(str+end);
        *(str+end) = a;
        
        start++;
        end--;
    }
}

// Implementation of itoa()
char* itoa(int num, char* str, int base, uint8_t width)
{
    int i = 0;
    int j = 0;
    bool isNegative = false;
    char padChar = '0';
    
    if (base == 10) {
      padChar = ' ';
    }
 
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        for (j=0;j<width;j++) {
          str[j] = '0';
        }
        str[width] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled only with 
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'A' : rem + '0';
        num = num/base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
    
    // Reverse the string
    reverse(str, i);
    
    // Eventually pad with leading zeroes
    if (width > i) {
      for (j=0;j<i;j++) {
        str[width-1-j] = str[i-1-j];
      }
      
      for (j=0;j<width-i;j++) {
        str[j] = padChar;
      }
    }
 
    str[width] = '\0'; // Append string terminator    
 
    return str;
}