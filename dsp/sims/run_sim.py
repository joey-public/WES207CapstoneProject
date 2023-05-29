import os
import numpy as np
import scipy as sp
import matplotlib.pyplot as plt

def read_bin_data(path:str)->np.array:
    data = np.fromfile(path, dtype=np.int16)
    data = np.reshape(data, (data.size//2, 2))
    return data[:,0] + 1j*data[:,1]

def process_data(raw_data):
    #Step 1. Filter the Data
    num_taps = 64
    fc = 0.1
    taps = sp.signal.firwin(num_taps, fc)
    real = sp.signal.lfilter(taps, 1.0, np.real(raw_data))
    imag = sp.signal.lfilter(taps, 1.0, np.imag(raw_data))
    processed_data = real + 1j*imag

def sim_dsp(file_path):
    rx_data = read_bin_data(file_path)
    fs = 25e6
    t = np.arange(0, rx_data.size/fs,1/fs) 
    idx0 = int(0.0* fs)
    idx1 = int(2.0 * fs)
    #processing steps
    data = rx_data[idx0:idx1] #manually reduce the size to increase sim speed
    #1. filter the data
    num_taps = 64 
    fc = 0.1
    taps = sp.signal.firwin(num_taps, fc)
    real = sp.signal.lfilter(taps, 1.0, np.real(data))
    imag = sp.signal.lfilter(taps, 1.0, np.imag(data))
    processed_rx_data = real + 1j*imag
    #2. normalize the filtered data #note in cpp this would need to be a double data type
    processed_rx_data = processed_rx_data / np.max(processed_rx_data)
    #3. detect a threshold
    thresh = 0.5
    threshold_idx = np.argwhere(processed_rx_data>thresh)
    threshold_idx = idx0 + threshold_idx[0][0]
    #4. store 30ms of the unfilted data starting 5ms before the threshold
    thresh_time = threshold_idx / fs

    start_time = thresh_time - 0.005

    end_time = start_time + 30e-3
    start_idx =int(start_time * fs)
    end_idx = int(end_time * fs)

    plt.figure(1)
    plt.clf()
    plt.subplot(311)
    plt.title('Raw Waveform Data')
    plt.plot(np.abs(rx_data[idx0:idx1]))
    plt.subplot(312)
    plt.title('Filtered and Normailzed data')
    plt.plot(np.abs(processed_rx_data))
    plt.vlines([start_idx-idx0, threshold_idx-idx0, end_idx-idx0], 0, 1, colors = ['r', 'k', 'r'], linestyles = '--', label='dsp results')
    plt.subplot(313)
    plt.title('Pulse to send to Host Controller')
    plt.plot(np.abs(rx_data[start_idx : end_idx]))
    plt.tight_layout()
    img_path = './images' + file_path[20:-4] + '_pulse.jpeg'
    del rx_data
    del processed_rx_data
    plt.savefig(img_path)
    

data_dir = './20230527_test_data/'
max_files = 1 
for filename in os.listdir("20230527_test_data"):
    if max_files == 0: break
    print('simulating: ' + filename + ' ...')
    sim_dsp(data_dir + filename)
    max_files -= 1
