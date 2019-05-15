
/*
 * uart.h
 *
 * Created: 5/12/2019 12:51:03 PM
 *  Author: Serak Gebremedhin
 */ 

/*
 * USART_RS232_H_file.h
 * http://www.electronicwings.com
 *
 */ 


#ifndef USART_RS232_H_FILE_H_				/* Define library H file if not defined */
#define USART_RS232_H_FILE_H_

#define F_CPU 16000000UL						/* Define CPU clock Frequency e.g. here its 8MHz */
#include <avr/io.h>							/* Include AVR std. library file */
#define BAUD_PRESCALE (((F_CPU / (BAUDRATE * 16UL))) - 1)	/* Define prescale value */

void usart_init(void);				/* USART initialize function */
void usart_send(unsigned char);				/* Send string of USART data function */
void usart_putstring(char*);
void usart_print(char*);

#endif		