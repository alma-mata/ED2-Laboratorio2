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
#include <stdio.h>
#include "LCD_8bits/LCD_8bits.h"
#include "ADC/ADC_libreria.h"

/****************************************/
// Prototipos de función
void setup();
void write_float();

// Variables globales
volatile uint16_t POT1 = 0;
volatile uint16_t POT2 = 0;
volatile uint8_t canal_ADC = 0;

uint8_t entero_POT1 = 0;
uint8_t decimal_POT1 = 0;
uint8_t centesima_POT1 = 0;

/****************************************/
// Función principal

int main(void)
{
	setup();
	
    while (1) 
    {	
		set_cursor_LCD(0, 0);
		char buffer[32];
		write_float();
		sprintf(buffer, "S1: %d.%02dV", entero_POT1, decimal_POT1);
		write_string_LCD(buffer);
		
		set_cursor_LCD(1, 0);
		write_float();
		sprintf(buffer, "S2: %d", POT2);
		write_string_LCD(buffer);
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
	
	// Inicializacion de ADC
	ADC_Init(canal_ADC);
	sei();
}

void write_float(){
	float conversion = ((POT1 * 5.00)/1023);
	entero_POT1 = conversion;
	decimal_POT1 = (conversion - entero_POT1) *100;
}

/****************************************/
// Subrutinas de Interrupcion
ISR(ADC_vect){				// Leer canal y asignarlo al POT correspondiente
	switch (canal_ADC){
		case 0: POT1 = ADC; break;
		case 1: POT2 = ADC; break;
	}
	canal_ADC = (canal_ADC + 1) % 2;
	ADMUX = (ADMUX & 0xF0) | (canal_ADC & 0x03);
	ADCSRA |= (1 << ADSC); // Volver a iniciar conversion
}