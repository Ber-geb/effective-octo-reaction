/*
 * Task1.c
 *
 * Created: 4/26/2019 1:26:05 PM
 * Author : Serak Gebremedhin
 */ 

// 	MIT License
//
// 	Copyright (c) 2018 Helvijs Adams
//
// 	Permission is hereby granted, free of charge, to any person obtaining a copy
// 	of this software and associated documentation files (the "Software"), to deal
// 	in the Software without restriction, including without limitation the rights
// 	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// 	copies of the Software, and to permit persons to whom the Software is
// 	furnished to do so, subject to the following conditions:
//
// 	The above copyright notice and this permission notice shall be included in all
// 	copies or substantial portions of the Software.
//
// 	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// 	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// 	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// 	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// 	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// 	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// 	SOFTWARE.

//
//	Software was tested on ATmega328P and ATmega328PB (PB needs few changes in SPI)
//	RF module software was tested on - cheap nRF24L01+ from China
//	All the relevant settings are defined in nrf24l01.c file
//	Some features will be added later, at this moment it is bare minimum to send/receive
//

//	Set clock frequency
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


//	Set up UART for printf();
#ifndef BAUD
#define BAUD 9600
#endif
#include "STDIO_UART.h"

//	Include nRF24L01+ library
#include "nrf24l01.h"
#include "nrf24l01-mnemonics.h"
#include "spi.h"


//	Used in IRQ ISR
volatile bool message_received = false;
volatile bool status = false;

int main(void)
{
	//	Set cliche message to send (message cannot exceed 32 characters)
	char tx_message[32];				// Define string array
	strcpy(tx_message,"Hello World!");	// Copy string into array
	
	//	Initialize UART
	uart_init();
	//Initialize ADC conversion	
	ADMUX |= (1<<REFS0); //AVcc w/ external capacitor at AREF pin
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0) |
	(1<<ADATE); //enable ADC, Auto trigger enabled, ADC prescaler = 32
	
	
	
	//Start conversion
	void adc_conv(void){
		ADCSRA |= (1<<ADSC); //start conversion
		while ((ADCSRA & (1<<ADIF))==0); //wait for conversion to finish
		ADCSRA |= (1<<ADIF); //reset flag
	}


	//	Initialize nRF24L01+ and print configuration info
	nrf24_init();
	
	
	//	Start listening to incoming messages
	nrf24_start_listening();
	
	while (1)
	{
		adc_conv(); //Start ADC conversion
		unsigned int a = ADCL; //set int a equal to lower byte of ADC
		a = a | (ADCH << 8); //Set value to temperature value
		a = (a/1024.0) * (5000/10);
		a = (a*9/5) + 32; //convert celsius to fahrenheit
		snprintf(tx_message, sizeof(tx_message), "%3d\r\n", a);
		nrf24_send_message(tx_message); //send the message to nrf24
		_delay_ms(1500); 
	
		if (message_received)
		{
			//	Message received, print it
			message_received = false;
			printf("Received message: %s\n",nrf24_read_message());
			//	Send message as response
			_delay_ms(500);
			
			
			status = nrf24_send_message(tx_message);
			if (status == true) printf("Temperature successfully\n");
		}
	}
}

//	Interrupt on IRQ pin
ISR(INT0_vect)
{
	message_received = true;
}

