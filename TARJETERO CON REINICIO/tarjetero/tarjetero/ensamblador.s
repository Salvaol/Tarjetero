
#include <avr/io.h>
#include <avr/interrupt.h>

.extern tiempo1
.extern tiempo2
.extern tiempo_N_S01

.extern bandera
.extern primero

.global flanco_subida

flanco_subida:

		
		//Guardamos los valores en el STACK PUSH....

		PUSH R16
		PUSH R17
		PUSH R18
		PUSH R19
		PUSH R20
		PUSH R21
		PUSH R22
		PUSH R23
	

		LDS R16, SREG
		PUSH R16

		LDS R16, tiempo1
		LDS R17, tiempo1+1
		LDS R18, tiempo2
		LDS R19, tiempo2+1
		LDS R20, tiempo_N_S01
		LDS R21, tiempo_N_S01+1
		LDS R22, bandera
		LDS R23, primero 

		LDS R18, ICR3
		LDS R19, ICR3+1
		STS tiempo2, R18
		STS tiempo2+1,R19
		
		LDI R22, 0
		LDI R23, 0
		SUB R18, R16
		SBC R19, R17
		MOV R20, R18
		MOV R21, R19

		STS tiempo_N_S01, R20
		STS tiempo_N_S01+1, R21
		STS bandera, R22
		STS primero, R23

		LDS R23, TCCR3B
		ANDI R23, 0b10111111
		STS TCCR3B, R23

//Recuperamos los valores del STACK PUSH....

		POP R16
		STS SREG, R16
		POP R23
		POP R22
		POP R21
		POP R20
		POP R19
		POP R18
		POP R17
		POP R16

		RET