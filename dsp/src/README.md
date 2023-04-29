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
A[Initialize USRP] --> B(Configure USRP)
B --> C(Start Threads)
C --> D(Wait for 'ESC' keypress)
D --> E(Stop streaming)
E --> F(Release USRP resources)
C --> G(Start streaming)
C --> H(Processing thread)
C --> I(Networking thread)
D --> J(Wait for threads to join)
G --> J
H --> J
I --> J
```

