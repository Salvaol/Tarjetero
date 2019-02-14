/*
 * main.c para probar
 *
 * Created: 06/05/2016 11:56:19
 *  Author: Tmicro 4
 */ 

#include "balancin.h"


// definición de variables globales (declaración externa)
extern estado est_actual;


int main(void)
{
	configuracion_balancin();
	
	control_led_L3();
	
	if (est_actual==EMERGENCIA) emergencia();
    while(1)
    {
        //TODO:: Please write your application code 
    }
}