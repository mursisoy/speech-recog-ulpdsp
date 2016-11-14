clear all
close all

path('mats', path);

load mean_basic.mat
load mean_derivada.mat
load mean_norm.mat
load mean_stdmean.mat
load mean_stnorm.mat


a = mean_dif./mean_same;
b =  mean_derivada_dif./mean_derivada_same;
c = mean_norm_dif./mean_norm_same;
d = mean_stdmean_dif./mean_stdmean_same;
e = mean_stdnorm_dif./mean_stdnorm_same;


figure
plot (a);
hold on;
plot (b);
plot (c);
plot (d);
plot (e);
legend('basico', 'derivadas', 'norm media', 'norm std + media', 'norm std')
title('distancia media personas distintas / distancia media personas iguales')