import numpy as np
import matplotlib.pyplot as plt

# define the path of the binary file
file_path = './data/20230429_dist_no_agc.dat' 

#read data from binary file into an array of type int16 
def read_binary_file_int16(file_path, num_chunks = -1):
    # Open the binary file for reading
    with open(file_path, 'rb') as f:
        #read the whole file or just the passed num_chunks
        if num_chunks == -1: 
            binary_data = f.read()  
        else: 
            binary_data = f.read(num_chunks * 2)     
        # Convert the binary data to a NumPy array of int16s
        np_array = np.frombuffer(binary_data, dtype=np.int16)    
    return np_array

#convert the int16 array into a single complex array
def convert_to_complex(int16_array):
    # Reshape the int16 array into a 2D array with shape (N/2, 2)
    reshaped_array = int16_array.reshape((-1, 2))
    # Convert the reshaped array to a complex dtype NumPy array
    complex_array = reshaped_array[:, 0] + 1j * reshaped_array[:, 1]
    return complex_array

#helper function for printing raw data
def print_int16_array_as_binary(int16_array, num_elements_to_print):
    # Loop through the specified number of elements in the array
    for i in range(num_elements_to_print):
        # Get the i-th element in the array and convert it to binary
        binary_str = format(int16_array[i], '016b')
        # Print the binary string on a new line
        print(binary_str)
        
def get_test_data(file_path, num_chunks=-1):
    int16_array = read_binary_file_int16(file_path, num_chunks)
    return convert_to_complex(int16_array)

signal = get_test_data(file_path)

plt.plot(np.real(signal), label='real')
plt.plot(np.imag(signal), label='imag')
plt.title("Tx Sample Data")
plt.xlabel("Sample Number")
plt.ylabel("Amplitude")
plt.legend()
plt.save_fig('./data/images/{}.jpeg'.format(name))

plt.show()
