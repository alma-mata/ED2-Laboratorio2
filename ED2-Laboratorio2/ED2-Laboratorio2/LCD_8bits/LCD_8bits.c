/*
 * Universidad del Valle de Guatemala
 * IE3054: Electrónica Digital 2
 * LCD_8bits.c
 * Autor: Alma Lisbeth Mata Ixcayau
 * Proyecto: Laboratorio 2 - Digital 2
 * Descripcion: Libreria para el uso de pantalla LCD en 8bits
 * Creado: 27/01/2026
*/

#include "LCD_8bits.h"

#define ENABLE (1 << PORTB2)
#define RS (1 << PORTB3)

void LCD_comando(char dato){
	PORTB &= ~(RS);			// RS = 0, modo comando
	mostrar_dato(dato);		// Se muestra el dato
	PORTB |= ENABLE;		// E = 1, envio de datos
	_delay_ms(1);
	PORTB &= ~ENABLE;		// E = 0, cierre de envio de datos
	_delay_ms(1);
}

void mostrar_dato(char dato){
	PORTD = (PORTD & 0x03) | ((dato << 2) & 0xFC);	// Muestra los primeros 6 bits en D2-D7
	PORTB = (PORTB & 0xFC) | ((dato >> 6) & 0x03);	// Muestra lo otros 2 bits en B0 y B1
}

void LCD_init8(){
	PORTB &= ~(RS);			// RS = 0, modo comando
	PORTB &= ~ENABLE;		// E = 0
	_delay_ms(20);			// delay indicado por el fabricante
	LCD_comando(0x30);		// Comando especificado por el fabricanto x3
	_delay_ms(5);
	LCD_comando(0x30);
	_delay_ms(5);
	LCD_comando(0x30);
	_delay_ms(10);
	// COMANDOS INDICADOS POR EL FABRICANTE
	LCD_comando(0x38);	// Usar matriz 5x8
	LCD_comando(0x0C);	// display encendido
	LCD_comando(0x01);	// Limpiar LCD
	LCD_comando(0x06);	// Comenzar a almacenar en la DDRAM
}

void set_cursor_LCD(char fila, char columna){
	if (fila == 0) LCD_comando(0x80 + columna);	// Posicionarse en linea 1
	if (fila == 1) LCD_comando(0xC0 + columna);	// Posicionarse en linea 2
}

void clear_LCD(){
	LCD_comando(0x01);	// Comando dado por el fabricante
}

void write_char_LCD(char dato){		// Escritura de un caracter en LCD
	PORTB |= RS;			// RS = 1, modo caracter
	mostrar_dato(dato);		// Escribir caracter
	PORTB |= ENABLE;		// E = 1, enviar dato
	_delay_ms(1);
	PORTB &= ~ENABLE;		// E = 0, cierre de envio de datos
	_delay_ms(1);
}

void write_string_LCD(char *cadena){	// Escritura de cadena en LCD
	int i;
	for (i = 0; cadena[i] != '\0'; i++){
		write_char_LCD(cadena[i]);
	}
}

void shiftRight_LCD(){		// Activa corrimiento a la derecha
	LCD_comando(0x1C); 
}

void shiftLeft_LCD(){		// Activa corrimiento a la izquierda
	LCD_comando(0x18);
}