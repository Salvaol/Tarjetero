/*
 * tarjetero.c
 *
 * Created: 5/10/2016 11:18:16 AM
 *  Author: Miguel
 */ 

#include "tarjetero.h"

unsigned int personas=0;
char enable_L1=0;
unsigned int desb_L1=15;
int PORT_L1=0;

unsigned int tiempo_N_S01 = 0;
unsigned int tiempo_B_S01 = 0;


unsigned int tiempo1 = 0;
unsigned int tiempo2 = 0;



unsigned int tarjeta_valida = 0; //Da un 1 cuando se logra una lectura válida

unsigned int tarjeta[24]; //Buffer de almacenamiento de la lectura

unsigned int filtro=2; //Filtro para eliminar Flancos espurios

unsigned int bandera=0; //Indica si se espera en Flanco de subida o de bajada

unsigned int primero=1; //Indica si es el primer flanco de subida o el segundo de cada bit.

int k = 0; //Para recorrer el vector de lectura "tarjeta[24]";

unsigned int seguridad=0; 

int vector_socios[] = {789,456,642,97,123,531};

int nTarjeta=0; //Aquí se almacenará el número de tarjeta leído en formato decimal para compararlo con los socios existentes

void flanco_subida();

double bit=0;

double c=3;
double c0=0;
double c1=0;
double c2=0;
double c3=0;
double c4=0;
double c5=0;
double c6=0;


char enable_tarjetero=0;
unsigned int desb_3s=45;

void configuracion_tarjetero() {
	
	cli(); //Deshabilitación general de las interrupciones 
	
		//Entradas/salidas
		
S01DDR &= 0b01111111; //S01 como entrada.

L1DDR  |= 0b01000000; //L1 como salida, poner el ultimo bit a 0

L1PORT |= 0b00000001;

//Registros para los timers


TCCR3B |= 0b00000010; // Timer/Counter 3 Control configuración del Timer prescalado/8  y del Input Capture bit(6) a 0 por flanco de bajada

TIMSK3 |= 0b00100001; // Timer/Counter3 (16 bits), Input Capture


sei(); //Habilitación general de las interrupciones

}


void control_led_L1(){ 
	//Función que apaga el LED tras el tiempo requerido (1seg) y restaura los valores del enable_L1 y 
						//la cuenta de Desbordamiento
	ledL1_OFF();
	enable_L1=0;
	desb_L1=15;
}

//INTERRUPCION temporal por OVERFLOW

 ISR (TIMER3_OVF_vect) { 
	 //Interrupción cada vez que se desborda un timer de 16 bits
	//Se desenmascara la cuenta de 1 Segundo cuando la lectura de la tarjeta es válida
	
	if(enable_L1==1) {desb_L1--;
		
			if(desb_L1==0) {control_led_L1();} }
				
	if (enable_tarjetero==1){desb_3s--;
			if(desb_3s==0){
				primero = 1; //se indica que el siguiente primer flanco de bajada tras el filtro, ya es el primer negro de la tarjeta.
				k = 0;
				c=3;		//índice que recorre el vector donde se almacenan los 1 y 0 leídos.
				filtro = 2;
				enable_tarjetero=0;
			}
	}
	
		
}
 
 
 //flanco_subida() es la función que hemos desarrollado en ensamblador.
 
/*
void flanco_subida() {
	//Hay que reiniciar el timer
	
	tiempo2 = ICR3;
	bandera=0;
	primero=0;
	
	tiempo_N_S01=tiempo2-tiempo1;
	
	
	TCCR3B &= 0b10111111;	
}
*/


//Esta es la función que se activa cuando se registra un flanco de bajada en el S01. Es decir, cuando se pasa de blanco, a negro. 
		//Por defecto, el S01 se encuentra arriba, en blanco.
		
void flanco_bajada() {
	enable_tarjetero=1;
	tiempo1 = ICR3; //Se almacena el valor del inputCapture3 en la variable tiempo 1.
	
	if(primero!=1) { 
		//Esta condición solamente se cumple cuando no es el primer flanco de bajada de la tarjeta. 
						//Esto es necesario porque se requiere el tiempo2 primero antes de poder calcular el tiempo que ha estado en Blanco (ARRIBA).
		
		tiempo_B_S01=tiempo1-tiempo2;
		//Se calcula el tiempo que el Sensor ha estado leyendo blanco (ARRIBA). Para después poderlo comparar con el tiempo que ha estado en negro (ABAJO).
		
		//if (tiempo_N_S101>8*tiempo_B_S01){k=0;
									//	primero=1;
									//	filtro=2;} //esta línea de código es un filtro para reiniciar la lectura de la tarjeta 
												//en el caso de haber pasado el final o el principio.
												//El tiempo en Negro, tiene que ser mucho mayor que el tiempo en blanco 
												//(equivale a las bandas anchas negras final e inicial de la tarjeta)
												
		/*if(tiempo_B_S01>8*c*tiempo_N_S01) {k=0;
										primero=1;
										filtro=2;}*/
		if(k==0)  {c0=2*(double)tiempo_N_S01/tiempo_B_S01;
		
					if(c0<c && c0>1 && c0 < 3)c=c0;}
		
		if(k==1)  {c1=2*(double)tiempo_N_S01/tiempo_B_S01;
		
					if(c1<c && c1>1 && c1 < 3)c=c1;}
		if(k==2)  {c2=2*(double)tiempo_N_S01/tiempo_B_S01;
		
					if(c2<c && c2>1 && c2 < 3)c=c2;}
		if(k==3)  {c3=2*(double)tiempo_N_S01/tiempo_B_S01;
		
					if(c3<c && c3>1  && c3 < 3)c=c3;}
					
		if(k==4)  {c4=2*(double)tiempo_N_S01/tiempo_B_S01;

					if(c4<c && c4>1 && c4 < 3)c=c4;}
		if(k==5)  {c5=2*(double)tiempo_N_S01/tiempo_B_S01;

				    if(c5<c && c5>1 && c5 < 3)c=c5;}
		if(k==6)  {c6=2*(double)tiempo_N_S01/tiempo_B_S01;

					if(c6<c && c6>1  && c6 < 3)c=c6;}
										
	
		if(c*tiempo_B_S01>tiempo_N_S01) {tarjeta[k]=1;  }  
			//Esta es la comparación definitiva para decidir si el bit es 0 o 1, en función de los tiempos ARRIBA y ABAJO
											//La corrección de escala de 2.10 para el tiempo Blanco se ha ajustado para que la lectura sea correcta.
											//Se debe a la sutil diferencia entre los tiempos percibidos por el S01.
			
		 //Se escribe 1 cuando el tiempo en blanco es notablemente mayor (aproximadamente el doble) que el tiempo negro.

		//else 
												
		else { tarjeta[k]=0; }  //Se escribe 0 cuando el tiempo en negro es notablemente mayor (aproximadamente el doble) que el tiempo blanco.

		//if (tiempo_N_S01>9*c*tiempo_B_S01) {
			//k=0;}
											//  primero=1;
											//	filtro=2;} //esta línea de código es un filtro para reiniciar la lectura de la tarjeta
												//en el caso de haber pasado el final o el principio.
												//El tiempo en Negro, tiene que ser mucho mayor que el tiempo en blanco
												//(equivale a las bandas anchas negras final e inicial de la tarjeta)		

		k++;			//Se avanza en la lectura de la tarjeta, hasta el bit siguiente.


		if(k==23) {  //Una vez se hayan leido los 24 dígitos, se ejecutan las siguientes instrucciones:
	
			primero = 1; //se indica que el siguiente primer flanco de bajada tras el filtro, ya es el primer negro de la tarjeta.
			k = 0;
			c=3;		//índice que recorre el vector donde se almacenan los 1 y 0 leídos.
			filtro = 4; //es necesario poner el filtro=4 porque hay habitualmente 2 flancos espurios  de bajada al principio y dos flancos al final
			nTarjeta=(8*tarjeta[8]+4*tarjeta[9]+2*tarjeta[10]+tarjeta[11])*100+(8*tarjeta[12]+4*tarjeta[13]+2*tarjeta[14]+tarjeta[15])*10+(8*tarjeta[16]+4*tarjeta[17]+2*tarjeta[18]+tarjeta[19]);
					//Esta pedazo de línea de código sirve para pasar el número leído a formato decimal y así comparar fácilmente con los números de socio.
			
			for(int i=0; i<6;i++) {	
				if(nTarjeta==vector_socios[i]) seguridad=1;} //Se compara el número leído con todos los socios. Si coincide, se pone la variable seguridad = 1 para activar el LED...
				
	
			if(seguridad == 1) {  
					ledL1_ON(); //Se enciende el LED
					enable_L1 = 1; //Se activa la cuenta por desbordamientos para contar 1 segundo.
					personas++;  //se aumenta la cola de personas
					seguridad=0; //Se baja la bandera de seguridad para esperar a una nueva comprobación.
				}
				
			
			
			
		}

		}
	
	bandera=1; //Una vez se ha almacenado el tiempo (ICR3) asociado al flanco de subida, 
				//se cambia la bandera y la máscara del Timer para que espere un flanco de bajada.
	
	TCCR3B |= 0b01000010;
	
	
	
}

ISR (TIMER3_CAPT_vect) {
	
	if(filtro!=0) {  //Filtro del que se ha hablado antes. Sirve para despreciar los primeros flancos y últimos de la tarjeta
					//Banda negra ancha y demás....
		filtro--;
	}
	
	else{
		
		if(bandera==1) flanco_subida();  //La variable bandera permite conocer que función activar según 
											//se trate de un flanco de subida, o un flanco de bajada
		
		else if(bandera==0) flanco_bajada();
		
	}		
		
	
}