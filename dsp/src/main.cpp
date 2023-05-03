#include <iostream>
#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <boost/version.hpp>

#include "include/UsrpInitilizer.h"
#include "include/UsrpRxStreamer.h"

boost::atomic<bool> stop_streaming(false);

void streamingThread()
{
    std::cout << "Streaming Thread Launched" << std::endl;
    while(!stop_streaming){}
    std::cout << "Streaming Thread Finished" << std::endl;

}

void processingThread()
{
    std::cout << "Processinging Thread Launched" << std::endl;
    while(!stop_streaming){}
    std::cout << "Processing Thread Finished" << std::endl;
}

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
    double bw = 10e6;
    //stream settings
    std::string cpu_fmt = "sc16";
    std::string wire_fmt = "sc16";

    UsrpInitilizer my_usrp(ip, subdev, ant, clock_ref, time_ref, 
                           sample_rate, center_freq, gain, bw);
    std::cout << my_usrp.get_clock_ref() << std::endl;

    UsrpRxStreamer my_rx_streamer(my_usrp.get_usrp(), cpu_fmt, wire_fmt);
    my_rx_streamer.stream_rx_data(100);
    
    constexpr int ESC_KEY = 27;
    
//    boost::thread s_thread(streamingThread);
//    boost::thread p_thread(processingThread);

//    std::cout << "Press 'ESC' to stop streaming..." << std::endl;
//    while (true) {
//        if (std::cin.get() == ESC_KEY) { // 27 is the ASCII code for 'ESC'
//            stop_streaming = true;
//            break;
//        }
//        else{std::cout << std::cin.get() << std::endl;}
//    }
//
//    s_thread.join();
//    p_thread.join();

    std::cout << "Host Application ended" << std::endl;

    return 0;
}

