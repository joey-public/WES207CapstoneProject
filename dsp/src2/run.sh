#!/bin/bash
make clean
cmake ../ -DEigen3_DIR=$HOME/mypackages/
make 
./main
