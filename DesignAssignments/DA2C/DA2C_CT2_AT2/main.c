/*
 * DA2C_CT2_AT2.c
 *
 * Created: 3/16/2019 7:14:24 PM
 * Author : Serak Gebremedhin
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

static volatile int count0 = 0;

int main(void)
{

	DDRB |= (1<<DDB2);		//PB2 as output
	TIMSK0 |= (1<<TOIE0);	//enable timer overflow interrupt
	TCNT0 = 0x0;
	sei();					//enable interrupts
	TCCR0A |= (1<<COM0A1) | (1<<COM0A0);
	TCCR0B |= (1<<CS02) | (1<<CS00);
	while (1){	
		
		if (!(PINC & (1<<PINC2))){		//check if PINC is set
			PORTB |= (1<<2);
			TCNT0 = 0;
			count0 = 0;
		}
	}
   
}


ISR (TIMER0_OVF_vect){						//timer0 overflow interrupt
	
	if (count0 == 76){
			PORTB &= (0<<2);
			count0 = 0;
			TCNT0 = 0;
	}	
	else	
		count0++;
}


