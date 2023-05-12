#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <uhd/utils/safe_main.hpp>

#include "include/UsrpInitFuncs.h"
#include "include/UsrpRxStreamFuncs.h"
#include "include/ProcessingFuncs.h"
#include "include/UtilFuncs.h"

int UHD_SAFE_MAIN(int argc, char* argv[])
{
    std::cout << "Host Application Launched" << std::endl;
    std::cout << "Boost Version: " << BOOST_VERSION << std::endl;
    
    //usrp settings
    std::string ip = "192.168.11.2";
    std::string subdev = "A:0";
    std::string ant = "TX/RX";
    std::string clock_ref = "internal";
    std::string time_ref = "none";
    double sample_rate = 1e6;
    double center_freq = 174e6;
    double gain = 0;
    double bw = 20e6;
    //stream settings
    std::string cpu_fmt = "sc16";
    std::string wire_fmt = "sc16";

    uhd::usrp::multi_usrp::sptr usrp = gen_usrp(ip, subdev, ant, clock_ref, time_ref, sample_rate, center_freq, gain, bw);
    
    //cacl buffer size for desired time
    int stream_time = 2;//seconds
    size_t buffer_sz = stream_time * usrp->get_rx_rate();

    //fill the buffer with data from the usrp
    std::vector<std::complex<float>> data_buffer(buffer_sz);
    stream_rx_data(usrp, buffer_sz, &data_buffer.front());
   
    //process the data
    process_data(data_buffer);
    
    //save data to file
     
//    save_raw_data_to_file(data_buffer);

    std::cout << "Host Application ended" << std::endl;

    return EXIT_SUCCESS;
}

