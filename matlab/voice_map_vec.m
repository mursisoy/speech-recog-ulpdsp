function X = voice_map_vec(file, down, debug)

fs = 48e3/down;

FRAME = 25*1e-3; %cogemos 25 ms
T = 10*1e-3; %paso 10 ms
N = FRAME*fs;%muestras por bloque
STEP = T*fs;

(debug) && fprintf(' %dms son %d muestras. El incremento es de %dms, %d muestras\n', FRAME*1000, N, T*1000, STEP ); 

s = audioread(file);
s = s(:,1);
s = resample(s, 1, down);

blocks = buffer(s,N,N-STEP,'nodelay'); 

blocks_power = sum(blocks.^2,1);

(debug) && fprintf(' tamano bloque=  %d x %d \n', size(blocks));

g = power_filter(blocks_power, 1/4);

if (debug)
    figure; 
    plot(g*max(blocks_power)); 
    hold on
    plot(blocks_power)
    title('Removed blocks below 1/4 of power mean')
end

blocks = blocks(g,:);

% First transformation (FFT)

fftsize = roundtowardvec(N, 2.^(1:15), 'ceil'); % Calc FFT points (funcion de M.filexchange)

S = abs(fft(blocks, fftsize, 2)); % FFT along 2nd dimension

S = S(:,1:fftsize/2); % Remove symmetric part

if (debug)
 figure; imagesc(S2);title('abs fft parte simetrica ')
 fprintf(' tamano fft */2* =  %d x %d \n', size(S2))
end

% Filtro de Mel
H = mel_fb(fftsize/2, 24, fs); % Mel filter bank

(debug) && fprintf(' tamano mel_filterbank =  %d x %d \n', size(H));

Y = S*H; % Convolution with mel filter bank

(debug) && fprintf(' tamano filtrado =  %d x %d \n',size(Y));

Y = log(Y); 

if (debug)
    figure; imagesc(Y);title('fft filtrada')
    colormap jet;
end

D = dctmtx(24); % Calculate 24 DCT basis
X = D(2:13,:)*Y'; % Convolve with Y











