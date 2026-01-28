/*
 * ADC_libreria.c
 *
 * Created: 27/01/2026
 *  Author: ialma
 */ 

#include "ADC_libreria.h"

void ADC_Init(uint8_t canal_ADC) {
	ADMUX = 0;
	ADMUX |= (1 << REFS0) | (canal_ADC & 0x07); // AVcc como referencia
	ADCSRA = 0;
	ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Habilitar ADC con prescaler de 128 (16MHz/128 = 125kHz)
	ADCSRA |= (1<<ADSC); // Inicia primera conversión del ADC
}