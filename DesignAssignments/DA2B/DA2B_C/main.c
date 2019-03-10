/*
 * DA2B_C.c
 *
 * Created: 3/9/2019 7:07:16 PM
 * Author : Serak Gebremedhin
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	DDRB = 1<<2;			//PB2 as an output
	PORTD = 1<<2;			//pull-up activated
	EICRA = 0x2;			//make INT0 falling edge triggered
	
	EIMSK = (1<<INT0);		//enable external interrupt 0
	sei();					//enable interrupts
	
	while (1);				//wait here	  
}

ISR (INT0_vect){			//ISR for external interrupt 0
	PORTB |= (1<<2);
	_delay_ms(1250);
	PORTB ^= (1<<2);
}

	

