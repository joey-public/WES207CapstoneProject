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

//#define ENABLE_TESTS
#define ENABLE_USRP
#define SAVE_DATA
 
int UHD_SAFE_MAIN(int argc, char* argv[])
{
    std::cout << "Host Application Launched" << std::endl;
    std::cout << "Boost Version: " << BOOST_VERSION << std::endl;

#ifdef ENABLE_TESTS
    run_all_tests();
#endif
    
#ifdef ENABLE_USRP
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
    
    //cacl buffer size for desired time
    //DO NOT MAKE stream time < 1, this will cause seg fault for some reason...
    int stream_time = 2;//seconds
    size_t buffer_sz = stream_time * usrp->get_rx_rate();

    //Stream the raw data
    //fill the buffer with data from the usrp
    std::vector<std::complex<float>> data_buffer(buffer_sz);
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "Start Streaming Data..." << std::endl;
    stream_rx_data(usrp, buffer_sz, &data_buffer.front());
    std::cout << "Stop Streaming Data..." << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    
    //Analyze the raw data
    std::cout << "Analyzing Raw Data..." << std::endl;
    float buff_mem = sizeof(std::complex<float>) * buffer_sz;//bytes 
    //print stats about size of data buffer
    std::cout << "\tCollected " << stream_time << "s of raw data at fs = "
              << usrp->get_rx_rate() << std::endl;
    std::cout << "\tBuffer length: " << buffer_sz << std::endl;
    std::cout << "\tBuffer takes: " << buff_mem / 1e6 << " Mb of memory" << std::endl;
    //save data to file
#ifdef SAVE_DATA
    std::cout << "\tSaving Raw data to txt file\n";
    std::string data_file_path = "./raw_data.txt"; 
    save_complex_vec_to_file(data_buffer, data_file_path);
#endif
    std::cout << "Done Analyzing Raw Data..." << std::endl;
    std::cout << "-------------------------------------" << std::endl;
   
    //Process the data
    std::cout << "Start Processing Data..." << std::endl;
    float threshold = 0.01;
    float save_time = 0.02;//20ms 
    int offset_time = 0*usrp->get_rx_rate();
    std::cout << "\tTakeing the magnitude..." << std::endl;
    std::vector<float> mag_data = calc_mag(data_buffer);
    buff_mem = sizeof(float) * mag_data.size();//bytes 
    std::cout << "\tMag Data takes: " << buff_mem / 1e6 << " Mb of memory" << std::endl;
    //save data to file
#ifdef SAVE_DATA
    std::cout << "\tSaving Mag data to txt file...\n";
    data_file_path = "./mag_data.txt"; 
    save_float_vec_to_file(mag_data, data_file_path);
#endif
    std::cout << "\tDoing threshold detection..." << std::endl;
    int start_idx = detect_threshold(mag_data, threshold, offset_time);
    if (start_idx < 0)
    {
        std::cout << "\tNo Peak Detected...\n";
    }
    else
    {
        std::cout << "\tPulse Detected starting at index: " << start_idx << std::endl;
        int k = int(save_time * usrp->get_rx_rate());
        std::vector<std::complex<float>> pulse_data = get_subvec(data_buffer, start_idx, k);
        buff_mem = sizeof(std::complex<float>) * pulse_data.size();//bytes 
        std::cout << "\tPulse Data takes: " << buff_mem / 1e6 << " Mb of memory" << std::endl;
        //save data to file
#ifdef SAVE_DATA
        std::cout << "\tSaving Pulse data to txt file\n";
        data_file_path = "./pulse_data.txt"; 
        save_complex_vec_to_file(pulse_data, data_file_path);
#endif
    }
    std::cout << "Stop Processing Data..." << std::endl;
    std::cout << "-------------------------------------" << std::endl;
#endif

    std::cout << "Host Application ended" << std::endl;
    return EXIT_SUCCESS;
}
