/*
 * DA2C_CT3_AT2.c
 *
 * Created: 3/16/2019 10:18:52 PM
 * Author : Serak Gebremedhin
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

static volatile int count0 = 0;

int main(void)
{
   DDRB |= (1<<2);					//set PB2 at output
   OCR0A = 0xFF;
   TCCR0A |= (1<<WGM01);			//set to CTC mode
   TIMSK0 |= (1<<OCIE0A);			//set interrupt on compare match
   TCCR0B |= (1<<CS02) | (1<<CS00);	//set prescaler to 1024
   sei();							//enable global interrupts
   while(1){
	  if (!(PINC & (1<<PINC2))){		//check if PINC is set
		  PORTB |= (1<<2);
		  TCNT0 = 0;
	  }
   }
}

ISR (TIMER0_COMPA_vect){
	if (count0 == 76){
		PORTB &= (0<<2);
		count0 = 0;
	}
	count0++;;
	
}
