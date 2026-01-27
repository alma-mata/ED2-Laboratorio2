/*
 * Universidad del Valle de Guatemala
 * IE3054: Electrónica Digital 2
 * LCD_8bits.h
 * Autor: Alma Lisbeth Mata Ixcayau
 * Proyecto: Laboratorio 2 - Digital 2
 * Descripcion: Libreria para el uso de pantalla LCD en 8bits
 * Creado: 27/01/2026
*/ 

#ifndef LCD_H_
#define LCD_H_

#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
void LCD_comando(char dato);
void mostrar_dato(char dato);
void LCD_init8();
void set_cursor_LCD(char fila, char columna);
void clear_LCD();
void write_char_LCD(char dato);
void write_string_LCD(char *cadena);
void shiftRight_LCD();
void shiftLeft_LCD();

#endif /* LCD_H_ */