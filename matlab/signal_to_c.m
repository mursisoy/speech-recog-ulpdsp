clear all
close all

path('training_samples',path)
path('lib',path)

voice = audioread('alberto1.wav');
voice = voice(:,1);

down = 3;
voice_w = resample(voice, 1, down);
blocks = buffer(voice_w,400,240,'nodelay')';
block = blocks(14,:)';

fid = fopen('voice.c','wt');
fprintf(fid,'signed int voz[%d] = {', size(voice_w,1));
fprintf(fid,'%d, ',int16(voice_w.*2^15));
fprintf(fid,'};');
fclose(fid);


fid = fopen('block.c','wt');
fprintf(fid,'signed int voz[%d] = {', size(block,1));
fprintf(fid,'%d, ',int16(block.*2^15));
fprintf(fid,'};');
fclose(fid);

fs = 16e3;
N = 400;
n = linspace(0,N/fs,N);
s = int16(sin(2*pi*2e3*n).*2^15)';
fid = fopen('sin.c','wt');
fprintf(fid,'signed int sin[%d] = {', size(s,1));
fprintf(fid,'%d, ',s);
fprintf(fid,'};');
fclose(fid);

fb = mel_fb(256, 24, 16e3);
fid = fopen('fb.c','wt');
fprintf(fid,'DATA mel_filter_bank[%d] = {', size(fb,1)* size(fb,2));
fprintf(fid,'%d, ',int16(reshape(fb',1,numel(fb)).*2^15));
fprintf(fid,'};');
fclose(fid);

D = dctmtx(24);
D = D(2:17,:);

fid = fopen('dct.c','wt');
fprintf(fid,'DATA dct_mat[%d] = {', numel(D));
fprintf(fid,'%d, ',int16(reshape(D,1,numel(D)).*2^15));
fprintf(fid,'};');
fclose(fid);

to_mel = abs(fft(block, 512));
to_mel = to_mel(1:256)';

to_dct = to_mel*fb;

fid = fopen('to_dct.c','wt');
fprintf(fid,'DATA to_dct[%d] = {', numel(to_dct));
fprintf(fid,'%d, ',int16((to_dct/(2*max(to_dct))*2^15)));
fprintf(fid,'};');
fclose(fid);

to_std_norm = to_dct*D';

plot(to_std_norm)

