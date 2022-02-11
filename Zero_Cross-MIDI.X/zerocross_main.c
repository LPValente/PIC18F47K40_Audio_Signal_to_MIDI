/*
 * File:   zerocross_main.c
 * Author: Lucas Pedrosa
 *
 * Created on 26 de Dezembro de 2020, 21:40
 */

/*
 * Este código será utilizado por um microcontrolador escravo
 * que mandará informações para um microcontrolador mestre
 * por meio do protocolo i2c ou spi
 */


// PIC18F47K40 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config FEXTOSC = OFF    // External Oscillator mode Selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINTOSC_64MHZ// Power-up default value for COSC bits (HFINTOSC with HFFRQ = 64 MHz and CDIV = 1:1)

// CONFIG1H
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)

// CONFIG2L
#pragma config MCLRE = EXTMCLR  // Master Clear Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR )
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (Power up timer disabled)
#pragma config LPBOREN = OFF    // Low-power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled , SBOREN bit is ignored)

// CONFIG2H
#pragma config BORV = VBOR_2P45 // Brown Out Reset Voltage selection bits (Brown-out Reset Voltage (VBOR) set to 2.45V)
#pragma config ZCD = ON         // ZCD Disable bit (ZCD always enabled)
#pragma config PPS1WAY = ON     // PPSLOCK bit One-Way Set Enable bit (PPSLOCK bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config DEBUG = OFF      // Debugger Enable bit (Background debugger disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Extended Instruction Set and Indexed Addressing Mode disabled)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled)

// CONFIG3H
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG4L
#pragma config WRT0 = OFF       // Write Protection Block 0 (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection Block 1 (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection Block 2 (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection Block 3 (Block 3 (00C000-00FFFFh) not write-protected)
#pragma config WRT4 = OFF       // Write Protection Block 3 (Block 4 (010000-013FFFh) not write-protected)
#pragma config WRT5 = OFF       // Write Protection Block 3 (Block 5 (014000-017FFFh) not write-protected)
#pragma config WRT6 = OFF       // Write Protection Block 3 (Block 6 (018000-01BFFFh) not write-protected)
#pragma config WRT7 = OFF       // Write Protection Block 3 (Block 7 (01C000-01FFFFh) not write-protected)

// CONFIG4H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-30000Bh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)
#pragma config SCANE = ON       // Scanner Enable bit (Scanner module is available for use, SCANMD bit can control the module)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (HV on MCLR/VPP must be used for programming)

// CONFIG5L
#pragma config CP = OFF         // UserNVM Program Memory Code Protection bit (UserNVM code protection disabled)
#pragma config CPD = OFF        // DataNVM Memory Code Protection bit (DataNVM code protection disabled)

// CONFIG5H

// CONFIG6L
#pragma config EBTR0 = OFF      // Table Read Protection Block 0 (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection Block 1 (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection Block 2 (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection Block 3 (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR4 = OFF      // Table Read Protection Block 4 (Block 4 (010000-013FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR5 = OFF      // Table Read Protection Block 5 (Block 5 (014000-017FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR6 = OFF      // Table Read Protection Block 6 (Block 6 (018000-01BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR7 = OFF      // Table Read Protection Block 7 (Block 7 (01C000-01FFFFh) not protected from table reads executed in other blocks)

// CONFIG6H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <pic18f47k40.h>
#include "TimerConfig.h"
#include "MIDI.h"
#include "ADC.h"
#include "ZCD.h"

#define _XTAL_FREQ 64000000UL

unsigned int compTime = 60000; // variável para comparar com TMR3

double temp = 0.0;
int cont = 0;


int midiNote = 0;	// 0 a 127 60 sendo o C4
int midiVelocity = 96;	// 0 a 127 linear, necessita usar dB para linearizar
int midiPitchMSB = 0;	// "pitchwheel" mais significativo
int midiPitchLSB = 0;	// "pitchwheel" menos significativo
int midiChannel = 0;	// canal midi, até 16 canais 0-Fh

int atNote = 0;		// nota atual para verificar troca
int atPitchM = 0;	// pitch mais significativo atual
int atPitchL = 0;	// pitch menos significativo atual
int atChannel = 0;	// canal midi atual

int pAux = 0;
int aux = 0;

void __interrupt() general_Interrupts(void){
	
	if(PIR6bits.CCP1IF){
		TMR1Reset();
		
		/*if(midiVelocity < 5){
			sendMIDI_OFF(midiChannel, atNote, 0);
			midiVelocity = 0;
		}*/
		
		// Usar função abaixo para pegar informação da quinta corda
		// Função para converter periodo para nota midi com pitch
		tempToMIDI(&midiNote, &midiPitchMSB, &midiPitchLSB, temp);
		
		/*
		 * criar função para pegar os valores de outros pics por i2c
		 * 1 -> E5 - E7
		 * 2 -> A4 - A6
		 * 3 -> D4 - D6
		 * 4 -> G3 - G5
		 * 
		 * Conferir cada uma das frequências para projetar o filtro analógico
		 */ 
		
		
		//midiVelocity = 0;
		
		PIR6bits.CCP1IF = 0b0;
	}
	
	if(PIR2bits.ZCDIF){	// caso sinal cruze o zero no pino RB0/ZCDIN
		//Corrigir timer para clock de 64MHz
		aux = TMR3;
		TMR3Reset();		// zera timer
		
		if(aux <= compTime){	// para clock de 64MHz com clock Fosc/4 e prescaler 1:4 equivale a 15ms
			
			temp = aux/4000.0; // considerando cruzamento positivo e negativo
			// O valor seria de 4*10^6 porém fora preferível distribuir esse valor em 4000.0 aqui
			// e 1000 na função de conversão
			
			
		}
		
		//pulso++;
		
		PIR2bits.ZCDIF = 0;	// desativa interrupção
	}
	
	//Corrigir para fazer RMS
	if(PIR1bits.ADIF){
		pAux = ADToMIDI_DC(ADRES); // Converte o valor adquirido no ADC para dB
		
		if(midiVelocity < pAux){ // salva o maior valor encontrado por ciclo
			midiVelocity = pAux; 
		}
		PIR1bits.ADIF = 0b0;
	}
	
}

void main(){
	/*
	 * Configurar portas		- ok
	 * Configurar timer		- ok verificar
	 * Configurar ADC		- ok verificar
	 * Configurar EUSART para MIDI	- ok
	 * Configurar ZCD		- ok
	 */
	
	//Configurar portas
	LATA = 0;
	LATB = 0;
	LATC = 0;
	
	LATAbits.LA0 = 0b0;
	TRISAbits.TRISA0 = 0b1;		//RA0 == ANA0
	ANSELAbits.ANSELA0 = 0b1;	// desativa o buffer de input digital
	
	LATBbits.LB0 = 0b0;
	TRISBbits.TRISB0 = 0b1;		//RB0 == ZCD
	TRISCbits.TRISC6 = 0b0;		//RC6 == TX1(EUSART)
	
	// Desativa interrupções
	INTCONbits.GIE = 0b0;
	INTCONbits.PEIE = 0b0;
	
	ZCDConfig();	// Configura módulo ZCD
	TMR1Config();	// Configura módulo timer1
	TMR3Config();	// Configura módulo timer3
	
	// corrigir para usar outro timer
	CCP1Config();	// Configura módulo comparador
	ADCConfig();	// Configura módulo ADC
	configMIDI();	// Configura módulo EUSART1 para MIDI
	ZCDPriority();
	
	// Ativa interrupções
	INTCONbits.GIE = 0b1;
	INTCONbits.PEIE = 0b1;
	
	/*char tes  = 0;
	
	for(tes = 0;tes < 128;tes++)
		sendMIDI_OFF(midiChannel, tes, 0);
	*/
	while(1){
		//criar função para ficar sempre verificando o valor de pico do sinal
		
		ADConvert(); // Aciona a conversão AD
		/*
		//mandar mensagem de pitch
		if((midiPitchMSB != atPitchM) || (midiPitchLSB != atPitchL)){
			sendMIDIPitch(midiChannel,midiPitchMSB,midiPitchLSB);
			atPitchM = midiPitchMSB;
			atPitchL = midiPitchLSB;
		}*/
		
		if(midiNote != atNote){
			// mandar mensagem note off da nota anterior
			sendMIDI_OFF(midiChannel, atNote, 0);
			// mandar mensagem note on da nota atual
			sendMIDI_ON(midiChannel, midiNote, 96);
			//sendMIDI_ON(midiChannel, midiNote, midiVelocity);
			// pegar o valor da variável que guarda o valor de pico do sinal
			atNote = midiNote;
		
			midiVelocity = 0;
		}
	}
	
}

