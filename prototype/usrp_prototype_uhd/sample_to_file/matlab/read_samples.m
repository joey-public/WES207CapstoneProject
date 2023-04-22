clc
clear all
close all

a = 10;
fc = 173.9353e6;
fs = 3*fc;
ton = 15e-3;
toff = 1-ton;

%sample signal
% t = 0:1/fs:15e-3;
% pulse = a*sin(2*pi*fc*t)+1j*a*cos(2*pi*fc*t);
% figure;
% plot(1:1000, real(pulse(1:1000)));
% 
% %create 1sec worth of data
% tx = [zeros(1, length(0:1/fs:toff)) pulse];
% 
% plot(1:length(tx), real(tx));

% File path
file_path = 'usrp_samples_0dB.dat';

% Chunk size in bytes
chunk_size = 1024 * 1024; % 1MB


% Read data from file in chunks
file_fid = fopen(file_path, 'rb'); % Open file in read mode

% File size in bytes
fseek(file_fid, 0, 'eof');
file_size = ftell(file_fid);
fseek(file_fid, 0, -1);

% Specify the number of samples in the file
num_samples = file_size / 4; % Divide by 4 since each sample is 2 bytes (short) for I and 2 bytes for Q

% Open the file for reading
fid = fopen(file_path, 'rb');

% Read the binary file into a complex array
data = fread(fid, [2, num_samples], 'int16'); % Read as 16-bit signed integers

% Close the file
fclose(fid);

% Convert the interleaved I/Q samples into a complex array
received_signal = complex(data(1,:), data(2,:));

received_signal = received_signal()

% Plot the received signal
figure;
plot(real(received_signal));
grid on;
hold on; 
plot(imag(received_signal));
grid on
xlabel('Sample Index');
ylabel('Amplitude');
title('Received Signal');
xlim([0.4122e7 0.4124e7])
legend("Real", "Imag")