function [ x ] = error_rms( A, B )

if length(A) ~= length(B)
    fprintf('errorrrrrrrrr vecs no iguales en error_rms')
end
s = sum((double(B)-double(A)).^2);
x = sqrt(s./numel(A));

end

