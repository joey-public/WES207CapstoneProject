#include <iostream>
#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <boost/version.hpp>

#include "include/UsrpHandler.h"

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
    
    std::string ip = "192.168.11.2";
    UsrpHandler my_usrp{};
    my_usrp.initilize(ip);
    std::cout << my_usrp.get_clock_ref() << std::endl;
    
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
