/*
 * DA2C_CT3_AT1.c
 *
 * Created: 3/16/2019 10:18:52 PM
 * Author : Serak Gebremedhin
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

static volatile int count0 = 0;
static volatile int count1 = 0;

int main(void)
{
   DDRB |= (1<<2);					//set PB2 at output
   OCR0A = 0xFF;
   TCCR0A |= (1<<WGM01);			//set to CTC mode
   TIMSK0 |= (1<<OCIE0A);			//set interrupt on compare match
   TCCR0B |= (1<<CS02) | (1<<CS00);	//set prescaler to 1024
   sei();							//enable global interrupts
   while(1){
	   //main loop
   }
}

ISR (TIMER0_COMPA_vect){
	if (count0 == 27){
		PORTB &= (0<<2);			//turn off led
		if (count1 == 18){
			PORTB |= (1<<2);		//turn on led
			count1 = 0;
			count0 = 0;
		}
		else
			count1++;
	}
	else
		count0++;
	
}
