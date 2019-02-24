;
; DA1B.asm
;
; Created: 2/23/2019 2:37:36 PM
; Author : Serak Gebremedhin
; Assignment 1b Design Assignment 1b (DA1B)
; This program will place certain values (starting at 11 and incrementing from there) at
; specific memory locations (i.e. initially values are places at 0x0200). Then, this program
; will find the numbers divisible by 3 and those that are not divisible by 3. Those that are
; divisible and not divisble by 3 will be placed in their own different memory locations, 
; respectively. Afterwards, the numbers that are not divisible by 3 will be summed and those
; that are divisible by 3 will be summed. 

.include <m328pdef.inc>

.SET	STARTADDS = 0x0200						;starting address

.ORG 0

	LDI R18, 1									;value determines jump to L1 or L2
start:	
	LDI R16, 0xB								;data value at address
	LDI R17, 99									;counter (total 99 numbers)
	LDI XL, 0x00								;low(STARTADDS)
	LDI XH, 0x02								;high(STARTADDS)
	CLR R19										;counter for number of divisions
	CPI R18, 2									;decides whether branch to L2
	BREQ L2										;if R18 == 2, then branch to L2
	INC R18										;increment R18
L1:	
	ST	X+, R16									;copy R16 to memory location X/Y/Z and increment the low byte of X/Y/Z
	DEC R17										;decrement the counter
	INC R16										;increment the data value
	MOV R28, R26								;copy X reg value to Y reg (for lower byte)
	MOV	R29, R27								;copy X reg value to Y reg (for upper byte)
	ST Y+, R16									;copy R16 to memory location X/Y/Z and increment the low byte of X/Y/Z
	DEC	R17										;decrement the counter
	INC R16										;increment the data value
	MOV	R30, R28								;copy Y reg value to Z reg (for lower byte)
	MOV	R31, R29								;copy Y reg value to Z reg (for upper byte)
	ST Z+, R16									;copy R16 to memory location X/Y/Z and increment the low byte of X/Y/Z
	MOV	R26, R30								;copy Z reg value to X reg (for lower byte)
	MOV R27, R31								;copy Z reg value to X reg (for upper byte)
	INC R16										;increment the data value
	DEC R17										;decrement the counter
	BRNE L1										;if counter != 0, then jmp to L1
	jmp start									;when done, jump to start
L2:
	CPI R19, 99									;ends div loop when final value already evaluated
	BREQ L4										;when final value reached branch to L4
	LD R16, X									;load data value from address X to R16
	CLR R17										;clear remainder
	INC R19										;counter for number of divisions
L3:
	CPI R18, 3									;initializes Y & Z address only once
	BREQ Div									;branch to Div if R18 == 3 (ensures Z & Y address doesn't re-initialize)
	LDI YL, 0x00								;Y is for number not divisible by 3 (lower byte)
	LDI YH, 0x06								;Y is for number not divisible by 3 (upper byte)
	LDI ZL, 0x00								;Z is for number divisible by 3 (lower byte)
	LDI ZH, 0x04								;Z is for number divisible by 3 (upper byte)
	INC R18										;increment R18 when Y & Z already initialized
Div:											;treats division as multiple subtractions
	SUBI R16, 3									;at the end R16 = remainder
	INC R17										;at the end R17 = result
	CPI R16, 3									;compare R16 to 3, if != 3 then remain in loop
	BRSH Div									;branch if R16 same or higher than 3 
	CPI	R16, 0									;compare R16 to 0
	BREQ DivBy3									;number is divisible by 3
	INC R1										;counter for non-divisible numbers
	LD R16, X+									;number is not divisible by 3
	ST Y+, R16									;store number not divisible by 3
	jmp L2										;jump to L2 to continue division for next number
DivBy3:
	INC R2										;counter for divisible numbers
	LD R16, X+									;load number from X reg (and post-increment X)
	ST Z+, R16									;store number divisible by 3
	jmp L2										;jump to L2 to continue division for next number

L4:
	LDI YL, 0x00								;Y is for number not divisible by 3 (lower byte)
	LDI YH, 0x06								;Y is for number not divisible by 3 (upper byte)
	LDI ZL, 0x00								;Z is for number divisible by 3 (lower byte)
	LDI ZH, 0x04								;Z is for number divisible by 3 (upper byte)
	CLR R16										;clear all registers that will be used to add
	CLR R17										;clear all registers that will be used to add
	CLR R18										;clear all registers that will be used to add
	CLR R19										;clear all registers that will be used to add
	CLR R20										;R20 will be used with ADC to add carry with 0

L5:
	LD R24, Y+									;load "non-divisible by 3" number to R24
	ADD R17, R24								;adding "non-divisible by 3" numbers
	ADC R16, R20								;R16:R17
	INC R3										;increment counter
	CP R3, R1									;compare counter to total number of "non-divisible by 3 numbers
	BRNE L5										;if R3 != R1, then branch to L5

L6:
	LD R25, Z+									;load "divisible by 3" number to R24
	ADD R19, R25								;adding "divisible by 3" numbers
	ADC R18, R20								;R18:R19
	INC R4										;increment counter
	CP	R4, R2									;compare counter to total number of "divisible by 3 numbers
	BRNE L6										;if R4 != R2, then branch to L5
	
	
end: jmp end									;infinite end loop