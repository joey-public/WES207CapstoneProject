Building Process

Go to BUILD folder:

cmake ../
make clean
make

Usage:
host: ./host <server IP> <PORT> <USRP IP>

host_controller: ./host_controller

host_controller commands:
1) "configure_usrp": configure USRP
2) "synchronize_pps": Sunchronizes to PPS, assumption, PPS is available
3) "start_streaming": streams and stores data to file
4) "stop_streaming": Currently NOP
5) "disconnect" : Close client.
