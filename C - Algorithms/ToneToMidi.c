#include <stdio.h>
#include <math.h>
#include <time.h>


void main(){

    double tone = 0;
    float pError = 0;
    int mPitchLSB = 0;
    int mPitchMSB = 0;
    int mNote = 0;
    
    while(1){
	
    printf("digite a frequencia:\n");

    scanf("%lf", &tone);
    
	pError = 12*(log2(tone/440.0)) + 69.0;
	
    mNote = pError;
    pError -= mNote;
    if(pError>0.5){
        pError -= 0.5;
        mNote++;
    }else pError += 0.5;
    mPitchMSB = 16384*pError;
    mPitchLSB = mPitchMSB%128;
    mPitchMSB = mPitchMSB/128;

    printf("Nota MIDI:\n%d\n\n", mNote);
    /*printf("Byte menos significativo de modulacao de pitch:\n%d\n", mPitchLSB);
    printf("Byte mais significativo de modulacao de pitch:\n%d\n", mPitchMSB);
    printf("Valor inteiro da informação de modulacao de pitch:\n%d\n", mPitchLSB + (mPitchMSB*128));
    printf("Valor decimal da modulacao de pitch:\n%f\n", pError);*/
	}
}
