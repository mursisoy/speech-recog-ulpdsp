close all
clear all

path('training_samples',path)
path('lib',path)

subjects = {'alberto' 'emanuel' 'javi' 'laura' 'miguel' 'mikel' 'pablo'};
voice_maps ={};
for i = 1:length(subjects)
   voice_maps{1,i} = voice_map(sprintf('%s1.wav',subjects{i}'), 4);
   voice_maps{2,i} = voice_map(sprintf('%s2.wav',subjects{i}'), 4); 
end

rmse = cell(length(subjects)*2,length(subjects)*2);
sumrmse = cell(length(subjects)*2,length(subjects)*2);
diffcum = cell(length(subjects)*2,length(subjects)*2);


CUMSUM_N = 600;

for i = 1:size(subjects,2)*2
    X = voice_maps{i};
    for j = 1:size(subjects,2)*2
        Y = voice_maps{j};
        N = size(X,2);
        M = size(Y,2);
        R = zeros(N, M);
        for k = 1:N
            for l = 1:M
                R(k,l) =  error_rms( X(:,k), Y(:,l));
            end
        end
        rmse{i,j} = R;
        l = sort(R(:)); 
        r =  cumsum(l(1:CUMSUM_N ));
        diffcum{i,j} = cumsum(Y) ./ cumsum(X);
    end
end

samesubject = zeros(size(subjects,2),CUMSUM_N,1);

subject_index = 1:2:size(subjects,2)*2;
for i = 1:length(subject_index)
    samesubject(i,:,:) = sumrmse{subject_index(i),subject_index(i)+1};
end

dif_subjects = sumrmse;

for i = 1:2:size(subjects,2)*2
    dif_subjects{i,i+1} = zeros(CUMSUM_N,1);
    dif_subjects{i+1,i} = zeros(CUMSUM_N,1);
    dif_subjects{i,i} = zeros(CUMSUM_N,1);
    dif_subjects{i+1,i+1} = zeros(CUMSUM_N,1);
end

sum_n = zeros(size(subjects,2)*2,size(subjects,2)*2);
SUM_N = 100;
for i = 1:numel(dif_subjects)
   sum_n(i) = sum(dif_subjects{i}(1:SUM_N));
end

%% MEAN

std_same = std(samesubject,[],1);

std_dif = zeros(numel(dif_subjects),CUMSUM_N,1);
for i = 1:numel(dif_subjects)
   std_dif(i,:) = dif_subjects{i};
end
std_dif = std(std_dif,[],1);

plot(std_same)
hold on
plot(std_dif)



