/*
 * DA2C_CT1_AT2.c
 *
 * Created: 3/16/2019 6:07:08 PM
 * Author : Serak Gebremedhin
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>


int main(void)
{
    DDRC &= (0<<2);						//set PINC.2 as input
	DDRB |= (1<<2);						//set PORTB.2 as output
	PORTC |= (1<<2);					//Turn on pull-up
	TCCR0A |= (1<<COM0A1) | (1<<COM0A0); //set up timer control register
	TCCR0B |= (1<<CS02);				//set up prescaler = 256
	while(1){
		 if (!(PINC & (1<<PINC2))){		//check if PINC is set
			 PORTB |= (1<<2);			//turn on LED
			 for (int i = 1; i <= 18; i++){
				  TIFR0 |= (1<<TOV0);	//reset flag
				  TCNT0 = 0;			//reset timer counter
				 for (int j = 1; j<=17; j++){
					while ((TIFR0 & (1<<TOV0)) == 0);	//wait until overflow flag is set
					TIFR0 |= (1<<TOV0);	//reset flag
				 }
			 }	
			PORTB &= (0<<2);			//turn off LED	 
		 }
	}
}


