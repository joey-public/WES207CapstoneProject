# DSP source code

Here I will test dsp algorithms using the UHD c++ api. 

note I also use the gnuplot library to help me visualize what I am doing. 

to install gnuplot use these commands:

```
sudo apt-get install gnuplot
sudo apt-get install libgnuplot-iostream-dev
```
 then you can use gnuplot in your c++ programs by incliding it:
 ```
 #include <gnuplot-iostream.h>
 ```

```mermaid
graph TD
A[Initialize USRP] --> B(wait for start message from host controller)
B --> C(synch to GPS)
C --> D(Start Threads)
D --> E(Wait for 'ESC' keypress)
D --> F(Start streaming)
D --> G(Processing thread)
D --> I(Wait for threads to join)
G --> I
H --> I
I --> J(send data packet to host controller)
```

