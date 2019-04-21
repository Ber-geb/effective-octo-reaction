/*
 * Task2.c
 *
 * Created: 4/20/2019 10:49:38 PM
 * Author : Serak Gebremedhin
 * THIS IS FOR TASK 2
 */ 

#define F_CPU 16000000UL/* Define CPU Frequency 16MHz */
#include <avr/io.h>
#include <util/delay.h>/* Include delay header file */
#include <avr/interrupt.h>

int main(void)
{
	//Configure TIMER1
	TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11); //NON Inverted PWM
	//PRESCALER=64 MODE 14(FAST PWM)
	TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10);
	ICR1=4999; //fPWM=50Hz (Period = 20ms Standard).
	DDRB|=(1<<DDB1); //PWM Pins as Out
	
	ADCSRA= 0x83;//make ADC enable and select ck/128
	ADMUX= 0xC0;//1.1V Vref, ADC0, right-justified
	
	while(1)
	{
		ADCSRA|=(1<<ADSC);//start conversion
		while((ADCSRA&(1<<ADIF))==0);//wait for conversion to finish
		ADCSRA |= (1<<ADIF); //reset flag
		
		if (ADC >= 0 && ADC <= 100){
			OCR1A=97; //0 degree
			_delay_ms(1000);
		}
		//Wait();
		else if (ADC > 100 && ADC <= 341){
			OCR1A=316; //90 degree
			_delay_ms(1000);
		}
		//Wait();
		else if (ADC > 341 && ADC <= 682){
			OCR1A=425; //135 degree
			_delay_ms(1000);
		}
		//Wait();
		else if (ADC > 682 && ADC <= 1023){
			OCR1A=535; //180 degree
			_delay_ms(1000);
		}
		//Wait();
	}
}

