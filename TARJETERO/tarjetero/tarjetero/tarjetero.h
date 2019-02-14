/*
 * tarjetero.h
 *
 * Created: 5/10/2016 10:52:10 AM
 *  Author: Miguel
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef TARJETERO_H_
#define TARJETERO_H_

// PINx: Para poder leer determinados pines relacionados con el tarjetero

#define S01 (PINE & 0b10000000) //Sensor óptico n.1 Para INPUT CAPTURE
#define L1 (PINK & 0b01000000) //LED1 de Confirmación

//DDRx: Para la configuración del sentido de los puertos del tarjetero

#define S01DDR DDRE // Sensor óptico
#define L1DDR DDRK // LED1 de confirmación

//PORTx: Para poder modificar el valor de los puertos

#define S01PORT PORTE // Sensor óptico
#define L1PORT PORTK // LED 1


//MACROS para lectura y cambio de estado de los puertos

#define ledL1_ON() L1PORT |= 0b01000000           // encender el led L1
#define ledL1_OFF() L1PORT &= 0b10111111           // apagar el led L1
#define estado_ledL1() L1                          // chequear el estado del led L1


// Declaración de constantes

#define cuenta1s 122 //Desbordamientos para contar 1s con timer de 16 bits

//Variable tipo enum para definir el estado en el que nos encontramos en cada momento

extern char enable_L1;

extern unsigned int socio[24];

//typedef enum {FUNCIONANDO,EMERGENCIA,DISPONIBLE} estado;
	
// Funciones generales del tarjetero


	

#endif /* TARJETERO_H_ */