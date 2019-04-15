/*
 * Task_1.c
 *
 * Created: 4/14/2019 7:57:35 AM
 * Author : Serak Gebremedhin
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define ENABLE 6
#define MTR_1 1
#define MTR_2 2
#define SW (PINC&(1<<1))

uint8_t buttonpress = 0;

int main (void )
{
	/*
	DDRD = 0xFF; //enable PORTD as output
	DDRB |= 0b00000111; //PB0, PB1, and PB2 as outputs
	DDRC = 0x00; //ENABLE PORTC as input
	OCR1A = 50;
	//Fast PWM, non-inverted
	TCCR1A |= (1<<COM1A1)|(1<<WGM11)|(1<<WGM10);
	TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS11); //N = 8
	*/
	DDRD = 0xFF; //port D as output
	PORTD = 0b00000010; //AIN2 = 1 and AIN1 = 0
	DDRB |= (1 << DDB1)|(1 << DDB2);
	// PB1 and PB2 is now an output
	ICR1 = 0xFFFF;
	// set TOP to 16bit
	TCCR1A |= (1 << COM1A1);
	// set none-inverting mode
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12)|(1 << WGM13);
	// set Fast PWM mode using ICR1 as TOP
	TCCR1B |= (1 << CS10);
	// START the timer with no prescaler
	
	sei();
	PCICR |= (1<<PCIE1); // enables PCINT 14..8
	PCIFR |= (1<<PCIF1);
	PCMSK1 |= (1<<PCINT9); //enables pc interrupt 9 (PC1)
	ADCSRA= 0x83;//make ADC enable and select ck/128
	ADMUX= 0xC0;//1.1V Vref, ADC0, right-justified

	while (1)
	{	
		
		ADCSRA|=(1<<ADSC);//start conversion
		while((ADCSRA&(1<<ADIF))==0);//wait for conversion to finish
		ADCSRA |= (1<<ADIF);

		_delay_ms(100);
		
		if (ADC == 0)	 //when pot is at 0 (MIN) then PWM at 0
			OCR1A = 0;	
		else if (ADC >= 0 && ADC < 100)
			OCR1A = 5659;
		else if (ADC >= 100 && ADC < 200)
			OCR1A = 11318;
		else if (ADC >= 200 && ADC < 300)
			OCR1A = 16977;
		else if (ADC >= 300 && ADC < 400)
			OCR1A = 22636;
		else if (ADC >= 400 && ADC < 500)
			OCR1A = 28295;
		else if (ADC >= 500 && ADC < 600)
			OCR1A = 33954;
		else if (ADC >= 600 && ADC < 700)
			OCR1A = 39613;
		else if (ADC >= 700 && ADC < 800)
			OCR1A = 45272;
		else if (ADC >= 800 && ADC < 900)
			OCR1A = 50931;
		else if (ADC >= 900 && ADC < 1000)
			OCR1A = 56590;
		else if (ADC <= 1023) //when pot is at 1023 (MAX) then PWM at 95%
			OCR1A = 62258;		
	}
}

ISR(PCINT1_vect){
		if (buttonpress==1 && SW != 0){
			_delay_ms(300);
			PORTD = 0b00000010; //AIN2 = 1 and AIN1 = 0
			buttonpress = 0;
		}
		
		else if (buttonpress==0 && SW != 0){ //if button pressed
			_delay_ms(300);
			PORTD = 0; //AIN1 and AIN2 = 0
			buttonpress = 1;
		}			
}
