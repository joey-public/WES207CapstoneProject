% This script makes various plots of the received signal to determine its
% characteristics

% read data from file
datafile = fopen('0421_test_usrp_samples_0dB.dat'); % filename of USRP capture File
filedata = fread(datafile,'int16'); % data from USRP is stored in 16 bit integers

threshold = 200; %threshold to determine when signal is received. will need to figure out a way to set this
fs = 10e6; %sample rate of USRP capture (Hz)

%16 bit integers alternate between the real and imaginary parts
realdata = filedata(1:2:end);
imagdata = filedata(2:2:end);

data = realdata+1j*imagdata; %combine into one vector

%[~, loc] = max(abs(realdata));
startidx = find(realdata>200,1,'first');

%% Plot Received Signal
% figure(1)
% plot(real(data(loc-500000:loc+500000)))
% 
% figure(2)
% plot(imag(data(loc-500000:loc+500000)))
figure(21)
samples_to_plot = startidx-500000:startidx+500000;
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
[pks, locs] = findpeaks(abs(data),'MinPeakHeight',threshold,'MinPeakDistance',fs/10);

figure(6)
bar(locs/fs,pks)
grid on
xlabel("Time (s)")
ylabel("Max Magnitude of Transmission")
title("Magnitude Comparison of Signal Captures over time")

% fignum = 100;
% for k = 1:length(locs)
%     if(locs(k) > 500000)
%         figure(fignum+k)
%         samples_to_plot = locs(k)-500000:locs(k)+500000;
%         plot(samples_to_plot,abs(data(samples_to_plot)))
%     end
% end

