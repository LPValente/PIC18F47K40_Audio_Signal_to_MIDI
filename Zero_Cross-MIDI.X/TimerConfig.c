
#include "TimerConfig.h"
#include <xc.h>

void TMR0Config(void){
	T0CON0bits.T016BIT = 0b1;
	T0CON1bits.T0CS = 0b010;
	T0CON1bits.T0ASYNC = 0b0;
	T0CON1bits.T0CKPS = 0b0010;
	T0CON0bits.T0OUTPS = 0b0000;
	T0CON0bits.T0EN = 0b1;
}

void TMR0Reset(void){
	T0CON0bits.T0EN = 0b0;
	TMR0 = 0;
	T0CON1bits.T0CKPS = 0b0010;
	T0CON0bits.T0OUTPS = 0b0000;
	T0CON0bits.T0EN = 0b1;
}

void TMR1Config(void){
	T1GCONbits.GE = 0b0;
	T1CONbits.RD16 = 0b1;
	T1CLKbits.CS = 0b0001;
	T1CONbits.nSYNC = 0b1;
	T1CONbits.CKPS = 0b10;
	T1CONbits.ON = 0b1;
}

void TMR1Reset(void){
	T1CONbits.ON = 0b0;
	TMR1H = 0;
	TMR1L = 0;
	T1CONbits.CKPS = 0b10;
	T1CONbits.ON = 0b1;
}

void TMR3Config(void){
	T3GCONbits.GE = 0b0;
	T3CONbits.RD16 = 0b1;
	T3CLKbits.CS = 0b0001;
	T3CONbits.nSYNC = 0b1;
	T3CONbits.CKPS = 0b10;
	T3CONbits.ON = 0b1;
}

void TMR3Reset(void){
	T3CONbits.ON = 0b0;
	TMR3H = 0;
	TMR3L = 0;
	T3CONbits.CKPS = 0b10;
	T3CONbits.ON = 0b1;
}

void CCP1Config(void){
	CCP1CONbits.CCP1MODE = 0b1001;
	CCP1CONbits.CCP1EN = 0b1;
	
	CCPR1 = 40000; // 10ms considerando timer Fosc/4 com prescaler 1:4
}
