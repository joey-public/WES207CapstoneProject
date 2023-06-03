import os
import numpy as np
import scipy as sp
import matplotlib.pyplot as plt

def read_short_bin_data(path:str)->np.array:
    data = np.fromfile(path, dtype=np.int16)
    print(data.size)
    data = np.reshape(data, (data.size//2, 2))
    return data[:,0] + 1j*data[:,1]

def read_float_bin_data(path:str)->np.array:
    data = np.fromfile(path, dtype=np.single)
    print(data.size)
    data = np.reshape(data, (data.size//2, 2))
    return data[:,0] + 1j*data[:,1]

def sim_dsp(file_path):
    print('Reading raw data...')
    fs = 25e6
    rx_data = read_short_bin_data(file_path)
    max_time = 1.0
    max_sz = int(fs * max_time)
    rx_data = rx_data[0:max_sz]
    t = np.arange(0, rx_data.size/fs,1/fs) 
#    idx0 = int(0.0* fs)
#    idx1 = int(2.0 * fs)
#    data = rx_data[idx0:idx1] #manually reduce the size to increase sim speed

    #processing steps

    #1. filter the data
    print("filtering the data with python...")
    #num_taps = 64 
    #fc = 0.1
    #taps = sp.signal.firwin(num_taps, fc)

    #using the exact FIRPM coefficients from Vinits Matlab sims
    taps = np.array(
           [0.000754, 0.001591, 0.003105, 0.005373, 
            0.008528, 0.012655, 0.017767, 0.023788, 
            0.030544, 0.037763, 0.045093, 0.052125, 
            0.058426, 0.063583, 0.067239, 0.069137, 
            0.069137, 0.067239, 0.063583, 0.058426, 
            0.052125, 0.045093, 0.037763, 0.030544, 
            0.023788, 0.017767, 0.012655, 0.008528, 
            0.005373, 0.003105, 0.001591, 0.000754])

    real = sp.signal.lfilter(taps, 1.0, np.real(rx_data))
    imag = sp.signal.lfilter(taps, 1.0, np.imag(rx_data))
    processed_rx_data = real + 1j*imag
    del real 
    del imag

    #2. normalize the filtered data #note in cpp this would need to be a double data type
    print('Normalizing the filtered data...')
    processed_rx_data = processed_rx_data / np.max(np.abs(processed_rx_data))

    #3. detect a threshold
    print('Detecting threshold...')
    thresh = 0.8
    threshold_idx = np.argwhere(np.abs(processed_rx_data)>thresh)
    threshold_idx = threshold_idx[0][0]

    #4. store 30ms of the unfilted data starting 5ms before the threshold
    print('Storing pulse data...')
    thresh_time = threshold_idx / fs
    start_time = thresh_time - 0.005
    end_time = start_time + 30e-3
    start_idx =int(start_time * fs)
    end_idx = int(end_time * fs)

    print('Plotting raw data...')
    plt.figure(1)
    plt.clf()
    plt.subplot(311)
    plt.title('Raw Waveform Data')
    plt.plot(np.abs(rx_data))
    plt.subplot(312)
    plt.title('Filtered and Normailzed data')
    plt.plot(np.abs(processed_rx_data))
    plt.text(1000,0.5, 'Thresh idx: = {}'.format(threshold_idx))
    plt.vlines([start_idx, threshold_idx, end_idx], 0, 1, colors = ['r', 'k', 'r'], linestyles = '--', label='dsp results')
    plt.subplot(313)
    plt.title('Pulse to send to Host Controller')
    plt.plot(np.abs(rx_data[start_idx : end_idx]))
    plt.tight_layout()
    img_path = './images/raw_data.jpeg'
    plt.savefig(img_path)


    plt.figure(2)
    plt.plot(np.abs(processed_rx_data), label='Py Results')
    print('Reading cpp data...')
    cpp_proc_data = read_float_bin_data('./proc_data.bin')
    print('Plotting cpp data...')
    plt.plot(np.abs(cpp_proc_data), label = 'CPP results')
    plt.legend(loc='upper right')
    img_path = './images/processing_compare.jpeg'
    plt.savefig(img_path)

    print('Plotting spectrum of raw data...')
    def my_fft(data):
         return 20*np.log10(np.abs(np.fft.fftshift(np.fft.fft(data))))

    plt.figure(3)
    plt.subplot(311)
    rx_spectrum = my_fft(rx_data)
    freqs = fs * np.arange(-0.5, 0.5, 1/rx_spectrum.size)
    plt.plot(freqs, rx_spectrum)
    plt.title('Raw Data Spectrum')
    del rx_spectrum
    del rx_data
    plt.subplot(312)
    proc_spectrum = my_fft(processed_rx_data)
    freqs = fs * np.arange(-0.5, 0.5, 1/proc_spectrum.size)
    plt.plot(freqs, proc_spectrum)
    plt.title('Python Processed Spectrum')
    del proc_spectrum
    del processed_rx_data
    plt.subplot(313)
    proc_cpp_spectrum = my_fft(cpp_proc_data)
    freqs = fs * np.arange(-0.5, 0.5, 1/proc_cpp_spectrum.size)
    plt.plot(freqs, proc_cpp_spectrum)
    plt.title('Cpp Processed Spectrum')
    plt.tight_layout()
    del proc_cpp_spectrum
    del cpp_proc_data
    img_path = './images/spectrum_comparison.jpeg'
    plt.savefig(img_path)

    
    
sim_dsp('./raw_data.bin')

#data_dir = './20230527_test_data/'
#max_files = 1 
#for filename in os.listdir("20230527_test_data"):
#    if max_files == 0: break
#    print('simulating: ' + filename + ' ...')
#    sim_dsp(data_dir + filename)
#    max_files -= 1
