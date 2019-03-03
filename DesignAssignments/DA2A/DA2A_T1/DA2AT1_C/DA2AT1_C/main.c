/*
 * DA2AT1_C.c
 *
 * Created: 3/2/2019 9:45:48 PM
 * Author : Serak Gebremedhin
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{ 
	//T1 for C
    DDRB |= (1<<2);				//set PORTB.2 for output
	while(1){
		PORTB |= (1<<2);		//set PORTB.2 = 1
		_delay_ms(435);			//delay for 435 ms
		PORTB &= (0<<2);		//set PORTB.2 = 0
		_delay_ms(290);			//delay for 290 ms
	
	}
	return 0;
}

