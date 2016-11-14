function [r] = mapcomp( name1, name2, escalado)

n1 = audioread(name1);
n2 = audioread(name2);
n1 = n1(:,1);
n2 = n2(:,1);


X = voice_map(n1, escalado);
Y = voice_map(n2, escalado);
[a, N] = size(X);
[b, M] = size(Y);
R = zeros(N, M);

for i = 1:N
    for j = 1:M
        R(i,j) =  error_rms( X(:,i), Y(:,j));
    end
end

%figure; imagesc(R)
l = sort(R(:)); 

r = l(1:600);%sum(l(1:m));
end

