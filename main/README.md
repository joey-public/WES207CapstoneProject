## Building Process

Go to BUILD folder:
```
mkdir build
cd build
cmake ../
make clean
make
```
## Host Controller
Note: The Boost version on which io-context works is 1.71. io-service has been deprecated. So, the host/hostcontroller code has been tested on Ubuntu 18.04 and Ubunutu 20.04, make sure,to have boost version 1.71 or above.
### Usage

```
./host_controller <server_IP> <PORT>
```

### Host Controller Commands
Note: Host controller uses async broadcast to send control messages but sequentially reads data from all hosts. 

After running the host controller you should wait for all the hosts to connect. Once all hosts connect to the system you should see the following statement in your terminal:

Once all clients are connected host controller issues following commands
1) `config`: configure USRP
2) `sync`: Synchronizes to GPS, assumption, GPS lock is available
3) `time`: See host and compare all timing.
3) `stream`: streams and stores data to file
4) `send`:  accumulates data from all host
5) `disconnect` : (internal command) Close client.
The console would ask at the end whether to continue with localization or not, if entered no, disconnect call is made.

### Limitation: 
1) Currently host_controller uses timing sleep. The code present in common folder has support to create/parse the control message response from host. Host controller need more coding to enable ack/nack read after each control message command and move on to next control message but this has not been coded. Reason: Timing constraints and major priority was to enable host send all data to controller for localization thread

## Host

### Usage

For full system testing with several USRPs and a host controller use:
  
```
./host <server IP> <PORT> 
```
  
If you want to test with a single USRP without any networking: 
```
./host
```
This can be useful for collecting data for postprocessing to test differnet dsp algorithms. 

### Host Settings

Before running the host you can configure the settings of the usrp and processing, by modifying the  `main/settings.txt`. 

- `usrp` settings denote settings related to the configureation of the usrp device.  
- `rx_stream` settings denote settings related to the streaming of data
- `proc` settings denote settings related to the signal processing
- `hostc` settings denote settings related to the host controller, but right now there is just one setting and it is not functional. 
- `save` settings are used to denote what data you want to save and to what path.  

An example of settings.txt with all the supported settings is listed below. 

```
usrp_ip: 192.168.11.2
usrp_subdev: A:A
usrp_ant: TX/RX
usrp_clock_ref: gpsdo
usrp_tim_ref: gpsdo
usrp_sample_rate: 25000000
usrp_center_freq: 173935300
usrp_gain: 10
usrp_bw: 10000000
rx_stream_time: 2
rx_stream_cpu_fmt: sc16
rx_stream_wire_fmt: sc16
proc_threshold: 0.8
proc_offset_samples: 1000
proc_pulse_save_time: 0.03
save_raw_data: true
raw_data_path: ./raw_data.bin
save_proc_data: true
proc_data_path: ./proc_data.bin
save_pulse_data: true
pulse_data_path: ./pulse_data.bin
hostc_max_clients: 1
```

### Notes on Data Collection

Be careful based on the sample rate and stream time you select the raw data and processing data can be very large.

The raw data file saves the raw data directly collected fromt he usrp. The file saved at `raw_data_path` will have the following size:

`rx_stream_time * usrp_sample_rate * sizeof(rx_stream_wire_fmt)`. 

We recommend using sc16 foe the `rx_stream_wire_fmt` which uses 32 bits to represent a single complex number. 

The proc data file saves the results of the fir filter. This can be useful for verification when testing new/differnt dsp algorithms for pulse detection. The file will be saved to `proc_data_path` with the following size:

`rx_stream_time * usrp_sample_rate * sizeof(std::complex<float>)`

The pulse data file saves the pulse data that gets sent to the host controller. It is saved to the file at `pulse_data_path` and has the following size:

`proc_pulse_save_time * usrp_sample_rate * sizeof(rx_stream_wire_fmt)`

The proc data stores the results of the signal processing done withing the cpp applicaiton. The proc_data file size will be. Note is takes 64 bits for a complex float so the proc_data will be 2 times the size of the raw data if you decide to save it.  

## Common

Common folder contains PacketUtils. This class helps in creating/parsing header packets, dataPackets, Control message Header/response. Main.cpp placed in the folder tests all functions.


