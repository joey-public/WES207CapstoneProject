## processing.m

### Use data kept in the path in zipped folder processing.zip
https://drive.google.com/drive/folders/1xOBHKqa7txcgBovVi-ar4vuvSNH4SdP2?usp=drive_link

Contains raw data analysis. FFT plot, magnitude plot.

## Filter design
This is sinc filter with kaise window designed for Attenuation 80dB. The USRP collects samples at baseband. So, we go for Low pass filter. Cutoff could be played with, for us, 67 taps worked well.

%Filter specifications:

fs = 25e6; %in MHz
f1 = 0; %in MHz
f2 = 2e6; %in MHz

A_db = 80;

Beta = A_db/10;

N = floor((fs/(f2-f1))*A_db/15);
if rem(N,2) == 0
    N=N+1;
end
NN = (N-1)/2;
phi = 2*pi*(-NN:NN)*(f1+f2)/(2*fs);
h = sin(phi)./phi;
h(NN+1) = 1;
h0 = h.*kaiser(2*NN+1, Beta)';
h1 = h0*(f2+f1)/fs;


