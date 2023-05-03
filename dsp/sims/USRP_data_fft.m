% This script makes various plots of the received signal to determine its
% characteristics
clear all
close all

% read data from file
datafile = fopen('test13_20m_nogain.dat'); % filename of USRP capture File
filedata = fread(datafile,'int16'); % data from USRP is stored in 16 bit integers

threshold = 100; %threshold to determine when signal is received. will need to figure out a way to set this
fs = 10e6; %sample rate of USRP capture (Hz)

%16 bit integers alternate between the real and imaginary parts
realdata = filedata(1:2:end);
imagdata = filedata(2:2:end);

data = realdata+1j*imagdata; %combine into one vector

%[~, loc] = max(abs(realdata));
%for some reason there is always a spike near the beginning
idxoffset = 20;
startidx = find(abs(data(idxoffset+1:end))>threshold,1,'first');
startidx = startidx - idxoffset;
%startidx = startidx(2);

%% Plot Received Signal
% figure(1)
% plot(real(data(loc-500000:loc+500000)))
% 
% figure(2)
% plot(imag(data(loc-500000:loc+500000)))
figure(21)
samples_to_plot = max(startidx-500000,1):startidx+500000;
plot(samples_to_plot,abs(data(samples_to_plot)))
grid on
xlabel("Sample Number")
ylabel("Amplitude")
title("Magnitude of Received Signal")

%% Plot real and imaginary parts of received signal near arrival time
figure(3)
samples_to_plot = startidx-5000:startidx+10000;
plot(samples_to_plot,realdata(samples_to_plot))
hold on
plot(samples_to_plot,imagdata(samples_to_plot))
hold off
grid on
xlabel("Sample Number")
ylabel("Amplitude")
title("Arrival of Signal")

%% Plot magnitude of received signal near arrival time
figure(31)
plot(samples_to_plot,abs(data(samples_to_plot)))
grid on
xlabel("Sample Number")
ylabel("Amplitude")
title("Magnitude of Signal Near Arrival")

%% Plot phase of signal near arrival time
figure(4)
plot(samples_to_plot,angle(data(samples_to_plot)))
grid on
xlabel("Sample Number")
ylabel("Phase")
title("Measured Phase Near Arrival of Signal")

%% Compare phase and magnitude of signal near arrival time
figure(5)
samples_to_plot = startidx-2000:startidx+1000;
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
title("Amplitude and Phase of Signal Arrival")


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
figure(6)
fftlen = 10000;
fs = 10e6;
startidx2 = max(startidx-5000,1);
samples_to_plot = startidx2:startidx2+fftlen-1;
f = (-1/2:1/fftlen:1/2-1/fftlen)*fs;
plot(f,20*log10(fftshift(abs(fft(data(samples_to_plot))))));
grid on
xlabel("Frequency (Hz)")
ylabel("Magnitude Squared")
title("FFT")

%% LPF
data_to_filter = data(samples_to_plot);
%[lpf_b,lpf_a] = cheby2(10,80,1e5/(fs/2));
lpf_b = firpm(1000,[0 50000 100000 fs/2]/(fs/2),[1 1 0 0]);
lpf_a = 1;

% filtered_data_real = filter(lpf_b,lpf_a,real(data_to_filter));
% filtered_data_imag = filter(lpf_b,lpf_a,imag(data_to_filter));
% filtered_data = filtered_data_real + 1j*filtered_data_imag;

filtered_data = filter(lpf_b,lpf_a,data_to_filter);

figure(7)
samples_to_plot = startidx:startidx+fftlen-1;
f = (-1/2:1/fftlen:1/2-1/fftlen)*fs;
plot(f,20*log10(fftshift(abs(fft(filtered_data)))));
grid on
xlabel("Frequency (Hz)")
ylabel("Magnitude Squared")
title("FFT")

figure(8)
plot(abs(filtered_data))
grid on
xlabel("Sample Number")
ylabel("Amplitude")
title("Magnitude of Filtered Signal Near Arrival")

%% Compare phase and magnitude of Filtered signal near arrival time
figure(9)
filtered_plot_offset = 500000;
samples_to_plot = filtered_plot_offset:filtered_plot_offset+3000;
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
title("Amplitude and Phase of Signal Arrival")
