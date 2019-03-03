;
; DA2AT2_Assem.asm
;
; Created: 3/2/2019 9:55:09 PM
; Author : Serak Gebremedhin
;

.include <m328pdef.inc>

.org 0
	LDI R19, 30							;decrement
	CBI DDRC, 2							;switch connected to PORTC.2
	SBI DDRB, 2							;set led output PORTB.2
	
again:
	SBIS PINC, 2						;check if PINC.2 is set
	RJMP over							;keep checking if LOW
	CBI PORTB, 2						;clear PORTB.2
	RJMP again							;relative jump to again
over:
	SBI PORTB, 2						;PORTB.2 = 1
	CALL delaySwitch					;call delay subroutine
	RJMP again							;relative jump to again

delaySwitch:
	LDI XH, 0xFF						;upper byte set
delaySw0:
	LDI XL, 0x65						;lower byte set
delaySw1:
	DEC XL								;decrement lower byte
	BRNE delaySw1						;if != zero then branch to delaySw1
delaySw2:
	DEC XH								;decrement upper byte
	BRNE delaySw0						;if != zero then branch to delaySw0
	DEC R19								;decrement R19
	BRNE delaySwitch					;if != zero then branch to delaySwitch
	RET									;return to instruction after call
