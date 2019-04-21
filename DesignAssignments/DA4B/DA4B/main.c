/*
 * DA4B.c
 *
 * Created: 4/20/2019 9:12:50 PM
 * Author : Serak Gebremedhin
 * THIS IS FOR TASK 1
 */ 

#define F_CPU 16000000UL/* Define CPU Frequency 16MHz */
#include <avr/io.h>/* Include AVR std. library file */
#include <util/delay.h>/* Include delay header file */
#include <avr/interrupt.h>

uint8_t count = 0;

int main(void)
{
	
	TCCR1B |= (1<<WGM12) | (1<<CS11) | (1<<CS10); //CTC mode and prescaler = 64
	TIMSK1 |= (1<<OCIE1A);
	sei();
	
	ADCSRA= 0x83;//make ADC enable and select ck/128
	ADMUX= 0xC0;//1.1V Vref, ADC0, right-justified
	
	//int period;
	DDRD = 0x0F;/* Make PORTD lower pins as output */
	//period = 100;/* Set period in between two steps */
	while (1)
	{
		
		ADCSRA|=(1<<ADSC);//start conversion
		while((ADCSRA&(1<<ADIF))==0);//wait for conversion to finish
		ADCSRA |= (1<<ADIF); //reset flag
		
		if (ADC == 0)
			OCR1A = 12499; //50 ms
		else if (ADC > 0 && ADC <=500)
			OCR1A = 24999; //100 ms
		else if  (ADC > 500 && ADC <= 1000)
			OCR1A = 37499; //150 ms
		else if (ADC > 1000)
			OCR1A = 49999; //200 ms
	
		/* Rotate Stepper Motor Anticlockwise with Full step sequence */
	/*
		PORTD = 0x09;
		_delay_ms(period);
		PORTD = 0x03;
		_delay_ms(period);
		PORTD = 0x06;
		_delay_ms(period);
		PORTD = 0x0C;
		_delay_ms(period);
	*/
	}
	
}

ISR (TIMER1_COMPA_vect){
	
	/* Rotate Stepper Motor Anticlockwise with Full step sequence */
	if (count == 0){
		PORTD = 0x09;
		count++;
	}
	else if (count == 1){
		PORTD = 0x03;
		count++;
	}
	else if (count == 2){
		PORTD = 0x06;
		count++;
	}
	else if (count >= 3){
		PORTD = 0x0C;
		count=0;
	}
	
}

