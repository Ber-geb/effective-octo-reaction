;
; DA2A.asm
;
; Created: 3/2/2019 9:47:40 AM
; Author : Serak Gebremedhin
;


.include <m328pdef.inc>

.org 0
begin:
	SBI DDRB, 2							;set pin 2 as output (low impedance for pin 2)
	SBI PORTB, 2						;PORTB.2 = 1
	LDI R19,37							;decrement
	CALL delayOn						;call one of two delay subroutines
	RJMP next							;relative jump to next
		
beginDelay:								;beginning of first delay subroutine
	LDI XH, 0xFF						;Value chosen for double nested loop
delayOn:
	LDI XL, 0xF5						;Value chosen for double nested loop
delay0:
	DEC XL								;Decrement lower byte
	BRNE delay0							;if != zero then continue decrement
delay1:
	DEC XH								;Decrement upper byte
	BRNE delayOn						;if != zero then continue decrement
	DEC R19								;decrement R19
	BRNE beginDelay						;if != zero then branch to beginDelay
	RET									;return to instruction after call

next:									;next delay subroutine for LOW signal
	CBI PORTB, 2						;clear PORTB.2 
	LDI R19, 28							;load decrement
	CALL beginDelayOff					;call second delay subroutine
	RJMP begin							;relative jump to beginning of code

beginDelayOff:
	LDI XH, 0xFF						;Value chosen for double nested loop
delayOff:
	LDI XL, 0xD8						;Value chosen for double nested loop
delay0Off:
	DEC XL								;Decrement lower byte
	BRNE delay0Off						;if != zero then continue decrement
delay1Off:
	DEC XH								;Decrement upper byte
	BRNE delayOff						;if != zero then continue decrement
	DEC R19								;decrement R19
	BRNE beginDelayOff					;if != zero then branch to beginning of delay subroutine
	RET									;return to instruction after call 

