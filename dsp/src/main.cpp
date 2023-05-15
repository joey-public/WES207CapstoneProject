#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <uhd/utils/safe_main.hpp>

#include "include/UsrpInitFuncs.h"
#include "include/UsrpRxStreamFuncs.h"
#include "include/ProcessingFuncs.h"
#include "include/UtilFuncs.h"

#include "include/Tests.h"

#define ENABLE_TESTS
//#define ENABLE_USRP

using Eigen::MatrixXd;
 
int UHD_SAFE_MAIN(int argc, char* argv[])
{
    std::cout << "Host Application Launched" << std::endl;
    std::cout << "Boost Version: " << BOOST_VERSION << std::endl;

#ifdef ENABLE_TESTS
    std::cout << "Running Tests..." << std::endl;
    if (!(test_calc_mag()))
    {
        std::cout << "\tcalc_mag() failed!" << std::endl;
        return 0;
    }
    else
    {
        std::cout << "\tcalc_mag() passed!" << std::endl;
    }

    if (!(test_calc_phase()))
    {
        std::cout << "\tcalc_phase() failed!" << std::endl;
        return 0;
    }
    else
    {
        std::cout << "\tcalc_phase() passed!" << std::endl;
    }
#endif
    
#ifdef ENABLE_USRP
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
    //DO NOT MAKE stream time < 1, this will cause seg fault for some reason...
    int stream_time = 3;//seconds
    size_t buffer_sz = stream_time * usrp->get_rx_rate();

    //fill the buffer with data from the usrp
    std::vector<std::complex<float>> data_buffer(buffer_sz);
    stream_rx_data(usrp, buffer_sz, &data_buffer.front());
    float buff_mem = sizeof(std::complex<float>) * buffer_sz;//bytes 
    
    //print stats about size of data buffer
    std::cout << "Collected " << stream_time << "s of raw data at fs = "
              << usrp->get_rx_rate() << std::endl;
    std::cout << "\tBuffer length: " << buffer_sz << std::endl;
    std::cout << "\tBuffer takes: " << buff_mem / 1e6 << " Mb of memory" << std::endl;

    //save data to file
    std::cout << "Saving Raw data to txt file\n";
    std::string data_file_path = "./raw_data.txt"; 
    save_complex_vec_to_file(data_buffer, data_file_path);
   
    //process the data
//    process_data(data_buffer);
    std::cout << "Processing the data\n\tTaking the magnitude...";
    std::vector<float> mag_data = calc_mag(data_buffer);

    //save data to file
    std::cout << "Saving Mag data to txt file\n";
    data_file_path = "./mag_data.txt"; 
    save_float_vec_to_file(mag_data, data_file_path);
#endif


    std::cout << "Host Application ended" << std::endl;

    return EXIT_SUCCESS;
}

