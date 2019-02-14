/*
 * Funciones del balancín
 *
 * Created: 06/05/2016 12:08:12
 *  Author: Tmicro 4
 */ 


//Definición de variables globales

#include "balancin.h"

estado est_actual=DISPONIBLE;
unsigned int personas=4;
char enable_L3=0;
unsigned int desb_L3=0; //desbordamiento para el control de L3
unsigned int desb_L4_05s=0; //desbordamiento para el control de L4 modo disponible
unsigned int desb_L4_02s=0; //desbordamiento para el control de L4 modo parada emergencia
char control_L4=0; //Variable de control para el modo de funcionamiento de L4: vale 1 cuando se ha pulsado la parada de emergencia y 0 cuando la atracción está disponible pero no en funcionamiento
int PORT_L3=0;


void configuracion_balancin(){
	cli(); // Deshabilitación general de las interrupciones
	
	// Entradas/Salidas
	// DDR: 0-->entrada 1-->salida
	
    SO4DDR  &= 0b11111011; //  SO4 entrada.
	SO5DDR  &= 0b11110111; //  SO5 entrada.
    SW1DDR  &= 0b11101111; //  SW1 entrada.
    SW3DDR  &= 0b11011111; //  SW3 entrada.
    L3DDR   |= 0b01000000; //  L3 salida.
    L4DDR   |= 0b10000000; //  L4 salida.
    M2ENDDR |= 0b00001000; //  M2 enable salida.
    M2DIDDR |= 0b00000100; //  M2 direction salida
    M2BKDDR |= 0b00000000; //  M2 brake no se usa en principio
    
    // Registros para los timers
	
	TCCR1B |= 0x01; // Timer/Counter 1 Control Register B , sin preescalado para el reloj
    TIMSK1 |= 0x01; // Timer/Counter1 (16 bits), overflow interrupt enable, desbordamiento cada 8192us (cada 65536 ciclos)

    
    // Interrupciones externas de la maqueta
	// S04: PCINT18
	// S05: PCINT19
	// SW1: PCINT20
	// SW2: PCINT21
	
	/* Interrupciones */
    PCICR |= 0b00000100; // Mascara global que habilita las PCINT16:23
	//PCIFR |= 0b00000100; // Borro posibles interrupciones pendientes en las PCINT16:23 OJO borro todas!
    PCMSK2 |= 0b00111100; // Máscara local que habilita las PCINT 18,19,20,21
	
	
	//Si usaramos INTn en vez de PCINTn tendriamos que escribir lo siguiente
		//EICRB |= 0b10101111; // External Interrupt Control Register B --> 7-4 Sense Control bits
							 // 1 0 --> the falling edge between two samples of INTn generates an interrupt request    
		// EIMSK --> External Interrupt Mask Register
		// EIFR  --> External Interrupt Flag Register
	
	sei(); // Habilitación general de las interrupciones
	
	
}






//INTERRUPCIONES 

ISR (TIMER1_OVF_vect) { // Interrupción cada vez que desborda un timer de 16 bits
//Se enmascara cuando funcione la atracción porque sólo se enciende el L4
	//Interrupcion para el L3
	if(enable_L3==1)desb_L3++;	
	
	desb_L4_05s++;
	desb_L4_02s++;
	//Interrupcion para el control del led4 en estado DISPONIBLE
	if((est_actual==DISPONIBLE) && (desb_L4_05s < cuenta05s)) {
		ledL4_ON();
	}	 
		if (desb_L4_05s > cuenta05s) ledL4_OFF();
		if(desb_L4_05s == cuenta10s) desb_L4_05s=0;
			
		
	//Interrupcion para el control del led4 en estado EMERGENCIA
	if((est_actual==EMERGENCIA) && (desb_L4_02s < cuenta02s)) {
		ledL4_ON(); 
	}		
		if (est_actual==EMERGENCIA && desb_L4_02s > cuenta02s) ledL4_OFF();
		if(desb_L4_02s == cuenta1s) desb_L4_02s=0;
			
					
}	

ISR(PCINT2_vect) {
	//Emergencia
	if(SW1 == 0) est_actual =EMERGENCIA;
	
}


void control_led_L3(){ // El led 3 solo se ilumina cuando la cola de personas llega a 4. Se mantiene durante 10 s (1221 desbordamientos de Timer de 16 bits) mientras se carga la atracción
	
	enable_L3=1;
	while(desb_L3 < cuenta10s && est_actual!=EMERGENCIA) { 
		ledL3_ON(); 
		}
	ledL3_OFF();
	enable_L3=0;
	desb_L3=0;
	
}

void emergencia(){
	//Aquí deshabilitaría todas las interrupciones externas, menos la temporal del control del led (faltan)
	PCICR |= 0b00000000;
	
	//Apagamos todo
	motor_balancin_OFF();
	ledL4_OFF();
	ledL3_OFF();
	
	while(1){}
	
}	
