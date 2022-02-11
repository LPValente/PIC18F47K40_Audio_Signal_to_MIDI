clear
clc

% Função que relaciona a nota midi à frequência de um sinal
% N = [0:1:127];

f =@(N) 440*2.^((N-69)/12);

figure
fplot(f,[0,127]);
title('Frequência X Nota MIDI');
grid on

% Função que relaciona Amplitude de um sinal normalizado entre -1 e 1 e seu valor em dB
%{
res = 10;
ini = 2^(res-1);
fin = 2^res-1;
in = [ini:1:fin];

v = ((2.*in)/fin) -1;

VdB = 20*log10((v./1));
v = 4.5.*v;
r = range(VdB);

figure
plot(v,VdB);
title('Amplitude (dBV) X Amplitude (V)');
grid on
%}

% Função que relaciona Amplitude de um sinal normalizado entre 0 e 1 e seu valor em dB
res = 10;
bits = [0:1:((2^res)-1)];
v = ((bits.+1)/1024);

VdB =@(v) 20*log10(((v/1024)/0.2512));

figure
fplot(VdB,[1,1024]);
title('Amplitude (dBV) X Amplitude (V)');
grid on



f =@(dB) dB*2.1+100

figure
fplot(f,[-48,12]);
title ('Amplitude (dBV) X Velocity MIDI');
grid on