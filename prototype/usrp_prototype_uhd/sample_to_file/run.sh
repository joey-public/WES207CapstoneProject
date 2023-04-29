./build/rx_usrp --args addr=192.168.11.2 --duration 3 --rate 10000000 --freq 173935300 --gain 0 --file test.dat
python plot_data.py
