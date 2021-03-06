/*
 * tarjetero.c
 *
 * Created: 5/10/2016 11:18:16 AM
 *  Author: Miguel
 */ 

#include "tarjetero.h"

unsigned int personas=4;
char enable_L1=0;
unsigned int desb_L1=15;
int PORT_L1=0;

unsigned int tiempo_N_S01 = 0;
unsigned int tiempo_B_S01 = 0;

int comparacion = 0;
int x =0;

unsigned int tiempo1 = 0;
unsigned int tiempo2 = 0;
unsigned int tiempo3 = 0;


unsigned int tarjeta_valida = 0; //Da un 1 cuando se logra una lectura válida

unsigned int tarjeta[24]; //Buffer de almacenamiento de la lectura

unsigned int socio[24]; //Tarjeta de socio

unsigned int filtro=2; //Filtro para eliminar Flancos espurios

unsigned int bandera=0; //Indica si se espera en Flanco de subida o de bajada

unsigned int primero=1; //Indica si es el primer flanco de subida o el segundo de cada bit.

int k = 0; //Para recorrer el vector de lectura "tarjeta[24]";

unsigned int seguridad=0; 








void configuracion_tarjetero() {
	
	cli(); //Deshabilitación general de las interrupciones 
	
		//Entradas/salidas
		//DDR: 0 -->entrada 1-->salida
		
S01DDR &= 0b01111111; //S01 como entrada.

L1DDR  |= 0b01000000; //L1 como salida, poner el ultimo bit a 0





L1PORT |= 0b00000001;




//Registros para los timers


TCCR3B |= 0b00000010; // Timer/Counter 3 Control configuración del Timer a la velocidad del Reloj y del input capture

TIMSK3 |= 0b00100001; // Timer/Counter3 (16 bits), Input Capture


//ATENCION BORRAR!!!

//
//DDRE = 0x01;
//
//PORTE=0b11111111;


sei(); //Habilitación general de las interrupciones

	
}

void control_led_L1(){ //
	ledL1_OFF();
	enable_L1=0;
	desb_L1=15;
}

//INTERRUPCIONES

ISR (TIMER3_OVF_vect) { //Interrupción cada vez que se desborda un timer de 16 bits
	//Se desenmascara la cuenta de 1 Segundo cuando la lectura de la tarjeta es válida
	
	if(enable_L1==1) {desb_L1--;
		
			if(desb_L1==0) {control_led_L1();} }
				
	if(bandera==0){}
		
}


void flanco_subida() {
	//Hay que reiniciar el timer
	
	tiempo2 = ICR3;
	bandera=0;
	primero=0;
	
	tiempo_N_S01=tiempo2-tiempo1;
	
	TCCR3B &= 0x10111111;


	
}


void flanco_bajada() {
	
	
	
	
	tiempo1 = ICR3;
	
	if(primero!=1) {
		
		tiempo_B_S01=tiempo1-tiempo2;
		
		
		if(tiempo_B_S01>tiempo_N_S01) {tarjeta[k]=1;  }
			
		
		else { tarjeta[k]=0; }
			
	
		if(tarjeta[k]==socio[k]) {seguridad++;}


		k++;


		if(k==23) {
	
			primero = 1;
			k = 0;
			filtro = 4;
	
	
			if(seguridad == 24) {
				ledL1_ON();
				enable_L1 = 1;
				personas++;
		
			}
		}

		}
	
	bandera=1;
	
	TCCR3B |= 0b01000010;
	
	
	
}

ISR (TIMER3_CAPT_vect) {
	
	if(filtro!=0) {
		filtro--;
	}
	
	else{
		
		if(bandera==1) flanco_subida();
		
		else if(bandera==0) flanco_bajada();
		
	}		
		
	
}