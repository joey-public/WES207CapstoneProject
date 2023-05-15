import uhd
import numpy as np
import matplotlib.pyplot as plt
import decode_usrp_data
import helper_funcs as hf
import sys, os

#get the usrp device
usrp = uhd.usrp.MultiUSRP()

#settings
settings = {
    "center_freq": 100e6,
    "sample_rate": 1e6,
    "gain" : 10,
    "clock_source": 0,
    "time_source": 0 
}

sample_time = 1 #seconds

hf.setup_usrp_device(usrp, settings)
num_samps = hf.calc_num_samples(sample_time, usrp)

print(num_samps)

samples = usrp.recv_num_samps(num_samps, 
                              settings['center_freq'],
                              settings['sample_rate'], 
                              [0],
                              settings['gain'], )

print(samples[0:30])
