import uhd
import numpy as np
import matplotlib.pyplot as plt
import decode_usrp_data

#constants
INTERNAL, EXTERNAL, GPSDO = 0,1,2

#get the usrp device
usrp = uhd.usrp.MultiUSRP()

#general settings
num_samps = 10000 # number of samples received
#note Bandwidth = center_freq +/- (sample_rate/2)
center_freq = 100e6 # Hz
sample_rate = 1e6 # Hz
gain = 10 # dB

usrp.set_rx_rate(sample_rate, 0)
usrp.set_rx_freq(uhd.libpyuhd.types.tune_request(center_freq), 0)
usrp.set_rx_gain(gain, 0)

#clock and timing settings
def config_timing(device:uhd.usrp.MultiUSRP, source:int)->None:
    if source == EXTERNAL:
        arg = 'external'
    elif source == GPSDO:
        arg == 'gpsdo'
    else:
        return 
    device.set_clock_source(arg)
    device.set_time_source(arg)

config_timing(usrp, INTERNAL)


# Set up the stream and receive buffer
st_args = uhd.usrp.StreamArgs("fc32", "sc16")
st_args.channels = [0]
metadata = uhd.types.RXMetadata()
streamer = usrp.get_rx_stream(st_args)
recv_buffer = np.zeros((1, 1000), dtype=np.complex64)

# Start Stream
stream_cmd = uhd.types.StreamCMD(uhd.types.StreamMode.start_cont)
stream_cmd.stream_now = True
streamer.issue_stream_cmd(stream_cmd)

# Receive Samples
samples = np.zeros(num_samps, dtype=np.complex64)
for i in range(num_samps//1000):
    streamer.recv(recv_buffer, metadata)
    samples[i*1000:(i+1)*1000] = recv_buffer[0]

# Stop Stream
stream_cmd = uhd.types.StreamCMD(uhd.types.StreamMode.stop_cont)
streamer.issue_stream_cmd(stream_cmd)

print(len(samples))
print(samples[0:10])
plt.plot(samples)
plt.show()
