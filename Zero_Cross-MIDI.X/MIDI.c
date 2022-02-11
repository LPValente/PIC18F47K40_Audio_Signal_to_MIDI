
#include "MIDI.h"
#include <math.h>
#include <xc.h>

// Função para converter período para nota midi com pitch
void tempToMIDI(int* note, int* pitchM, int* pitchL, double temp){
	double tone = 0;	// Frequência
	float pError = 0;	// Desvio da nota em decimais
	int mPitchLSB = 0;	// Byte menos significante do "Pitch wheel"
	int mPitchMSB = 0;	// Byte mais significante do "Pitch wheel"
	int mNote = 0;		// Nota MIDI

	tone = 1000.0/temp;	// adquire a frequência a partir do periodo
	
	// Calcula o valor convertido da frequência para midi com parte decimal
	pError = 12*(log2((tone/440.0))) + 69.0;

	// Separa a parte inteira da decimal
	mNote   = (int) pError;
	pError -= (float) mNote;
	
	// Normaliza a nota e o erro perante o desvio em decimais
	if(pError>0.5){
		pError -= 0.5;
		mNote++;
	}else pError += 0.5;
	
	// Separa a informação em 7bits mais e 7bits menos significativos
	mPitchMSB = (int) 16384*pError;
	mPitchLSB = mPitchMSB%128;
	mPitchMSB = mPitchMSB/128;
	
	
	*note   = mNote;		// Salva a informação da nota MIDI no endereço de memoria note
	*pitchM = mPitchMSB;	// Salva a informação da nota MIDI no endereço de memoria pitchM
	*pitchL = mPitchLSB;	// Salva a informação da nota MIDI no endereço de memoria pitchL
}

// Função para converter período para nota midi com pitch
void freqToMIDI(int* note, int* pitchM, int* pitchL, double freq){
	float pError = 0;	// Desvio da nota em decimais
	int mPitchLSB = 0;	// Byte menos significante do "Pitch wheel"
	int mPitchMSB = 0;	// Byte mais significante do "Pitch wheel"
	int mNote = 0;		// Nota MIDI
	
	// Calcula o valor convertido da frequência para midi com parte decimal
	pError = 12*(log2((freq/440.0))) + 69.0;

	// Separa a parte inteira da decimal
	mNote   = (int) pError;
	pError -= (float) mNote;
	
	// Normaliza a nota e o erro perante o desvio em decimais
	if(pError>0.5){
		pError -= 0.5;
		mNote++;
	}else pError += 0.5;
	
	// Separa a informação em 7bits mais e 7bits menos significativos
	mPitchMSB = (int) 16384*pError;
	mPitchLSB = mPitchMSB%128;
	mPitchMSB = mPitchMSB/128;
	
	
	*note   = mNote;		// Salva a informação da nota MIDI no endereço de memoria note
	*pitchM = mPitchMSB;	// Salva a informação da nota MIDI no endereço de memoria pitchM
	*pitchL = mPitchLSB;	// Salva a informação da nota MIDI no endereço de memoria pitchL
}

// Função para receber sinal da entrada no ADC e converter para velocity MIDI

/* Função com inteiro, substituida por float
int ADToMIDI_AC(int in){
	// caso use retificador de onda completa, corrigir para aplicar de 0-1023 AD
	
	// valor máximo ADC 1111111111b ou 0x3FF ou 1023
	// range velocity MIDI 0 - 127
	
	float v = (2.0 * (in+1) / 1023) - 1;// normaliza o sinal entre -1 e 1
	v = fabs(v);			// retira o módulo de v
	v = 20*log10((v/0.25));		// converte a amplitude para decibel
	
	// range de v -48.156311 dB - 12.041200 dB
	int out = (int) ((v+48)*2.1);
	return out;
}*/

int ADToMIDI_AC(float v){
	// caso use retificador de onda completa, corrigir para aplicar de 0-1023 AD
	
	// valor máximo ADC 1111111111b ou 0x3FF ou 1023
	// range velocity MIDI 0 - 127
	
	v = fabs(v);			// retira o módulo de v
	v = 20*log10((v/0.25));		// converte a amplitude para decibel
	
	// range de v -48.156311 dB - 12.041200 dB
	int out = (int) ((v+48)*2.1);
	return out;
} 

// Função com inteiro, substituida por float entre 1 e 0
int ADToMIDI_DC(int in){
	// caso use retificador de onda completa, corrigir para aplicar de 0-1023 AD
	
	// valor máximo ADC 1111111111b ou 0x3FF ou 1023
	// range velocity MIDI 0 - 127
	
	float v = (in+1) / 1024.0;	// normaliza o sinal entre 0 e 1
	v = 20*log10((v/0.2512));	// converte a amplitude para decibel
	
	// range de v -48.156312848 dB - 12.049686285 dB
	int out = (int) ((v+48)*2.1);
	return out;
}

/*
int ADToMIDI_DC(float v){
	// caso use retificador de onda completa, corrigir para aplicar de 0-1023 AD
	
	// valor máximo ADC 1111111111b ou 0x3FF ou 1023
	// range velocity MIDI 0 - 127
	
	v = 20*log10((v+0.001)/0.2512); // converte a amplitude para decibel
	
	// range de v -48.156312848 dB - 12.049686285 dB
	int out = (int) ((v+48)*2.1);
	return out;
}*/

void sendMIDI_ON(int ch, int note, int velocity){
	// terminar de escrever as funções
	TX1REG = 0b10010000 + ch;
	while(!PIR3bits.TX1IF);
	TX1REG = note;
	while(!PIR3bits.TX1IF);
	TX1REG = velocity;
	while(!PIR3bits.TX1IF);
}

void sendMIDI_OFF(int ch, int note, int velocity){
	// terminar de escrever as funções
	TX1REG = 0b10000000 + ch;
	while(!PIR3bits.TX1IF);
	TX1REG = note;
	while(!PIR3bits.TX1IF);
	TX1REG = velocity;
	while(!PIR3bits.TX1IF);
}

void sendMIDIPitch(int ch,int pitchM, int pitchL){
	TX1REG = 0b11100000 + ch;
	while(!PIR3bits.TX1IF);
	TX1REG = pitchL;
	while(!PIR3bits.TX1IF);
	TX1REG = pitchM;
	while(!PIR3bits.TX1IF);
	
}

void configMIDI(void){
	//inicializar SPxBRGH, SPxBRGL e BRGH e BRG16
	SP1BRGL = 31; //conferir o valor
	TX1STAbits.BRGH = 0;
	BAUD1CONbits.BRG16 = 0;
	//selecionar o o pino de transmissão utilizando o registrador RxyPPS
	RC6PPS = 0x09;
	//SYNC = 0, SPEN = 1
	TX1STAbits.SYNC = 0b0;
	RC1STAbits.SPEN = 0b1;
	//TXEN = 1
	TX1STAbits.TXEN = 0b1;
	TX1STAbits.TX9 = 0b0;
	PIE3bits.TX1IE = 0b0;
}

// criar outras funções midi;
