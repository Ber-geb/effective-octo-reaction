/*
 * DA2AT2.c
 *
 * Created: 3/2/2019 7:51:38 PM
 * Author : Serak Gebremedhin
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	//T2 for C
	DDRC &= (0<<2);						//Set PINC.2 as input
	DDRB |=  (1<<2);					//Set PINB.2 as output
	PORTB |= (1<<2);					//Turn on pull-up
	while (1){
		if (!(PINC & (1<<PINC2))){		//check if PINC is set
			PORTB |= (1<<2);			//Turn on LED
			_delay_ms(1250);			//Delay for 1250 ms
		}
		else {
			PORTB &= (0<<2);			//otherwise keep LED off
		}
	}
}
