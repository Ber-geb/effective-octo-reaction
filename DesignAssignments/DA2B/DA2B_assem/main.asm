;
; DA2B_assem.asm
;
; Created: 3/9/2019 6:03:34 PM
; Author : Serak Gebremedhin
;

.include <m328pdef.inc>
.org 0x0						;location for reset	
JMP MAIN
.org 0x02						;location for external interrupt 0
JMP EX0_ISR
MAIN:
	LDI R20, HIGH(RAMEND)
	OUT SPH, R20
	LDI R20, LOW(RAMEND)
	OUT SPL,R20					;initialize stack
	CBI PORTB, 2				; clear portb.2
	LDI R20, 0x2				;make INT0 falling edge triggered
	STS EICRA, R20				
	SBI	DDRB, 2					;PB.2 = output
	CBI DDRC, 2					;PC.2 = input
	SBI PORTD, 2				;pull-up activated
	LDI R20, 1<<INT0			;enable INT0
	OUT EIMSK, R20
	SEI							;enable interrupts
HERE:	JMP HERE


EX0_ISR:
	IN R21, PORTB
	LDI R22, (1<<2)				;toggling PB2
	EOR R21, R22
	OUT PORTB, R21
	;Code below is from DA2A.2 except the RETI instruction
delaySwitch:
	LDI XH, 0xFF				;upper byte set
delaySw0:
	LDI XL, 0x65				;lower byte set
delaySw1:
	DEC XL						;decrement lower byte
	BRNE delaySw1				;if != zero then branch to delaySw1
delaySw2:
	DEC XH						;decrement upper byte
	BRNE delaySw0				;if != zero then branch to delaySw0
	DEC R19						;decrement R19
	BRNE delaySwitch			;if != zero then branch to delaySwitch
	EOR R21, R22
	OUT PORTB, R21
	RETI						;exit ISR and return to main
