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
#include "LCD_8bits/LCD_8bits.h"	// Librería para manejo de LCD
#include "ADC/ADC_libreria.h"		// Librería ADC
#include "UART/UART.h"				// Librería UART

/****************************************/
// Prototipos de función
void setup();				// Configuración general
void write_float();			// Conversión de ADC a decimal 0-5V
void UART_counter(void);	// Contador de entrada de UART

// Variables globales
volatile uint16_t POT1 = 0;			// Entrada Potenciometro 1 ADC
volatile uint16_t POT2 = 0;			// Entrada potenciometro 2 ADC
volatile uint8_t canal_ADC = 0;		// canal para configuración ADC
volatile uint8_t received_RX = 0;	// Dato recibido del UART
volatile uint8_t dato_ENVIADO = 0;	// Bandera de dato enviado UART
volatile uint8_t contador_UART = 0; // Contador UART

uint8_t entero_POT1 = 0;			// Parte entero del valor del potenciometro
uint8_t decimal_POT1 = 0;			// Parte decimal

/****************************************/
// Función principal

int main(void)
{
	setup();		// Se llama a la configuración general
	
    while (1) 
    {	
		set_cursor_LCD(0, 0);		// Se coloca cursor en fila 0
		char buffer[32];			// Variable para cadena de salida
		sprintf(buffer, " S1     S2   S3");	
		write_string_LCD(buffer);			// Se escribe en LCD
		
		set_cursor_LCD(1, 0);		// Se coloca cursor en fila 1
		write_float();				// Conversion valor POT1
		UART_counter();				// Contador UART
		sprintf(buffer, "%d.%02dV  %04d  %03d", entero_POT1, decimal_POT1, POT2, contador_UART);
		write_string_LCD(buffer);	// Se escribe en la LCD
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
	// Inicializacion de UART
	UART_init();
	sei();
}

void write_float(){ // Función que convierte el valor ADC (0-1023) a voltaje (0-5v)
	float conversion = ((POT1 * 5.00)/1023);
	entero_POT1 = conversion;
	decimal_POT1 = (conversion - entero_POT1) *100;
}

void UART_counter(void){ // Aumenta el contador UART si se ha mandado el carácter específico
	if (dato_ENVIADO)
	{
		dato_ENVIADO = 0;
		if (received_RX == '+')
		{
			contador_UART++;
		}
		else if (received_RX == '-')
		{
			contador_UART--;
		}
	}
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

ISR(USART_RX_vect){			// Interrupción UART
	received_RX = UDR0;
	dato_ENVIADO = 1;
}