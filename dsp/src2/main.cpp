#include <iostream>
#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <boost/version.hpp>

#include "include/UsrpInitFuncs.h"
#include "include/UsrpRxStreamFuncs.h"

boost::atomic<bool> stop_streaming(false);

int main()
{
    std::cout << "Host Application Launched" << std::endl;
    std::cout << "Boost Version: " << BOOST_VERSION << std::endl;
    
    //usrp settings
    std::string ip = "192.168.11.2";
    std::string subdev = "A:0";
    std::string ant = "TX/RX";
    std::string clock_ref = "internal";
    std::string time_ref = "none";
    double sample_rate = 10e6;
    double center_freq = 174e6;
    double gain = 0;
    double bw = 20e6;
    //stream settings
    std::string cpu_fmt = "sc16";
    std::string wire_fmt = "sc16";

    uhd::usrp::multi_usrp::sptr usrp = gen_usrp(ip, subdev, ant, clock_ref, time_ref, sample_rate, center_freq, gain, bw);
    
    stream_rx_data(usrp);

    std::cout << "Host Application ended" << std::endl;

    return 0;
}

