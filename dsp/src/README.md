# DSP source code

Here I will test dsp algorithms using the UHD c++ api. 

note I also use the gnuplot library to help me visualize what I am doing. 

to install gnuplot use these commands:

## Host Application Flowchart
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
D --> F(Streaming thread)
D --> G(Processing thread)
E --> I(Wait for threads to join)
F --> I
G --> I
I --> J(send data packet to host controller)
J --> B
```

## Data Processing Thread Flowchart

```mermaid
graph TD
A[Start Data Processing Thread] --> B(Recieve Magnitude Data from Stream Thread)
B --> C{Data Above Threshold?}
C -- No --> B
C -- Yes --> D[Dump N samples into Processing Buffer]
D --> E(Process Data)
E --> F(Save Results to packet)
```
style C fill:#fff,stroke:#000,stroke-width:2px,scale:0.8;

