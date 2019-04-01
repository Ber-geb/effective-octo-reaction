/*
 * DA3B.c
 *
 * Created: 3/31/2019 6:25:59 PM
 * Author : Serak Gebremedhin
 */ 

#define F_CPU 16000000UL
#define BAUD_RATE 9600

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void usart_init();
void usart_send (unsigned char ch);

volatile uint8_t compACheck;


int main(void)
{
    usart_init();
	
	/**Setup and enable ADC **/
	ADMUX = (0<<REFS1)|					//Reference Selection Bits
	(1<<REFS0)|							//AVcc - external cap AREF
	(0<<ADLAR)|							//ADC Left Adjust Result
	(1<<MUX2)|							//Analog Channel Selection Bits
	(0<<MUX1)|							//ADC5 (PC5 PIN28)
	(1<<MUX0);
	ADCSRA = (1<<ADEN)|					//ADC Enable
	(0<<ADSC)|							//ADC Start Conversion
	(0<<ADATE)|							//ADC Auto Trigger Enable
	(0<<ADIF)|							//ADC Interrupt Flag
	(0<<ADIE)|							//ADC Interrupt Enable
	(1<<ADPS2)|							//ADC Prescaler Select Bits
	(0<<ADPS1)|
	(1<<ADPS0);
	
	TCCR1A = 0b01000000;				//Set to Normal Mode
	TCCR1B |= (1<<CS10) | (1<<CS12);	//Set prescaler to 1024
	compACheck = 0;						//Resets OVFCount
	OCR1A = 15624;
	TIMSK1 |= (1<<OCIE1A) |
	(1<<OCIE1B);						//Enable Timer1 Overflow Interrupt
	sei();								//Enable global interrupts
	
	while(1){
		ADCSRA |= (1<<ADSC);			//start conversion
		while ((ADCSRA&(1<<ADIF))==0);	//wait for conversion to finish
		
		ADCSRA |= (1<<ADIF);
		
		int a = ADCL;
		a = a | (ADCH << 8);
		usart_send((a/100)+'0');
		a = a % 100;
		usart_send((a/10)+'0');
		a = a % 10;
		usart_send((a)+'0');
		usart_send('\r');
		TCNT1 = 0;
		compACheck = 0;
		while (compACheck==0);	
		
	}
	return 0;
}

void usart_init(void){
	UCSR0B = (1<<TXEN0);
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
	UBRR0L = F_CPU/16/BAUD_RATE-1;
}

void usart_send(unsigned char ch){
	while(!(UCSR0A & (1<<UDRE0)));			//wait until UDR0 is empty
	UDR0 = ch;								//transmit ch
}

void usart_print(char* str){
	int i = 0;
	while (str[i] != 0)
		usart_send(str[i]);
}


ISR(TIMER1_COMPA_vect)
{
	TCNT1 = 0;
	compACheck = 1;
}

