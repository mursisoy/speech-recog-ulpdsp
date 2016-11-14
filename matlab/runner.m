clear all
close all

m = 500;%muestras a sumar de matriz distancias
escalado = 4; % de umbral (dividir media)


misma = mapcomp('alberto1.wav', 'alberto2.wav', escalado);
distinta = mapcomp('alberto1.wav', 'miguel1.wav', escalado);

dif = (cumsum(distinta) ./ cumsum(misma));


figure;plot(dif)
grid on;