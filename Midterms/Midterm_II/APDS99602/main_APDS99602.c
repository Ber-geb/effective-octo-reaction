/*
 * main_APDS99602.c
 *
 * Created: 5/12/2019 1:26:17 PM
 *  Author: Serak Gebremedhin
 */ 

#define F_CPU 16000000UL
#define BAUD_RATE 115200

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>										/* Include standard library file */
#include <stdio.h>										/* Include standard library file */
#include "APDS9960_def.h"							/* Include APDS99602 register define file */
#include "i2c_master.h"							/* Include I2C Master header file */
#include "uart.h"							/* Include USART header file */

#define APDS9960_WRITE 0x72 //i2c seven bit address shifted left one
#define APDS9960_READ 0x73 //seven bit address for read

void usart_init();
void usart_send (unsigned char ch);
void usart_putstring(char* ch);

uint16_t clear; //variable for clear

uint16_t red; //variable for red color

uint16_t green; //variable for green color

uint16_t blue; //variable for blue color

void init_uart(void){
	
	UBRR0 = 8;	//Manually setting to value from atmega328p datasheet as the formula give a float value that has too much error.
	
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1<< UCSZ01)|(1<<UCSZ00);					/* Load UBRRH with upper 8 bit of prescale value */
}

void uart_putc(unsigned char c){

	while(!(UCSR0A & (1<<UDRE0))); // wait until sending is possible
	UDR0 = c; // output character saved in c
}

void uart_puts(char *s){
	while(*s != 0x00){
		uart_putc(*s);
		s++;
	}
}


void init_APDS9960(void){
	_delay_ms(150);			

	
	i2c_start(APDS9960_WRITE);  //0x39 slave address
	i2c_write(APDS9960_ATIME); //enable reg address
	i2c_write(DEFAULT_ATIME);  //ALS enable
	i2c_stop();
		
	i2c_start(APDS9960_WRITE); 
	i2c_write(APDS9960_CONTROL); 
	i2c_write(DEFAULT_AGAIN);  
	i2c_stop();
	
	i2c_start(APDS9960_WRITE);  //set to write
	i2c_write(APDS9960_ENABLE); 
	i2c_write(0b00000011);  //Powers on, enables ALS
	i2c_stop();	
	
}

void getreading(void){
	i2c_start(APDS9960_WRITE);  //0x39 slave address
	i2c_write(APDS9960_CDATAL); //set pointer
	i2c_stop();
		
	i2c_start(APDS9960_READ);
	clear = (((uint16_t)i2c_read_ack()) | (uint16_t)i2c_read_ack()<<8);
	red = (((uint16_t)i2c_read_ack()) | (uint16_t)i2c_read_ack()<<8);
	green = (((uint16_t)i2c_read_ack()) | (uint16_t)i2c_read_ack()<<8);
	blue = (((uint16_t)i2c_read_ack()) | (uint16_t)i2c_read_ack()<<8);
	i2c_stop();
	
}

int main(void){


	char thingSpeakGET[150];
	char setMUX[] = "AT+CIPMUX=0\r\n"; //Repeating the steps ESPlorer used
	char thingSpeakStart[] = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n" ;
	char thingSpeakSend[] = "AT+CIPSEND=150\r\n"; //saying we'll send more data than we actually will
	char thingSpeakClose[] = "AT+CIPCLOSE\r\n"; //ending the send communication
	
	init_uart();
	i2c_init();
	init_APDS9960();
	while(1){
		getreading();
		uart_puts(setMUX); //Sends string to USART using USART_send to send each character of string
		_delay_ms(1000);
		
		uart_puts(thingSpeakStart);
		_delay_ms(1000); //add in delay to allow for proper interaction
		
		uart_puts(thingSpeakSend);
		_delay_ms(1000);
	
		snprintf(thingSpeakGET, sizeof(thingSpeakGET), "GET https://api.thingspeak.com/update?api_key=9XCZN88B0QROK7MS&field1=%d&field2=%d&field3=%d&field4=%d\r\n", red, green, blue, clear);
		uart_puts(thingSpeakGET);
		_delay_ms(50);
				

		uart_puts(thingSpeakClose); //ending of the send
		_delay_ms(1000);	
		
	}
}