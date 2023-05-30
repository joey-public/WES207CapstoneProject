#include "TestNoNet.h"
//#define ENABLE_TESTS
//#define ENABLE_USRP
 
void test()
{
    std::cout << "Host Application Launched" << std::endl;
#ifdef ENABLE_TESTS
    run_all_tests();
#endif
    std::cout << "RX_DTYPE sz: "<< sizeof(RX_DTYPE) << std::endl;
    std::cout << "SAMP_DTYPE sz: "<< sizeof(SAMP_DTYPE) << std::endl;
    std::cout << "float sz: "<< sizeof(float) << std::endl;
    std::cout << "double sz: "<< sizeof(double) << std::endl;
    int16_t a = 9;
    float b = 0.5;
    float c = a * b;
    std::cout << "float * int16_t" << c << std::endl; 
    //Setup the USRP device
    sett::update_settings_from_file();
    uhd::usrp::multi_usrp::sptr usrp = init::gen_usrp(sett::usrp_ip, 
                                                      sett::usrp_subdev, 
                                                      sett::usrp_ant, 
                                                      sett::usrp_clock_ref, 
                                                      sett::usrp_time_ref, 
                                                      sett::usrp_sample_rate, 
                                                      sett::usrp_center_freq, 
                                                      sett::usrp_gain, 
                                                      sett::usrp_bw);

    //sync to gps
    rx_strm::synchronize_gps(usrp); 

    //Stream the data 
    size_t buffer_sz = sett::rx_stream_time * usrp->get_rx_rate();
    std::vector<RX_DTYPE> data_buffer(buffer_sz);
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "Start Streaming Data..." << std::endl;
    rx_strm::stream_rx_data_nsamps(usrp, 
                                   buffer_sz, 
                                   &data_buffer.front(), 
                                   sett::rx_stream_cpu_fmt, 
                                   sett::rx_stream_wire_fmt);
    std::cout << "Stop Streaming Data..." << std::endl;
    std::cout << "-------------------------------------" << std::endl;

    //Analyze the raw data
    float buff_mem = sizeof(RX_DTYPE) * buffer_sz;//bytes 
    std::cout << "Analyzing Raw Data..." << std::endl;
    std::cout << "\tCollected " << sett::rx_stream_time << "s of raw data at fs = "
              << usrp->get_rx_rate() << std::endl;
    std::cout << "\tBuffer length: " << buffer_sz << std::endl;
    std::cout << "\tBuffer takes: " << buff_mem / 1e6 << " Mb of memory" << std::endl;
    if(sett::save_raw_data)
    {
        std::cout << "\tSaving Raw data to txt file\n";
        util::save_complex_vec_to_file_bin(data_buffer, sett::raw_data_path);
        if(sett::plot_raw_data)
        {
            std::cout << "\tPlotting Data with python script\n";
            util::plot_with_python(sett::raw_data_path, usrp->get_rx_rate());
        }
    }
    std::cout << "Done Analyzing Raw Data..." << std::endl;
    std::cout << "-------------------------------------" << std::endl;
   
    //Process the data
    std::cout << "Start Processing Data..." << std::endl;
    proc::dsp_struct dsp_result = proc::process_data(data_buffer, usrp->get_rx_rate());
//    std::cout << "\tTaking the magnitude..." << std::endl;
//    std::vector<SAMP_DTYPE> mag_data = proc::calc_norm_mag(data_buffer);
//    buff_mem = sizeof(SAMP_DTYPE) * mag_data.size();//bytes 
//    std::cout << "\tMag Data takes: " << buff_mem / 1e6 << " Mb of memory" << std::endl;
//    std::cout << "\tDoing threshold detection..." << std::endl;
//    int start_idx = proc::detect_threshold(mag_data, 
//                                           sett::proc_threshold, 
//                                           sett::proc_offset_samples);
//    if (start_idx < 0) 
//    {
//        std::cout << "\tNo Peak Detected...\n";
//    }
//    else //pulse was detected
//    {
//        std::cout << "\tPulse Detected starting at index: " << start_idx << std::endl;
//        int end_idx = int(sett::proc_pulse_save_time * usrp->get_rx_rate());
//        std::vector<RX_DTYPE> pulse_data = util::get_subvec(data_buffer, start_idx, end_idx);
//        buff_mem = sizeof(RX_DTYPE) * pulse_data.size();//bytes 
//        std::cout << "\tPulse Data takes: " << buff_mem / 1e6 << " Mb of memory" << std::endl;
//        if(sett::save_pulse_data)
//        {
//            std::cout << "\tSaving Pulse data to txt file\n";
//            util::save_complex_vec_to_file_bin(pulse_data, sett::pulse_data_path);
//            if(sett::plot_pulse_data)
//            {
//                std::cout << "\tPlotting Data with python script\n";
//                util::plot_with_python(sett::pulse_data_path, usrp->get_rx_rate());
//            }
//        }
//    }
    std::cout << "Stop Processing Data..." << std::endl;
    std::cout << "-------------------------------------" << std::endl;

    std::cout << "Host Test Application ended" << std::endl;
}
