/*
 * Universidad del Valle de Guatemala
 * IE3054: Electrónica Digital 2
 * ED2-Laboratorio2.c
 * Autor: Alma Lisbeth Mata Ixcayau
 * Proyecto: Laboratorio 2 - Digital 2
 * Descripcion: Uso de pantalla LCD con ADC y UART
 * Creado: 27/01/2026
*/
/****************************************/
// Encabezado
#define F_CPU 16000000UL	//Frecuencia es 16Mhz
#include <avr/io.h>
#include <avr/interrupt.h>	// Librería de interrupciones
#include <stdint.h>
#include "LCD_8bits/LCD_8bits.h"

/****************************************/
// Prototipos de función
void setup();

// Variables globales


/****************************************/
// Función principal

int main(void)
{
	setup();
	// Prueba de escritura
	set_cursor_LCD(0, 0);
	write_string_LCD("HOLA");
    while (1) 
    {
		_delay_ms(100);
    }
}

/****************************************/
// Subrutinas sin Interrupcion
void setup(void)
{
	cli();
	// Configuracion de puertos
	DDRD = 0xFF;	// Todo el PORTD es salida
	PORTD = 0x00;	// Se apagan los pines del PORTD
	
	DDRB = 0x0F;	// Primeros 4 bits son salidas
	PORTB = 0x00;	// Se apagan los pines del PORTB
	
	//Inicializacion de LCD
	LCD_init8();
	clear_LCD();
	
	sei();
}

/****************************************/
// Subrutinas de Interrupcion