/*
 * DA2C_CT2_AT1.c
 *
 * Created: 3/16/2019 7:14:24 PM
 * Author : Serak Gebremedhin
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

static volatile int count0 = 0;
static volatile int count1 = 0;

int main(void)
{

	DDRB |= (1<<DDB2);		//PB2 as output
	TIMSK0 |= (1<<TOIE0);	//enable timer overflow interrupt
	TCNT0 = 0x0;
	sei();					//enable interrupts
	TCCR0A |= (1<<COM0A1) | (1<<COM0A0);
	TCCR0B |= (1<<CS02);
	while (1){	
	}
   
}


ISR (TIMER0_OVF_vect){						//timer0 overflow interrupt

	if (count0 == 106){						//two counters used
		PORTB &= (0<<2);					//turn off led
		if (count1 == 71){
			PORTB |= (1<<2);				//turn on led
			count0 = 0;						//reset counter
			count1 = 0;						//reset counter
		}
		else
			count1++;						//increment counter
	}
	else count0++;							//increment counter				
	
}

