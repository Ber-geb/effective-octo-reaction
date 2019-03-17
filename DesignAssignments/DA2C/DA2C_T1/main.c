/*
 * DA2C_CT1_AT1.c
 *
 * Created: 3/16/2019 3:04:17 PM
 * Author : Serak Gebremedhin
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>


int main(void)
{
    DDRB |= (1<<2);						//PORTB.2 is set as output
	TCCR0A = 0x40;						//setting timer to normal mode
	TCCR0B = 0x03;						//setting prescaler to 64
	TCNT0 = 0;							//reset timer counter
	while (1){
		PORTB |= (1<<2);				//turn on led
		for (int i = 1; i <= 426; i++){
			while ((TIFR0 & (1<<TOV0)) == 0);	//wait until overflow flag is set		
			if (i >= 426){
				/*
				compare increment with 426 (b/c the highest multiple for 255 before it reaches 108749 is 426)
				255*426 = 108630 < 108749
				*/
				OCR0A = 119;					//119 is selected here because 108749-108630 = 119
				TCNT0 = 0;						//reset timer
				TIFR0 = (1<<1);					//clear flag
				while ((TIFR0 & (1<<OCF1A)) == 0); //wait until compare match flag is set	
				TIFR0 |= (1<<1) | (1<<0);		//clear flags
				TCNT0 = 0;						//reset timer
			}
			TIFR0 = (1<<0);						//reset flag
		}
		PORTB &= (0<<2);				//turn off led
		for (int i = 1; i <= 284; i++){
			while ((TIFR0 & (1<<TOV0)) == 0);	//wait until overflow flag is set		
			if (i >= 284){
				/*
				compare increment with 284 (b/c the highest multiple for 255 before it reaches 72499 is 284)
				255*284 = 72420 < 72499
				*/
				OCR0A = 79;						//39 is selected here because 72499-72420 = 79
				TCNT0 = 0;						//reset timer
				TIFR0 = (1<<1);					//clear flag
				while ((TIFR0 & (1<<OCF1A)) == 0); //wait until compare match flag is set	
				TIFR0 |= (1<<1) | (1<<0);		//clear flags
				TCNT0 = 0;						//reset timer
			}
			TIFR0 = (1<<0);						//reset flag
		}
	}
}

