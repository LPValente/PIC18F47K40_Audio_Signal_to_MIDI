#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){
	int in = 510;
	int i = 0;
	float v = 0;
	
	//while(in<=1023){
	while(1){
		printf("Digite o valor da entrada:\n");
		scanf("%d", &in);
		
		v = (in + 1) / 1024.0;
		
		//printf("Valor normalizado:\n");
		//printf("%f\n", v);
		
		//v= fabs(v);
		
		v = 20*log10((v/0.2512));
		//printf("%f\n", v+48);
		
		i = (int) ((v + 48)*2.1);
		//i = 0b0111;
		printf("Valor em dB e MIDI:\n");
		printf("%f, %d\n\n", v, i);
		sleep(1);
		
		while(!1) printf("não entra\n");
		//in++;
	}
}
