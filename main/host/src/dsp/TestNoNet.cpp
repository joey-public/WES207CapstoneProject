#include "TestNoNet.h"
//#define ENABLE_TESTS
#define ENABLE_USRP
#define SAVE_DATA
#define SAVE_DATA_PULSE
#define PLOT_DATA
#define PLOT_DATA_PULSE
 
void test()
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
    std::string clock_ref = "external";
    std::string time_ref = "_external_";
    double sample_rate = 25e6;
    double center_freq =173935300;
    double gain = 0;
    double bw = 10e6;
    //stream settings 
    std::string cpu_fmt = "sc16";
    std::string wire_fmt = "sc16";

    uhd::usrp::multi_usrp::sptr usrp = init::gen_usrp(ip, subdev, ant, clock_ref, time_ref, sample_rate, center_freq, gain, bw);
    
    //cacl buffer size for desired time
    //DO NOT MAKE stream time < 1, this will cause seg fault for some reason...
    int stream_time = 1;//seconds
    size_t buffer_sz = stream_time * usrp->get_rx_rate();

    //Stream the raw data
    //fill the buffer with data from the usrp
    std::vector<RX_DTYPE> data_buffer(buffer_sz);
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "Start Streaming Data..." << std::endl;
    rx_strm::stream_rx_data_nsamps(usrp, buffer_sz, &data_buffer.front(), 
                                   cpu_fmt, wire_fmt);
    std::cout << "Stop Streaming Data..." << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    
    //Analyze the raw data
    std::cout << "Analyzing Raw Data..." << std::endl;
    float buff_mem = sizeof(RX_DTYPE) * buffer_sz;//bytes 
    //print stats about size of data buffer
    std::cout << "\tCollected " << stream_time << "s of raw data at fs = "
              << usrp->get_rx_rate() << std::endl;
    std::cout << "\tBuffer length: " << buffer_sz << std::endl;
    std::cout << "\tBuffer takes: " << buff_mem / 1e6 << " Mb of memory" << std::endl;
    //save data to file
    std::string data_file_path = ""; 
#ifdef SAVE_DATA
    std::cout << "\tSaving Raw data to txt file\n";
    data_file_path = "./raw_data.bin"; 
    util::save_complex_vec_to_file_bin(data_buffer, data_file_path);
#ifdef PLOT_DATA
    std::cout << "\tPlotting Data with python script\n";
    util::plot_with_python(data_file_path);
#endif
#endif
    std::cout << "Done Analyzing Raw Data..." << std::endl;
    std::cout << "-------------------------------------" << std::endl;
   
    //Process the data
    std::cout << "Start Processing Data..." << std::endl;
    int16_t threshold = 1500;
    float save_time = 0.02;//20ms 
    int offset_samps = 1000;
    std::cout << "\tTakeing the magnitude..." << std::endl;
    std::vector<int16_t> mag_data = proc::calc_mag(data_buffer);
    buff_mem = sizeof(int16_t) * mag_data.size();//bytes 
    std::cout << "\tMag Data takes: " << buff_mem / 1e6 << " Mb of memory" << std::endl;
    //save data to file
//#ifdef SAVE_DATA
//    std::cout << "\tSaving Mag data to txt file...\n";
//    data_file_path = "./mag_data.bin"; 
//    util::save_float_vec_to_file(mag_data, data_file_path);
//#endif
    std::cout << "\tDoing threshold detection..." << std::endl;
    int start_idx = proc::detect_threshold(mag_data, threshold, offset_samps);
    if (start_idx < 0)
    {
        std::cout << "\tNo Peak Detected...\n";
    }
    else
    {
        std::cout << "\tPulse Detected starting at index: " << start_idx << std::endl;
        int k = int(save_time * usrp->get_rx_rate());
        std::vector<RX_DTYPE> pulse_data = util::get_subvec(data_buffer, start_idx, k);
        buff_mem = sizeof(RX_DTYPE) * pulse_data.size();//bytes 
        std::cout << "\tPulse Data takes: " << buff_mem / 1e6 << " Mb of memory" << std::endl;
        //save data to file
#ifdef SAVE_DATA_PULSE
        std::cout << "\tSaving Pulse data to txt file\n";
        data_file_path = "./pulse_data.bin"; 
        util::save_complex_vec_to_file_bin(pulse_data, data_file_path);
#ifdef PLOT_DATA_PULSE
    std::cout << "\tPlotting Data with python script\n";
    util::plot_with_python(data_file_path);
#endif
#endif
    }
    std::cout << "Stop Processing Data..." << std::endl;
    std::cout << "-------------------------------------" << std::endl;
#endif

    std::cout << "Host Application ended" << std::endl;
}
