clear all
close all

path('training_samples',path)
path('lib',path)

%% Establecemos f muestreo y periodos bloque

voz = audioread('alberto1.wav');
voz = voz(:,1);
down = 3;
s = resample(voz, 1, down);
L = length(s);

fs = 48e3/down;

%soundsc(s, fs);

T1 = 25*1e-3; %cogemos 25 ms
inc = 10*1e-3; %paso 10 ms
N1 = T1*fs;%muestras por bloque
In = inc*fs;

fprintf(' %dms son %d muestras. El incremento es de %dms, %d muestras\n', T1*1000, N1, inc*1000, In  ); 

%% separamos audio en bloques

p = 1:In:L-N1;
N = length(p); %N bloques
bloques = zeros(N,N1);
pot_bloq = zeros(1,N);

for i = 1:N
    bloques(i,:) = s(p(i):p(i)+N1-1);
    pot_bloq(i) = sum(abs(bloques(i,:)));
end
fprintf(' tamano bloque=  %d x %d \n', size(bloques))


%% Realizamos fft bloques zero padding hasta N

%suma muestras cuadradas cada ventana. Establecer umbral descarte ventana
%por energia para elimninar silencio
escalado = 2;
m_pot = mean(pot_bloq);
umbral_pot = m_pot/escalado;
g = (pot_bloq > umbral_pot);

figure; plot(g*max(pot_bloq)); hold on
plot(pot_bloq)
xlabel('tiempo')
title('seleccion de ventanas por energia')

bloques = bloques(g,:);
N = sum(g);

%% umbral de potencia 

fftsize = roundtowardvec(N1, 2.^(1:15), 'ceil'); %funcion de M.filexchange
S1 = zeros(N,fftsize);

for i = 1:N
    S1(i,:) = abs(fft(bloques(i,:), fftsize)); %valor absoluto
end

S2 = S1(:,1:fftsize/2);

figure; imagesc(S2);title('abs fft parte simetrica ')
fprintf(' tamano fft */2* =  %d x %d \n', size(S2))

%% Filtro de Mel

H = mel_fb(fftsize/2, 24, fs);

fprintf(' tamano mel_filterbank =  %d x %d \n', size(H))
figure; imagesc(H);title('mel filter-bank')

%% producto filtro por fft

Y = S2*H;

fprintf(' tamano filtrado =  %d x %d \n',size(Y));

Y = log(Y);
figure; imagesc(Y);title('fft filtrada (log)')
colormap jet;
%% dct a senal Y 

D = dctmtx(24);
figure; imagesc(D)
X = D(2:17,:)*Y';
%% normalizaciones + derivadas

% m = mean(X,2);%forzamos media nula
% X = X - m*ones(1,N);
St = std(X, [], 2);%forzamos varianza 1
X = X./(St*ones(1,N));
% 
% d1 = [-1.0000,   -0.7500,   -0.5000,   -0.2500,         0,    0.2500,    0.5000,   0.7500,    1.0000];
% d2 = [ 1.0000,    0.2500,   -0.2857,   -0.6071,   -0.7143,   -0.6071,   -0.2857,   0.2500,    1.0000];
% X_d  = filter2(d1,X);
% X_dd = filter2(d2,X);
% X = [X; X_d; X_dd];

figure; imagesc(X); fprintf(' tamano post DCT =  %d x %d \n',size(X))

