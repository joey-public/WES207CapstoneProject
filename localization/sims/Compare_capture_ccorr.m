% This script compares two signals from the same capture received at 2
% different receivers and finds the approximate time difference between
% them
clear all
close all

%% PARAMETERS
threshold = 4000; %0m, 10dB gain
threshold2 = threshold;

%10m
%threshold = 800;
%threshold2 = 950;

%threshold = 390; %threshold to determine when signal is received. will need to figure out a way to set this
fs = 25e6; %sample rate of USRP capture (Hz)
pulseWidth = 20e-3; %seconds
beforeAfterPulseTime = 5e-3; %time on either side of pulse for filtering and xcorr (seconds)

%use idxoffset to ignore samples at beginning of file
%for some reason there is always a spike near the beginning
%so default is 20
idxoffset = 20;
%idxoffset = 2.8e7;

%%
% read data from file
%datafile = fopen('0527/raw_data_vinit.bin'); % filename of USRP capture File
%datafile2 = fopen('0527/raw_data_joey_20230525.bin'); % filename of USRP capture File
datafile = fopen('0527/raw_data_0m_10dB_joey.bin'); % filename of USRP capture File
datafile2 = fopen('0527/27_may_Vinit/raw_data0m_10dB_vin.bin'); % filename of USRP capture File
%datafile = fopen('0527/raw_data_20dB_0m_maxLen_joey.bin'); % filename of USRP capture File
%datafile2 = fopen('0527/27_may_Vinit/raw_data10m_20dB_max_vin.bin'); % filename of USRP capture File
filedata = fread(datafile,'int16'); % data from USRP is stored in 16 bit integers
filedata2 = fread(datafile2,'int16'); % data from USRP is stored in 16 bit integers

%remove timestamps
%timestamps consist of a 64 bit integer and a double and are spaced every 10000 samples
%so remove those 128 bits of data by removing the first 8 16 bit elements
%of each 20008 elements of this array
%(The data is stored in 20000 elements because the real and complex parts
%are stored separately)
% samples = reshape(filedata,20008,[]);
% samples = reshape(samples(9:end,:),[],1);
% 
% clear filedata
% 
% samples2 = reshape(filedata2,20008,[]);
% samples2 = reshape(samples2(9:end,:),[],1);
% 
% clear filedata2

%16 bit integers alternate between the real and imaginary parts
realdata = filedata(1:2:end);
imagdata = filedata(2:2:end);

clear filedata

realdata2 = filedata2(1:2:end);
imagdata2 = filedata2(2:2:end);

clear filedata2

data = realdata+1j*imagdata; %combine into one vector
data2 = realdata2+1j*imagdata2; %combine into one vector

%[~, loc] = max(abs(realdata));.
startidx = find(abs(data(idxoffset+1:end))>threshold,1,'first');
startidx = startidx + idxoffset;

startidx_rec2 = find(abs(data2(idxoffset+1:end))>threshold2,1,'first');
startidx_rec2 = startidx_rec2 + idxoffset;

%% Plot Received Signal
% figure(1)
% plot(real(data(loc-500000:loc+500000)))
% 
% figure(2)
% plot(imag(data(loc-500000:loc+500000)))

startidx1 = max(startidx-500000,idxoffset);
startidx1_rec2 = max(startidx_rec2-500000,idxoffset);
% samples_to_plot = startidx1:startidx1+1000000;
% figure(21)
% plot(samples_to_plot,abs(data(samples_to_plot)))
% grid on
% xlabel("Sample Number")
% ylabel("Amplitude")
% title("Magnitude of Received Signal")

%% Plot real and imaginary parts of received signal near arrival time
startidx2 = max(startidx-5000,1);
startidx2_rec2 = max(startidx_rec2-5000,1);
% samples_to_plot = startidx2:startidx2+15000;
% figure(3)
% plot(samples_to_plot,realdata(samples_to_plot))
% hold on
% plot(samples_to_plot,imagdata(samples_to_plot))
% hold off
% grid on
% xlabel("Sample Number")
% ylabel("Amplitude")
% title("Arrival of Signal")

%% Plot magnitude of received signal near arrival time
% figure(31)
% plot(samples_to_plot,abs(data(samples_to_plot)))
% grid on
% xlabel("Sample Number")
% ylabel("Amplitude")
% title("Magnitude of Signal Near Arrival")

%% Plot phase of signal near arrival time
% figure(4)
% plot(samples_to_plot,angle(data(samples_to_plot)))
% grid on
% xlabel("Sample Number")
% ylabel("Phase")
% title("Measured Phase Near Arrival of Signal")

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


%% Compare multiple transmissions
% [pks, locs] = findpeaks(abs(data),'MinPeakHeight',threshold,'MinPeakDistance',fs/10);
% 
% figure(6)
% bar(locs/fs,pks)
% grid on
% xlabel("Time (s)")
% ylabel("Max Magnitude of Transmission")
% title("Magnitude Comparison of Signal Captures over time")

% fignum = 100;
% for k = 1:length(locs)
%     if(locs(k) > 500000)
%         figure(fignum+k)
%         samples_to_plot = locs(k)-500000:locs(k)+500000;
%         plot(samples_to_plot,abs(data(samples_to_plot)))
%     end
% end

%% FFT
fftlen = 10000;

startidx3 = max(startidx-3000,1);
startidx3_rec2 = max(startidx_rec2-3000,1);
samples_to_plot = startidx1:startidx1+1000000;
%samples_to_plot = startidx3:startidx3+fftlen-1;
f = (-1/2:1/fftlen:1/2-1/fftlen)*fs;
% figure(6)
% plot(f,20*log10(fftshift(abs(fft(data(samples_to_plot),fftlen)))));
% grid on
% xlabel("Frequency (Hz)")
% ylabel("Magnitude Squared")
% title("Spectrum of Received Signal")

%% LPF
%[lpf_b,lpf_a] = cheby2(10,80,1e5/(fs/2));
lpf_b = firpm(1000,[0 50000 100000 fs/2]/(fs/2),[1 1 0 0]);
%lpf_b = ones(1000,1);
lpf_a = 1;
H_lpf = fftshift(abs(fft(lpf_b,fftlen)));

% samples_to_plot = startidx2+1-8000:startidx2+18000;
% data_to_filter = data(samples_to_plot);
% 
% %samples_to_plot_rec2 = (startidx2_rec2+1-20000):(startidx2_rec2+12000);
% %samples_to_plot_rec2 = (startidx2_rec2+1-20000):(startidx2_rec2+12000);
% data_to_filter2 = data2(samples_to_plot_rec2);

pulseWidthSamples = round(pulseWidth*fs);
beforeAfterPulseSamples = round(beforeAfterPulseTime*fs);

samples_to_plot = startidx2-beforeAfterPulseSamples+1:startidx2+pulseWidthSamples+beforeAfterPulseSamples;
data_to_filter = data(samples_to_plot);

samples_to_plot_rec2 = startidx2_rec2-beforeAfterPulseSamples+1:startidx2_rec2+pulseWidthSamples+beforeAfterPulseSamples;
data_to_filter2 = data2(samples_to_plot_rec2);

filtered_data = filter(lpf_b,lpf_a,data_to_filter);
filtered_data2 = filter(lpf_b,lpf_a,data_to_filter2);

% filtered_data_real = filter(lpf_b,lpf_a,real(data_to_filter));
% filtered_data_imag = filter(lpf_b,lpf_a,imag(data_to_filter));
% filtered_data = filtered_data_real + 1j*filtered_data_imag;

% figure(7)
% samples_to_plot = startidx:startidx+fftlen-1;
% f = (-1/2:1/fftlen:1/2-1/fftlen)*fs;
% plot(f,20*log10(fftshift(abs(fft(filtered_data,fftlen)))));
% grid on
% xlabel("Frequency (Hz)")
% ylabel("Magnitude Squared")
% title("Spectrum of Filtered Signal")

f = (-1/2:1/fftlen:1/2-1/fftlen)*fs;
% figure(71)
% plot(f,20*log10(H_lpf));
% grid on
% xlabel("Frequency (Hz)")
% ylabel("Magnitude Squared")
% title("Filter Frequency Response")

% figure(8)
% plot(abs(filtered_data))
% grid on
% xlabel("Sample Number")
% ylabel("Amplitude")
% title("Magnitude of Filtered Signal Near Arrival")

%% Compare phase and magnitude of Filtered signal near arrival time
filtered_plot_offset = 500000;

figure(91)
%samples_to_plot = filtered_plot_offset:filtered_plot_offset+3000;
hold on
yyaxis left
ylabel('Amplitude')
plot(samples_to_plot,real(filtered_data))
plot(samples_to_plot,imag(filtered_data))
yyaxis right
plot(samples_to_plot,angle(filtered_data))
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
plot(samples_to_plot_rec2,real(filtered_data2))
plot(samples_to_plot_rec2,imag(filtered_data2))
yyaxis right
plot(samples_to_plot_rec2,angle(filtered_data2))
ylabel('Phase')
hold off
grid on
xlabel("Sample Number")
title("Amplitude and Phase of Signal Arrival (Receiver 2)")

%% Cross Correlation
[xc, idxs]= ccorr(filtered_data,filtered_data2,length(data_to_filter));
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

[xc2, idxs2] = ccorr(abs(filtered_data),abs(filtered_data2),length(data_to_filter));
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
[xcu, idxsu]= ccorr(data_to_filter,data_to_filter2,length(data_to_filter));
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

[xc2u, idxs2u] = ccorr(abs(data_to_filter),abs(data_to_filter2),length(data_to_filter));
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
