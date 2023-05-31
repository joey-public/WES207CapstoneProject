import numpy as np
import scipy as sp

num_taps = 127 
fc = 0.1
taps = sp.signal.firwin(num_taps, fc)
np.savetxt('taps.txt', 
            taps, delimiter=' ', 
            newline=',', 
            header='std::vecotr<float> fir_coeffs = { ', 
            footer = ' }\n', 
            comments = '',
            fmt='%f')

