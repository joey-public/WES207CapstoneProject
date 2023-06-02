## Building Process

Go to BUILD folder:
```
cmake ../
make clean
make
```
## Host Controller

### Usage

```
host_controller: ./host_controller <server_IP> <PORT>
```

### Host Controller Commands

After running the host controller you should wait for all the hosts to connect. Once all hosts connect to the system you should see the following statement in your terminal:

Then you can move onto using the different host_controller commands to control the syte. The commands are run in the following order
1) `config`: configure USRP
2) `sync`: Sunchronizes to PPS, assumption, PPS is available
3) `stream`: streams and stores data to file
4) `stop`: Currently NOP
5) `disconnect` : Close client.

## Host

### Usage

For full system testing with several USRPs and a host controller use:
  
```
host: ./host <server IP> <PORT> <USRP IP>
```
  
If you want to test with a single USRP without any networking. This can be useful for collecting data for postprocessing to test differnet dsp algorithms. 

```
host: ./host
```
### Host Settings

Before running the host you can configure the settings of the usrp and processing, by modifying the  `main/settings.txt`. The available settings are listed below. 

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

The raw data file saves the raw data directly collected fromt he usrp. size will be rx_stream_time * usrp_sample_rate * sizeof(rx_stream_wire_fmt). We recommend using sc16 which uses 32 bits to represent a single complex number. 

The pulse data file size will be proc_pulse_save_time * usrp_sample_rate * sizeof(rx_stream_wire_fmt)

The proc data stores the results of the signal processing done withing the cpp applicaiton. This can be usefule for verification when testing new/differnt dsp algorithms for pulse detection. The proc_data file size will be rx_stream_time * usrp_sample_rate * sizeof(std::complex<float>). Note is takes 64 bits for a complex float so the proc_data will be 2 times the size of the raw data if you decide to save it.  




