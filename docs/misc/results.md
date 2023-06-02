```
ontroller 100.81.35.20 33334
Server listening on port 33334
accept_connection
accept_connection exit
Client connected: 0
accept_connection
accept_connection exit
Client connected: 1
Max num client connected
config
Broadcasting control command:config to all clients
sync
Broadcasting control command:sync to all clients
stream
Broadcasting control command:stream to all clients
send
Send control command: send to client: 0
Receive header packet from client: 0
Receive data packet from client: 0
Recieved Data Packet: 
	rx_id: 0
	long: -11.2
	lat: -10.2
	alt: 10
	Num Time Stamps: 1
	peak timestamps: 0.010815
	waveform samples len: 19999
	Saving Pulse data to txt file
Vector saved to file: ./pulse_data.txt
Send control command: send to client: 1
Receive header packet from client: 1
Receive data packet from client: 1
Recieved Data Packet: 
	rx_id: 1
	long: -11.2
	lat: -10.2
	alt: 10
	Num Time Stamps: 1
	peak timestamps: 9e-06
	waveform samples len: 19999
	Saving Pulse data to txt file
Vector saved to file: ./pulse_data.txt
Localization Thread started processing:
Starting localization. Inputs: 
0.010815
9e-06
0.010815
9e-06
Estimated Location: -1.61973e+06
          50
           7
stream  
Broadcasting control command:stream to all clients
send
Send control command: send to client: 0
Receive header packet from client: 0
Receive data packet from client: 0
Recieved Data Packet: 
	rx_id: 0
	long: -11.2
	lat: -10.2
	alt: 10
	Num Time Stamps: 1
	peak timestamps: 0.465796
	waveform samples len: 19999
	Saving Pulse data to txt file
Vector saved to file: ./pulse_data.txt
Send control command: send to client: 1
Receive header packet from client: 1
Receive data packet from client: 1
Recieved Data Packet: 
	rx_id: 1
	long: -11.2
	lat: -10.2
	alt: 10
	Num Time Stamps: 1
	peak timestamps: 0.362653
	waveform samples len: 19999
	Saving Pulse data to txt file
Vector saved to file: ./pulse_data.txt
Localization Thread started processing:
Starting localization. Inputs: 
0.465796
0.362653
0.465796
0.362653
Estimated Location: -1.54609e+07
          50
           7

```

```
Using Device: Single USRP:
  Device: USRP2 / N-Series Device
  Mboard 0: N210r4
  RX Channel: 0
    RX DSP: 0
    RX Dboard: A
    RX Subdev: WBXv3 RX+GDB
  TX Channel: 0
    TX DSP: 0
    TX Dboard: A
    TX Subdev: WBXv3 TX+GDB

external
external
Actual RX Freq: 173.935300 MHz...

Actual RX Rate: 1.000000 Msps...

Actual RX Gain: 0.000000 dB...

Actual RX Bandwidth: 1.000000 MHz...

USRP configured.
Synchronizing to PPS...
[INFO] [MULTI_USRP]     1) catch time transition at pps edge
[INFO] [MULTI_USRP]     2) set times next pps (synchronously)

Success!

Product requires verification of ref_locked sensor!
Checking ref_locked sensor...PPS sync PASS!
Current USRP Time frac sec: 0.00412121
Current USRP Time full sec: 1
Current USRP Time Real sec: 1.00412
Current USRP Time Tick count: 0
Synchronized to PPS.
-------------------------------------
Start Streaming Data...
	Collecting 1000000 samples
	cpu_fmt = sc16
	wire_fmt = sc16
	RX Buff Size: 363
	pkt dt = 363 samples / 1.000000 Mhz = 363.000000 us
Stop Streaming Data...
-------------------------------------
Analyzing Raw Data...
	Collected 1s of raw data at fs = 1e+06
	Buffer length: 1000000
	Buffer takes: 4 Mb of memory
	Saving Raw data to txt file
Vector saved to file: ./raw_data.txt
Done Analyzing Raw Data...
-------------------------------------
Start Processing Data...
	Takeing the magnitude...
	Mag Data takes: 4 Mb of memory
	Saving Mag data to txt file...
Vector saved to file: ./mag_data.txt
	Doing threshold detection...
	Pulse Detected starting at index: 10815
	Pulse Data takes: 0.079996 Mb of memory
	Pulse Data size: 19999
	Saving Pulse data to txt file
Vector saved to file: ./pulse_data.txt
Stop Processing Data...
-------------------------------------
Streaming Done!
	Header packet created
Creating data packet
	Adding lat
	Adding long
Header packet length = 80044
Send operation completed successfully
-------------------------------------
Start Streaming Data...
	Collecting 1000000 samples
	cpu_fmt = sc16
	wire_fmt = sc16
	RX Buff Size: 363
	pkt dt = 363 samples / 1.000000 Mhz = 363.000000 us
Stop Streaming Data...
-------------------------------------
Analyzing Raw Data...
	Collected 1s of raw data at fs = 1e+06
	Buffer length: 1000000
	Buffer takes: 4 Mb of memory
	Saving Raw data to txt file
Vector saved to file: ./raw_data.txt
Done Analyzing Raw Data...
-------------------------------------
Start Processing Data...
	Takeing the magnitude...
	Mag Data takes: 4 Mb of memory
	Saving Mag data to txt file...
Vector saved to file: ./mag_data.txt
	Doing threshold detection...
	Pulse Detected starting at index: 465796
	Pulse Data takes: 0.079996 Mb of memory
	Pulse Data size: 19999
	Saving Pulse data to txt file
Vector saved to file: ./pulse_data.txt
Stop Processing Data...
-------------------------------------
Streaming Done!
	Header packet created
Creating data packet
	Adding lat
	Adding long
Header packet length = 80044
Send operation completed successfully
```
