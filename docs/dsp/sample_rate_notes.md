# Notes on Sample Rate and TOA Accuracy

Consider a system with two receivers and a single transmitter as seen below. The receivers have some sampling rate $f_s$ and are distances $d_1$ and $d_2$ away from the transmitter. We define time $t=0s$ as the exact instant that the transmitter turns on, $t=t_1$ as the instant the signal reaches receiver 1, and $t=t_2$ as the time that the signal reaches receiver 2.  

Because each receiver is sampling at a rate of $f_s$ they can only measure arrival times in integer multiples of the sampling rate. Then $t_1 = n_1 / f_s$ and $t_2 = n_2 / f_s$ where $n_1$ and $n_2$ are positive integers. This means that the TDOA  measurement must also be some integer multiple of the sampling time $TDOA = \left| \frac{n_2-n_1}{f_s} \right|$. 

The speed of light is $c=3\times 10^8$ m/s. Then the transmit signal can be expected to travel a distance of $1$ meter in $\Delta t=\frac{1}{3\times 10^8}\frac{m}{m/s} \approx 3.33$ ns. The https://kb.ettus.com/About_USRP_Bandwidths_and_Sampling_Rates sample rate of the USRP radios is 100-200 MHz which corresponds to a sampling time of  $5$ to $10$ ns. *The accuracy of the timestamps taken by each receiver is limited by the sampling rate of the receiver.* We expect our time-stamps to be accurate with a range of  $5$ to $10$ ns. 

![[image1.jpg]]

Then we can define the time difference of arrival (TDOA) as
$$TDOA = |t_2-t_1|$$
$$ TDOA = \left|\frac{d_2}{c}-\frac{d_1}{c}\right| $$
$$TDOA = \left|\frac{d_2-d_1}{c}\right|$$
Where the value of $d_2-d_1$ is commonly referred to as the Range Difference (RD) in literature. 



