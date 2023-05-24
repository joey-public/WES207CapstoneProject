import os
import sys
import numpy as np
import pyqtgraph as pg
from PyQt5.QtWidgets import QApplication

def calc_downsampling_factor(file_path):
    file_size = os.path.getsize(file_path)
    threshold = 5*1024*1024; #5Mb
    downsampling_factor = (file_size // (threshold)) + 1
    downsampling_factor = max(downsampling_factor, 1)
    print('File Size is {}, data in plots will be downsampled by {}'.format(file_size, downsampling_factor))
    return downsampling_factor

def read_bin_data(file_name, downsampling_factor):
    data = np.fromfile(file_name, dtype=np.int16)
    num_complex = len(data) // 2
    if downsampling_factor > 1:
        downsampled_data = data[::downsampling_factor]
        num_complex = len(downsampled_data) // 2
        data = downsampled_data
    reshaped_data = np.reshape(data, (num_complex, 2))
    return reshaped_data

def plot_complex_data(file_name):
    downsampling_factor = calc_downsampling_factor(file_name)
    data = read_bin_data(file_name, downsampling_factor)
    real_part = data[:, 0]
    imaginary_part = data[:, 1]
    complex_data = real_part + 1j * imaginary_part

    app = QApplication([])
    # Create Figure 1: Real and Imaginary data
    win1 = pg.GraphicsLayoutWidget()
    win1.setWindowTitle('Real and Imaginary Data')
    p1 = win1.addPlot(title='Real Part')
    p1.plot(real_part, pen='r')
    p2 = win1.addPlot(title='Imaginary Part')
    p2.plot(imaginary_part, pen='b')
    win1.show()

    # Calculate magnitude and phase
    magnitude = np.abs(complex_data)
    phase = np.angle(complex_data)

    # Create Figure 2: Magnitude and Phase
    win2 = pg.GraphicsLayoutWidget()
    win2.setWindowTitle('Magnitude and Phase')
    p3 = win2.addPlot(title='Magnitude')
    p3.plot(magnitude, pen='g')
    p4 = win2.addPlot(title='Phase')
    p4.plot(phase, pen='m')
    win2.show()

    # Start the PyQtGraph application event loop
    QApplication.instance().exec_()

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Usage: python script.py <data_file>") 
        sys.exit(1)
    data_file = sys.argv[1]
    plot_complex_data(data_file)
