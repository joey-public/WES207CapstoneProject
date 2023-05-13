import sys
import numpy as np
import matplotlib.pyplot as plt

def plot_data(data_file):
    # Read the data from the text file
    data = np.genfromtxt(data_file, delimiter=',', dtype=np.complex128)

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

if __name__ == '__main__':
    # Check if the data file path is provided as a command line argument
    if len(sys.argv) != 2:
        print("Usage: python script.py <data_file>")
        sys.exit(1)

    # Get the data file path from the command line argument
    data_file = sys.argv[1]

    # Call the plot_data function with the data file
    plot_data(data_file)

