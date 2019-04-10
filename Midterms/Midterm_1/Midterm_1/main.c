/*
 * Midterm_1.c
 *
 * Created: 4/10/2019 10:32:34 AM
 * Author : Serak Gebremedhin
 */ 

#define F_CPU 16000000UL
#define BAUD_RATE 115200

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

void usart_init();
void usart_send (unsigned char ch);
void usart_putstring(char* ch);

volatile uint8_t compACheck;


int main(void)
{
    usart_init();
	
	/**Setup and enable ADC **/
	ADMUX = (0<<REFS1)|					//Reference Selection Bits
	(1<<REFS0)|							//AVcc - external cap AREF
	(0<<ADLAR);						//ADC Left Adjust Result
	ADCSRA = (1<<ADEN)|					//ADC Enable
	(0<<ADSC)|							//ADC Start Conversion
	(0<<ADATE)|							//ADC Auto Trigger Enable
	(0<<ADIF)|							//ADC Interrupt Flag
	(0<<ADIE)|							//ADC Interrupt Enable
	(1<<ADPS2)|							//ADC Prescaler Select Bits
	(0<<ADPS1)|
	(1<<ADPS0);
	
	char thingSpeakGET[100];
	char setMUX[] = "AT+CIPMUX=0\r\n"; //Repeating the steps ESPlorer used
	char thingSpeakStart[] = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n" ;
	char thingSpeakSend[] = "AT+CIPSEND=80\r\n"; //saying we'll send more data than we actually will 
	char thingSpeakClose[] = "AT+CIPCLOSE\r\n"; //ending the send communication
	
	while(1){
		ADCSRA |= (1<<ADSC);			//start conversion
		while ((ADCSRA&(1<<ADIF))==0);	//wait for conversion to finish
		
		ADCSRA |= (1<<ADIF);
		
		int a = ADCL;
		a = a | (ADCH << 8);
		a = (a/1024.0) * (5000/10);
		a = (a*9/5)+32;	//convert from celsius to fahrenheit
		
		a = a % 100;
		
		
		 
		usart_putstring(setMUX); //Sends string to USART using USART_send to send each character of string	
		_delay_ms(1000);
		
		usart_putstring(thingSpeakStart);
		_delay_ms(1000); //add in delay to allow for proper interaction
		
		usart_putstring(thingSpeakSend);
		_delay_ms(1000);
		
	    snprintf(thingSpeakGET, sizeof(thingSpeakGET), "GET https://api.thingspeak.com/update?api_key=27KKGYHOI2F2SN15&field1=%d\r\n", a); 
		usart_putstring(thingSpeakGET); 
		_delay_ms(1000);

		
		usart_putstring(thingSpeakClose); //ending of the send
		_delay_ms(1000);
		
	}
	return 0;
}

void usart_init(void){
	
	//UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);
	//UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UBRR0 = 8;	//Manually setting to value from atmega328p datasheet as the formula give a float value that has too much error.
	
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1<< UCSZ01)|(1<<UCSZ00);
}

void usart_send(unsigned char ch){
	while(!(UCSR0A & (1<<UDRE0)));			//wait until UDR0 is empty
	UDR0 = ch;								//transmit ch
}

void usart_putstring(char* ch){
	
	while(*ch != 0x00){    //Here we check if there is still more chars to send, this is done checking the actual char and see if it is different from the null char
		usart_send(*ch);    //Using the simple send function we send one char at a time
	ch++;}        //We increment the pointer so we can read the next char
	
}

void usart_print(char* str){
	int i = 0;
	while (str[i] != 0)
		usart_send(str[i]);
}





