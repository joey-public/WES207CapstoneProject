% This script compares two pulse data files received at the host controller
% by cross correlating them
clear all
close all

%%
fs = 25e6; %sample rate of USRP capture (Hz)
fc = 173935300; %center frequency of USRP capture (Hz)
pulseWidth = 20e-3; %seconds
beforeAfterPulseTime = 5e-3; %time on either side of pulse for filtering and xcorr (seconds)

% read data from file
datafile = fopen('0603/HostController/run9hostctrl/pulse_data_1.bin');
datafile2 = fopen('0603/HostController/run9hostctrl/pulse_data_2.bin');
filedata = fread(datafile,'int16'); % data from USRP is stored in 16 bit integers
filedata2 = fread(datafile2,'int16'); % data from USRP is stored in 16 bit integers

%16 bit integers alternate between the real and imaginary parts
realdata = filedata(1:2:end);
imagdata = filedata(2:2:end);

clear filedata

realdata2 = filedata2(1:2:end);
imagdata2 = filedata2(2:2:end);

clear filedata2

data = realdata+1j*imagdata; %combine into one vector
data2 = realdata2+1j*imagdata2; %combine into one vector

pulseWidthSamples = round(pulseWidth*fs);
beforeAfterPulseSamples = round(beforeAfterPulseTime*fs);
startidx2 = beforeAfterPulseSamples;
startidx2_rec2 = beforeAfterPulseSamples;

%% Compare phase and magnitude of signal near arrival time
figure(51)
samples_to_plot = startidx2+2000:startidx2+6000;
hold on
yyaxis left
ylabel('Amplitude')
plot(samples_to_plot,realdata(samples_to_plot))
plot(samples_to_plot,imagdata(samples_to_plot))
yyaxis right
plot(samples_to_plot,angle(data(samples_to_plot)))
ylabel('Phase')
hold off
grid on
xlabel("Sample Number")
title("Amplitude and Phase of Signal Arrival (Receiver 1)")

figure(52)
samples_to_plot_rec2 = startidx2_rec2+2000:startidx2_rec2+6000;
hold on
yyaxis left
ylabel('Amplitude')
plot(samples_to_plot_rec2,realdata2(samples_to_plot_rec2))
plot(samples_to_plot_rec2,imagdata2(samples_to_plot_rec2))
yyaxis right
plot(samples_to_plot_rec2,angle(data2(samples_to_plot_rec2)))
ylabel('Phase')
hold off
grid on
xlabel("Sample Number")
title("Amplitude and Phase of Signal Arrival (Receiver 2)")

clear realdata
clear imagdata
clear realdata2
clear imagdata2

%% FFT
fftlen = 10000;
f = (-1/2:1/fftlen:1/2-1/fftlen)*fs+fc;
figure(61)
plot(f,20*log10(fftshift(abs(fft(data,fftlen)))))
grid on
xlabel("Frequency (Hz)")
ylabel("Magnitude Squared (dB)")
title("Spectrum of First Pulse")

figure(62)
plot(f,20*log10(fftshift(abs(fft(data2,fftlen)))))
grid on
xlabel("Frequency (Hz)")
ylabel("Magnitude Squared (dB)")
title("Spectrum of Second Pulse")


%% LPF
fftlen = 10000;
%[lpf_b,lpf_a] = cheby2(10,80,1e5/(fs/2));
lpf_b = firpm(1000,[0 50000 100000 fs/2]/(fs/2),[1 1 0 0]);
%lpf_b = ones(1000,1);
lpf_a = 1;
H_lpf = fftshift(abs(fft(lpf_b,fftlen)));

filtered_data = filter(lpf_b,lpf_a,data);
filtered_data2 = filter(lpf_b,lpf_a,data2);




%% Compare phase and magnitude of Filtered signal near arrival time
filtered_plot_offset = 500000;

figure(91)
%samples_to_plot = filtered_plot_offset:filtered_plot_offset+3000;
hold on
yyaxis left
ylabel('Amplitude')
plot(real(filtered_data))
plot(imag(filtered_data))
yyaxis right
plot(angle(filtered_data))
ylabel('Phase')
hold off
grid on
xlabel("Sample Number")
title("Amplitude and Phase of Signal Arrival (Receiver 1)")

figure(92)
%samples_to_plot_rec2 = filtered_plot_offset:filtered_plot_offset+3000;
hold on
yyaxis left
ylabel('Amplitude')
plot(real(filtered_data2))
plot(imag(filtered_data2))
yyaxis right
plot(angle(filtered_data2))
ylabel('Phase')
hold off
grid on
xlabel("Sample Number")
title("Amplitude and Phase of Signal Arrival (Receiver 2)")

%% Cross Correlation
[xc, idxs]= ccorr(filtered_data,filtered_data2,length(filtered_data));
figure(100)
plot(idxs, abs(xc))
[~, maxidx] = max(abs(xc));
grid on
xlabel("Sample Offset")
ylabel("Cross Correlation Magnitude")
title("Cross Correlation")
maxidx = idxs(maxidx)
sample_offset = samples_to_plot_rec2(1)-samples_to_plot(1)
TDoA = (sample_offset-maxidx)/fs

figure(101)
plot(idxs,real(xc))
hold on
plot(idxs,imag(xc))
hold off
grid on
xlabel("Sample Offset")
ylabel("Cross Correlation Magnitude")
title("Real and Imaginary Components of Cross Correlation")

[xc2, idxs2] = ccorr(abs(filtered_data),abs(filtered_data2),length(filtered_data));
figure(102)
plot(idxs2,xc2)
grid on
xlabel("Sample Offset")
ylabel("Cross Correlation Magnitude")
title("Cross Correlation of Signal Magnitudes")
[~, maxidx2] = max(xc2);
maxidx2 = idxs2(maxidx2)
sample_offset_mag = samples_to_plot_rec2(1)-samples_to_plot(1);
TDoA2 = (sample_offset_mag-maxidx2)/fs




xLoc = LocTest2Receivers(TDoA)


%% Testing cross correlation on unfiltered data
[xcu, idxsu]= ccorr(data,data2,length(data));
figure(103)
plot(idxsu, abs(xcu))
[~, maxidxu] = max(abs(xcu));
grid on
xlabel("Sample Offset")
ylabel("Cross Correlation Magnitude")
title("Cross Correlation")
maxidxu = idxs(maxidxu)
sample_offset_u = samples_to_plot_rec2(1)-samples_to_plot(1)
TDoAu = (sample_offset_u-maxidxu)/fs

figure(104)
plot(idxs,real(xcu))
hold on
plot(idxs,imag(xcu))
hold off
grid on
xlabel("Sample Offset")
ylabel("Cross Correlation Magnitude")
title("Real and Imaginary Components of Cross Correlation")

[xc2u, idxs2u] = ccorr(abs(data),abs(data2),length(data));
figure(105)
plot(idxs2u,xc2u)
grid on
xlabel("Sample Offset")
ylabel("Cross Correlation Magnitude")
title("Cross Correlation of Signal Magnitudes")
[~, maxidx2u] = max(xc2u);
maxidx2u = idxs2u(maxidx2u)
sample_offset_mag_u = samples_to_plot_rec2(1)-samples_to_plot(1);
TDoA2u = (sample_offset_mag_u-maxidx2u)/fs




xLocu = LocTest2Receivers(TDoAu)
