clear all
close all

path('mats', path);

load std_basic.mat
load std_derivada.mat
load std_norm.mat
load std_stdmean.mat
load std_stdnorm.mat


figure
plot (std_same);
hold on;
plot (std_derivada_same);
plot(std_norm_same);
plot (std_stdmean_same);
plot (std_stdnorm_same);

legend('basico', 'derivadas', 'norm media', 'norm media +std', 'norm std')
title('std voces personas iguales')

figure
plot (std_dif);
hold on;
plot (std_derivada_dif);
plot(std_norm_dif);
plot (std_stdmean_dif);
plot (std_stdnorm_dif);

legend('basico', 'derivadas', 'norm media', 'norm media + sdt', 'norm std')
title('std voces persona distinta')