function [corrArray, lags] = ccorr(s1,s2,len)
%lags = -len+1:len-1;
lags = 0:2*len-2;
lags(len+1:length(lags)) = lags(len+1:length(lags))-(2*len-1);

%% Conjugage Way
s1zp = zeros(2*len-1,1);
s2zp = zeros(2*len-1,1);

s1zp(1:len) = s1; %s1, zero-padded
s2zp(1:len) = s2; %s2, zero-padded

corrArray = ifft(fft(s1zp).*conj(fft(s2zp)));
