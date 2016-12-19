function [ X ] = voice_map( in, escalado )


% Establecemos f muestreo y periodos bloque

%load(string);
down = 3;
s = resample(in, 1, down);
L = length(s);

fs = 48e3/down;

%soundsc(s, fs);

T1 = 25*1e-3; %cogemos 25 ms
inc = 10*1e-3; %paso 10 ms
N1 = T1*fs;%muestras por bloque
In = inc*fs;

%fprintf(' %dms son %d muestras. El incremento es de %dms, %d muestras\n', T1*1000, N1, inc*1000, In  ); 

% separamos audio en bloques

p = 1:In:L-N1;
N = length(p); %N bloques
bloques = zeros(N,N1);
pot_bloq = zeros(1,N);

for i = 1:N
    bloques(i,:) = s(p(i):p(i)+N1-1);
    pot_bloq(i) = sum(bloques(i,:).^2);
end
%fprintf(' tamano bloque=  %d x %d \n', size(bloques))

m_pot = mean(pot_bloq);
umbral_pot = m_pot/escalado;
g = (pot_bloq > umbral_pot);
N = sum(g);
bloques = bloques(g,:);

% Realizamos fft bloques zero padding hasta N

fftsize = roundtowardvec(N1, 2.^(1:15), 'ceil'); %funcion de M.filexchange
S1 = zeros(N,fftsize);

for i = 1:N
    S1(i,:) = abs(fft(bloques(i,:), fftsize)); %valor absoluto
end

S2 = S1(:,1:fftsize/2);

%figure; imagesc(fliplr(S2)');title('abs fft parte simetrica ')
%fprintf(' tamano fft */2* =  %d x %d \n', size(S2))

% Filtro de Mel

H = mel_fb(fftsize/2, 24, fs);

%fprintf(' tamano mel_filterbank =  %d x %d \n', size(H))
%figure; imagesc(H);title('mel filter-bank')

% producto filtro por fft

Y = S2*H;

Y = log(Y);
%fprintf(' tamano filtrado =  %d x %d \n',size(Y));
%figure; imagesc(fliplr(Y)');title('fft filtrada')

% dct a senal Y

D = dctmtx(24);
X = D(2:17,:)*Y';

% normalizaciones y derivadas

%  m = mean(X,2);
%  X = X - m*ones(1,N);%media 0
 St = std(X, [], 2);
 X = X./(St*ones(1,N));%varianza 1

% d1 = [-1.0000,   -0.7500,   -0.5000,   -0.2500,         0,    0.2500,    0.5000,   0.7500,    1.0000];
% d2 = [ 1.0000,    0.2500,   -0.2857,   -0.6071,   -0.7143,   -0.6071,   -0.2857,   0.2500,    1.0000];
% X_d  = filter2(d1,X);
% X_dd = filter2(d2,X);
% X = [X; X_d; X_dd];

%figure; imagesc(X); fprintf(' tamano post DCT =  %d x %d \n',size(X))

end

