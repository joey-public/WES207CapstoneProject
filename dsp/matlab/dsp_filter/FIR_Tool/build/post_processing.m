clc;
close all;
clear all;
%Download data from Google Drive for the script to work
%https://drive.google.com/drive/folders/1xOBHKqa7txcgBovVi-ar4vuvSNH4SdP2?usp=drive_link
fs = 25e6;

complex_data = load("filtered_data.mat");
real_data = complex_data.real_data;
imag_data = complex_data.imag_data;

complex_data = complex(real_data,imag_data);

figure;
subplot(4, 1, 1);
plot(1:length(complex_data), abs(complex_data));
grid on
xlabel('Filtered data from C++');
ylabel('Magnitude');
title('Magnitude of Complex Data 10m 20dB');

subplot(4, 1, 2);
plot(1:length(complex_data), angle(complex_data));
%plot(xval, angle(complex_data2(xval)));
grid on
xlabel('Sample Index');
ylabel('Phase (radians)');
title('Phase of Complex Data template');

subplot(4,1,3)
plot(1:length(complex_data), real(complex_data));
hold on
grid on
plot(1:length(complex_data), imag(complex_data));
xlabel("Time n")
ylabel("Amplitude")

subplot(4,1,4)
fftlen = 2*length(complex_data);
plot((-0.5:1/fftlen:0.5-1/fftlen)*fs,20*log10(fftshift(abs(fft(real(complex_data),fftlen)))), 'linewidth',2)
grid on
title("FFT of Filtered data from C++")
xlabel('Frequency fo/fs', 'fontsize',14)
ylabel('Log Magnitude','fontsize',14 )
xlim([-fs/2 fs/2])





