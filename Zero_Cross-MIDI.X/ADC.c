
#include <xc.h>
#include "ADC.h"

void ADCConfig(void){
	/*
	 * port configuration
	 * channel selection
	 * ADC voltage reference selection
	 * ADC conversion clock
	 * Interrupt control
	 * Result Formatting
	 * Conversion trigger selection
	 * ADC acquisition time
	 * ADC precharge time
	 * Additional sample and hold capacitor
	 * guard ring outputs
	 */
	ADCON0bits.ADFM = 0b1;	// Se 1: justificado à direita; se 2: justificado à esquerda
	ADCON0bits.ADCS = 0b1;	// Oscilador FRC
	ADPCH = 0b000000;	// Canal 1
	ADREFbits.ADPREF = 0b00;// Refêrencia positiva VDD
	ADREFbits.ADNREF = 0b0; // Referência negativa VSS
	PIR1bits.ADIF = 0;	// Zera interrupção AD
	PIE1bits.ADIE = 0b0;	// Se 1 ativa interrupção do ADC
	INTCONbits.IPEN = 0b1;
	//INTCONbits.GIE = 0b1;
	//INTCONbits.PEIE = 0b1;
	ADCON0bits.ADON = 0b1;	// Ativa módulo ADC
}

void ADConvert(void){
	ADCON0bits.GO = 0b1;
	while(ADCON0bits.GO);
}