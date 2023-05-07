Building Process

Go to BUILD folder:

cmake ../
make clean
make

Usage:
host: ./host <server IP> <PORT> <USRP IP>

host_controller: ./host_controller <server_IP> <PORT>

host_controller commands:
1) "config": configure USRP
2) "sync": Sunchronizes to PPS, assumption, PPS is available
3) "stream": streams and stores data to file
4) "stop": Currently NOP
5) "disconnect" : Close client.
