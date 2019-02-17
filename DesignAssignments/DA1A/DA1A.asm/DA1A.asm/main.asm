;
; DA1A.asm.asm
;
; Created: 2/16/2019 4:07:24 PM
; Author : Serak Gebremedhin
; Design Assignment 1A


.include <m328pdef.inc>				;include directive for atmega328p


.ORG 0								;directive to state where instruction address begins
		LDI R25, 0x88				;holds upper byte of 16-bit multiplicand
		LDI R24, 0x88				;holds lower byte of 16-bit multiplicand
		LDI R22, 0x88				;holds value of 8-bit multiplier
		LDI R21, 0x0				;decrement for addition loop
		LDI R26, 0x0				;holds zero
		MOV R21, R22				;decrement set equal to 8-bit multiplier
		LDI R20, 0x0				;will be upper byte for solution
		LDI	R19, 0x0				;will be the middle byte for solution
		LDI R18, 0x0				;will be lower byte for solution

AddLoop:

		ADD R18, R24				;add lower byte of multiplicand and multiplier
		ADC R19, R25				;add with carry from above addition
		ADC R20, R26				;add with carry from above addition so R20:R19:R18
		DEC R21						;decrements R21 for iterative addition
		BRNE AddLoop				;branch back to AddLoop if R21 != 0



end:	jmp end						;infinite loop at end




		

