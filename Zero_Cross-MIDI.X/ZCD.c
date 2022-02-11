
#include "ZCD.h"
#include <xc.h>

void ZCDConfig(void){
	PIR2bits.ZCDIF = 0b0;
	PIE2bits.ZCDIE = 0b1;
	
	// caso ambos estejam setados teremos o período sendo metade do real
	ZCDCONbits.INTP = 0b1;
	ZCDCONbits.INTN = 0b0;
	
	//INTCONbits.PEIE = 0b1;
	//INTCONbits.GIE = 0b1;
	ZCDCONbits.POL = 0b0;
}

void ZCDPriority(void){
	INTCONbits.IPEN = 0b1;
	IPR2bits.ZCDIP = 0b0;
	IPR1bits.ADIP = 0b0;
	IPR2bits.C1IP = 0b1;
	
}