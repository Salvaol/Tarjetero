/*
 * balancin.h
 *
 * Created: 06/05/2016 15:18:46
 *  Author: Tmicro 8
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef BALANCIN_H_
#define BALANCIN_H_

// PINx: Para poder leer determinados pines relacionados con el balancín

#define M2EN (PINL & 0b00001000) // Enable del motor 2
#define M2DI (PINL & 0b00000100) // Direccion del motor 2
#define M2BK (PINL & 0b00000000) // No se puede leer en principio
#define L3   (PINL & 0b01000000) // Led de espera de cola
#define L4   (PINL & 0b10000000) // Led de emergencia

#define SO4  (PINK & 0b00000100) // Sensor óptico del encoder
#define SO5  (PINK & 0b00001000) // Sensor óptico del balancín, detecta abajo
#define SW1  (PINK & 0b00010000) // Switch de emergencia, activo por nivel bajo
#define SW3  (PINK & 0b00100000) // Switch de cola completa, activo por nivel bajo

//DDRx: Para la configuración del sentido de los puertos del balancín

#define M2ENDDR DDRL // Enable del motor 2
#define M2DIDDR DDRL // Direccion del motor 2
#define M2BKDDR DDRL // No se puede leer en principio
#define L3DDR   DDRL // Led de espera de cola
#define L4DDR   DDRL // Led de emergencia

#define SO4DDR  DDRK // Sensor óptico del encoder
#define SO5DDR  DDRK // Sensor óptico del balancín, detecta abajo
#define SW1DDR  DDRK // Switch de emergencia, activo por nivel bajo
#define SW3DDR  DDRK // Switch de cola completa, activo por nivel bajo

//PORTx: Para poder modificar el valor de los puertos

#define M2ENPORT PORTL // Enable del motor 2
#define M2DIPORT PORTL // Direccion del motor 2
#define M2BKPORT PORTL // No se puede leer en principio
#define L3PORT   PORTL // Led de espera de cola
#define L4PORT   PORTL // Led de emergencia

#define SO4PORT  PORTK // Sensor óptico del encoder
#define SO5PORT  PORTK // Sensor óptico del balancín, detecta abajo
#define SW1PORT  PORTK // Switch de emergencia, activo por nivel bajo
#define SW3PORT  PORTK // Switch de cola completa, activo por nivel bajo

//MACROS para lectura y cambio de estado de los puertos

#define motor_balancin_ON() M2ENPORT |= 0b00001000 // encender el motor (sin PWM ojo)
#define motor_balancin_OFF() M2ENPORT &= 0b11110111 // apagar el motor
#define estado_motor_balancin() M2EN			   // Consultar el PIN del enable del motor
#define ledL3_ON()  L3PORT |= 0b01000000           // encender el led L3
#define ledL3_OFF() L3PORT &= 0b10111111           // apagar el led L3
#define estado_ledL3() L3						   // chequear el estado del led L3			
#define ledL4_ON() L4PORT |= 0b10000000           // encender el led L4
#define ledL4_OFF() L4PORT &= 0b01111111           // apagar el led L4
#define estado_ledL4() L4                          // chequear el estado del led L4

// Distintas constantes

#define cuenta10s 1221 // Desbordamientos para 10 s con timer de 16 bits
#define cuenta1s 122 //Desbordamientos para contar 1s con timer de 16 bits
#define cuenta05s 61 //Desbordamientos para contar 0.5s con timer de 16 bits
#define cuenta02s 24 //Desbordamientos para contar 0.2s con timer de 16 bits


//Variable tipo enum para definir el estado en el que nos encontramos en cada momento
typedef enum {FUNCIONANDO,EMERGENCIA,DISPONIBLE} estado;

// Funciones generales del balancín

void configuracion_balancin();
void control_led_L3();
void emergencia();
	


#endif /* BALANCIN_H_ */