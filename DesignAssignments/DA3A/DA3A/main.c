/*
 * DA3A.c
 *
 * Created: 3/27/2019 5:26:56 PM
 * Author : Serak Gebremedhin
 */ 

#define F_CPU 16000000UL
#define UBRR_9600 103						//for 16Mhz with .2% error

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <time.h>

void read_adc(void);						//function declarations
void adc_init(void);
void USART_init(unsigned int ubrr);
void USART_tx_string(char *data);

static volatile int count = 0;				//determines when while loop should end
volatile unsigned int adc_temp;
char outs[20];

int main(void){
	float x;
	TIMSK1 |= (1<<OCIE1A);					//enable timer compare interrupt
	sei();									//enable interrupts
	TCCR1A |= (1<<COM1A1);
	TCCR1B |= (1<<CS12) | (1<<CS10);		//set prescaler 1024
	OCR1A = 15624;							//delay for 1 sec
	adc_init();								//initialize the ADC(Analog/Digital Converter)
	USART_init(UBRR_9600);					//initialize the USART (RS232 interface)
	USART_tx_string("Connected!\r\n");		// we're alive!
	TCNT1 = 0x0;							//set timer counter to 0
	while(1){
		while (count==0);					//when count = 1 end loop
		count = 0;							//reset count to zero
		read_adc();
		USART_tx_string("Hello World!\r\n");
		x = (float)rand()/(float)(1000.0);
		snprintf(outs,sizeof(outs),"%lf\r\n",x);
		USART_tx_string(outs);
		snprintf(outs,sizeof(outs),"%d\r\n",rand());	//print it
		USART_tx_string(outs);
	

	}
} 

void adc_init(void){
	/* setup and enable ADC */
	ADMUX = (0<<REFS1)|						//Reference Selection Bits
	(1<<REFS0)|								//AVcc - external cap AREF
	(0<<ADLAR)|								//ADC Left Adjust Result
	(0<<MUX2)|								//Analog Channel Selection Bits
	(1<<MUX1)|								//ADC2 (PC2 PIN25)
	(1<<MUX0);
	ADCSRA = (1<<ADEN)|						//ADC ENable
	(0<<ADSC)|								//ADC Start Conversion
	(0<<ADATE)|								//ADC Auto Trigger Enable
	(0<<ADIF)|								//ADC Interrupt Flag
	(0<<ADIE)|								//ADC Interrupt Enable
	(1<<ADPS2)|								//ADC Prescaler Select Bits
	(0<<ADPS1)|
	(1<<ADPS0);								//Select Channel
	
}

/* READ ADC PINS */

void read_adc(void){
	unsigned char i = 4;
	adc_temp = 0;
	while(i--){
		ADCSRA |= (1<<ADSC);
		while (ADCSRA & (1<<ADSC));
		adc_temp+= ADC;
	}
	adc_temp = adc_temp/4;					//Average a few samples
}

/* INIT USART (RS-232) */
void USART_init(unsigned int ubrr){
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<TXEN0);					//Enable receiver, transmitter & RX interrupt
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);		//asynchronous 8 N 1
}

/* SEND A STRING TO THE RS-232 */
void USART_tx_string(char *data){
	while ((*data != '\0')){
		while (!(UCSR0A & (1<<UDRE0)));
		UDR0 = *data;
		data++;
	}
}


ISR (TIMER1_COMPA_vect){						//timer0 overflow interrupt

	TIFR1 |= (1<<OCF1A);						//clear compare match flag 
	count = 1;									//set count equal to 1 to end loop
}


