import pyqtgraph as pg
import numpy as np
from PyQt5.QtWidgets import QApplication

def plot_complex_data(file_name):
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
# Usage example:
plot_complex_data("complex_vector.bin")
