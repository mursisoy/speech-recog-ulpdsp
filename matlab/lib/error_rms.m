function [ x ] = error_rms( A, B )

if length(A) ~= length(B)
    error('A y B deben tener la misma longitud error_rms')
end
s = sum((double(B)-double(A)).^2);
x = sqrt(s./numel(A));

end

