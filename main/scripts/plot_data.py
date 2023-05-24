import sys
import numpy as np
import matplotlib.pyplot as plt
from PyQt5.QtWidgets import QApplication
import pyqtgraph as pg

def read_complex_bin_file():
    pass
def read_real_bin_file():
    pass
def read_complex_txt_file():
    pass
def read_real_txt_file():
    pass

def plot_complex_data_qt(file_name):
    # Read the binary file as int16 data
    data = np.fromfile(file_name, dtype=np.int16)
    # Calculate the number of complex values
    num_complex = len(data) // 2
    # Reshape the data to separate real and imaginary parts
    reshaped_data = np.reshape(data, (num_complex, 2))
    # Separate real and imaginary parts
    real_part = reshaped_data[:, 0]
    imaginary_part = reshaped_data[:, 1]
    # Create complex array from real and imaginary parts
    complex_data = real_part + 1j * imaginary_part
    # Create Figure 1: Real and Imaginary data
    app = QApplication([])
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

def plot_complex_data_mpl(data_file):
    # Read the data from the text file
    data = np.genfromtxt(data_file, delimiter=' ', dtype=np.complex128)
    data = data.reshape((-1,2))
    data = data[:, 0] + 1j * data[:,1]
    # Split the data into real, imaginary, and magnitude components
    real_data = np.real(data)
    imaginary_data = np.imag(data)
    magnitude_data = np.abs(data)
    # Create the figure and subplots
    fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(8, 10))
    # Plot the real data
    ax1.plot(real_data, label='Real')
    ax1.set_ylabel('Amplitude')
    ax1.set_title('Real Data')
    # Plot the imaginary data
    ax2.plot(imaginary_data, label='Imaginary')
    ax2.set_ylabel('Amplitude')
    ax2.set_title('Imaginary Data')
    # Plot the magnitude data
    ax3.plot(magnitude_data, label='Magnitude')
    ax3.set_xlabel('Sample')
    ax3.set_ylabel('Amplitude')
    ax3.set_title('Magnitude Data')
    # Add legends and adjust subplot spacing
    plt.tight_layout()
    plt.show()

def plot_real_data_mpl(data_file):
    # Read the data from the text file
    data = np.genfromtxt(data_file, delimiter=' ', dtype=float)
    plt.plot(data)
    # Add legends and adjust subplot spacing
    plt.tight_layout()
    plt.show()


if __name__ == '__main__':
    # Check if the data file path is provided as a command line argument
    print(len(sys.argv))
    if len(sys.argv) != 4:
        print("Usage: python script.py <data_file> <data_type> <plot library(mpl or qt)>")
        sys.exit(1)
    # Get the data file path from the command line argument
    data_file = sys.argv[1]
    data_type = sys.argv[2] #real or complex
    plot_libr = sys.argv[3] #mpl or qt

    if data_type == 'real':
        complex_data = False;
    elif data_type == 'complex':
        complex_data = True;
    else:
        print('2nd arg should be real or complex, not: {}'.format(data_type))
        sys.exit(1)

    if plot_libr == 'mpl':
        use_qt = False
    elif plot_libr == 'qt':
        use_qt = True
    else:
        print('3rd argument should be mpl or qt, not: {}'.format(plot_libr))
        sys.exit(1)

    # Call the plot_data function with the data file
    if use_qt:
        if complex_data:
            plot_complex_data_qt(data_file)
        else:
            print("qt plotting of real data is not supported using mpl")
            plot_real_data_mpl(data_file)
    else:
        if complex_data:
            plot_complex_data_mpl(data_file)
        else:
            plot_real_data_mpl(data_file)

