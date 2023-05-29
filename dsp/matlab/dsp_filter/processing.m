clc;
close all;
clear all;

fs = 25e6;

temp = load("template_signal.mat");
pulse = load("complex_data_pulse.mat");
noise = load("noise.mat");

template_signal = temp.template_signal;
complex_pulse = pulse.complex_data_pulse;
noise_data = noise.noise_data;

% Perform further processing or analysis on complex_data
% For example, you can plot the magnitude and phase:
figure;
subplot(4, 1, 1);
plot(1:length(template_signal), abs(template_signal));
grid on
xlabel('Template Sample Index');
ylabel('Magnitude');
title('Magnitude of Complex Data 0m 10dB');

subplot(4, 1, 2);
plot(1:length(template_signal), angle(template_signal));
%plot(xval, angle(complex_data2(xval)));
grid on
xlabel('Sample Index');
ylabel('Phase (radians)');
title('Phase of Complex Data template');

subplot(4,1,3)
plot(1:length(template_signal), real(template_signal));
hold on
grid on
plot(1:length(template_signal), imag(template_signal));
xlabel("Time n")
ylabel("Amplitude")

subplot(4,1,4)
fftlen = 2*length(template_signal);
plot((-0.5:1/fftlen:0.5-1/fftlen)*fs,20*log10(fftshift(abs(fft(real(template_signal)/sum(template_signal),fftlen)))), 'linewidth',2)
grid on
title("FFT of Template signal")
xlabel('Frequency fo/fs', 'fontsize',14)
ylabel('Log Magnitude','fontsize',14 )
xlim([-fs/2 fs/2])

%Plotting unfiltered complex signal
figure;
subplot(4, 1, 1);
plot(1:length(complex_pulse), abs(complex_pulse));
grid on
xlabel('Complex Sample Index');
ylabel('Magnitude');
title('Magnitude of Complex Data 10m 20dB');

subplot(4, 1, 2);
plot(1:length(complex_pulse), angle(complex_pulse));
%plot(xval, angle(complex_data2(xval)));
grid on
xlabel('Sample Index');
ylabel('Phase (radians)');
title('Phase of Complex Data');

subplot(4,1,3)
plot(1:length(complex_pulse), real(complex_pulse));
hold on
grid on
plot(1:length(complex_pulse), imag(complex_pulse));
xlabel("Time n")
ylabel("Amplitude")

subplot(4,1,4)
fftlen = 2*length(complex_pulse);
plot((-0.5:1/fftlen:0.5-1/fftlen)*fs,20*log10(fftshift(abs(fft(real(complex_pulse)/sum(complex_pulse),fftlen)))), 'linewidth',2)
grid on
title("FFT of Complex signal at 10m 20dB")
xlabel('Frequency fo/fs', 'fontsize',14)
ylabel('Log Magnitude','fontsize',14 )
xlim([-fs/2 fs/2])

% Do match filter with complex pulse
%Filp the template signal for match filtering.
%template = fliplr(template_signal); % Matched filter template
%filtered_signal = filter(template,1, complex_pulse);
%filtered_signal = c_corr(template_signal, complex_pulse);


% figure;
% subplot(4, 1, 1);
% plot(1:length(filtered_signal), abs(filtered_signal));
% grid on
% xlabel('Filtered Sample Index');
% ylabel('Magnitude');
% title('Magnitude of Filtered Data 10m 20dB');
% 
% subplot(4, 1, 2);
% plot(1:length(filtered_signal), angle(filtered_signal));
% %plot(xval, angle(complex_data2(xval)));
% grid on
% xlabel('Sample Index');
% ylabel('Phase (radians)');
% title('Phase of Filtered Data');
% 
% subplot(4,1,3)
% plot(1:length(filtered_signal), real(filtered_signal));
% hold on
% grid on
% plot(1:length(filtered_signal), imag(filtered_signal));
% xlabel("Time n")
% ylabel("Amplitude")
% 
% subplot(4,1,4)
% fftlen = 2*length(filtered_signal);
% plot((-0.5:1/fftlen:0.5-1/fftlen)*fs,20*log10(fftshift(abs(fft(real(filtered_signal)/sum(filtered_signal),fftlen)))), 'linewidth',2)
% grid on
% title("FFT of Complex signal at 10m 20dB")
% xlabel('Frequency fo/fs', 'fontsize',14)
% ylabel('Log Magnitude','fontsize',14 )
% xlim([-fs/2 fs/2])
% 
% %noise and match filter
% %filtered_noise = filter(template,1,noise_data);
% filtered_noise = c_corr(template_signal, noise_data);
% 
% figure;
% subplot(3, 1, 1);
% plot(1:length(filtered_noise), abs(filtered_noise));
% grid on
% xlabel('Filtered Noise Index');
% ylabel('Magnitude');
% title('Magnitude of Filtered Noise 10m 20dB');
% 
% subplot(3, 1, 2);
% plot(1:length(filtered_noise), angle(filtered_noise));
% grid on
% xlabel('Sample Index');
% ylabel('Phase (radians)');
% title('Phase of Filtered Data');
% 
% subplot(3,1,3)
% plot(1:length(filtered_noise), real(filtered_noise));
% hold on
% grid on
% plot(1:length(filtered_noise), imag(filtered_noise));
% xlabel("Time n")
% ylabel("Amplitude")

%filter design
%HW_2 Problem 1 a

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


sinc_filtered_pulse = filter(h1,1, complex_pulse);

figure;
subplot(4, 1, 1);
plot(1:length(sinc_filtered_pulse), abs(sinc_filtered_pulse));
grid on
xlabel('Complex Sample Index');
ylabel('Magnitude');
title('Magnitude of Complex Data 10m 20dB');

subplot(4, 1, 2);
plot(1:length(sinc_filtered_pulse), angle(sinc_filtered_pulse));
%plot(xval, angle(complex_data2(xval)));
grid on
xlabel('Sample Index');
ylabel('Phase (radians)');
title('Phase of Complex Data');

subplot(4,1,3)
plot(1:length(sinc_filtered_pulse), real(sinc_filtered_pulse));
hold on
grid on
plot(1:length(sinc_filtered_pulse), imag(sinc_filtered_pulse));
xlabel("Time n")
ylabel("Amplitude")

subplot(4,1,4)
fftlen = 2*length(sinc_filtered_pulse);
plot((-0.5:1/fftlen:0.5-1/fftlen)*fs,20*log10(fftshift(abs(fft(real(sinc_filtered_pulse)/sum(sinc_filtered_pulse),fftlen)))), 'linewidth',2)
grid on
title("FFT of Complex signal at 10m 20dB")
xlabel('Frequency fo/fs', 'fontsize',14)
ylabel('Log Magnitude','fontsize',14 )
xlim([-fs/2 fs/2])

figure;
fftlen = 2*length(h1);
plot((-0.5:1/fftlen:0.5-1/fftlen)*fs,20*log10(fftshift(abs(fft(real(h1)/sum(h1),fftlen)))), 'linewidth',2)
grid on
title("FFT of Sinc filter with Kaise window")
xlabel('Frequency fo/fs', 'fontsize',14)
ylabel('Log Magnitude','fontsize',14 )
xlim([-fs/2 fs/2])

%Plot of noise with filtered data.
filtered_noise = filter(h1,1, noise_data);
figure;
fftlen = 2*length(filtered_noise);
plot((-0.5:1/fftlen:0.5-1/fftlen)*fs,20*log10(fftshift(abs(fft(real(filtered_noise)/sum(filtered_noise),fftlen)))), 'linewidth',2)
grid on
title("FFT of Noise signal at 10m 20dB")
xlabel('Frequency fo/fs', 'fontsize',14)
ylabel('Log Magnitude','fontsize',14 )
xlim([-fs/2 fs/2])

%Using FIRPM 
fs = 25e6;
AdB = 60;
f1 = 0;
f2 = 5e6;
N = round(fs/(f2-f1) * AdB/22);

h=firpm(N-1, [0 f1 f2 fs/2]/(fs/2), [1 1 0 0], [1 10]);

figure;
fftlen = 1000;
plot((-0.5:1/fftlen:0.5-1/fftlen)*fs,20*log10(fftshift(abs(fft(h/sum(h),fftlen)))), 'linewidth',2)
grid on
title("FFT of Firpm filter")
xlabel('Frequency fo/fs', 'fontsize',14)
ylabel('Log Magnitude','fontsize',14 )
xlim([-fs/2 fs/2])

firpm_filtered_pulse = filter(h,1, complex_pulse);

figure;
subplot(4, 1, 1);
plot(1:length(firpm_filtered_pulse), abs(firpm_filtered_pulse));
grid on
xlabel('Complex Sample Index with FIRPM filter');
ylabel('Magnitude');
title('Magnitude of Complex Data 10m 20dB');

subplot(4, 1, 2);
plot(1:length(firpm_filtered_pulse), angle(firpm_filtered_pulse));
%plot(xval, angle(complex_data2(xval)));
grid on
xlabel('Sample Index');
ylabel('Phase (radians)');
title('Phase of Complex Data');

subplot(4,1,3)
plot(1:length(firpm_filtered_pulse), real(firpm_filtered_pulse));
hold on
grid on
plot(1:length(firpm_filtered_pulse), imag(firpm_filtered_pulse));
xlabel("Time n")
ylabel("Amplitude")

subplot(4,1,4)
fftlen = 2*length(firpm_filtered_pulse);
plot((-0.5:1/fftlen:0.5-1/fftlen)*fs,20*log10(fftshift(abs(fft(real(firpm_filtered_pulse)/sum(firpm_filtered_pulse),fftlen)))), 'linewidth',2)
grid on
title("FFT of Complex signal at 10m 20dB with FIRPM")
xlabel('Frequency fo/fs', 'fontsize',14)
ylabel('Log Magnitude','fontsize',14 )
xlim([-fs/2 fs/2])

%Plot of noise with filtered data.
filtered_noise = filter(h,1, noise_data);
figure;
fftlen = 2*length(filtered_noise);
plot((-0.5:1/fftlen:0.5-1/fftlen)*fs,20*log10(fftshift(abs(fft(real(filtered_noise)/sum(filtered_noise),fftlen)))), 'linewidth',2)
grid on
title("FFT of Noise signal at 10m 20dB with FIRPM")
xlabel('Frequency fo/fs', 'fontsize',14)
ylabel('Log Magnitude','fontsize',14 )
xlim([-fs/2 fs/2])



%cross corelation function
function [ r_xy ] = c_corr( x, ref )
    % C_CORR calculated the shiftd cross-correlation
    % y or the ref is assumed to be shorter, conj will be taken of y
    temp = xcorr(x,ref);
    L = numel(x);
    r_xy = temp(L:end);
end