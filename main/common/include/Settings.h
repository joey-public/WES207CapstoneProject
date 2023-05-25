#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <fstream>
#include <string>
#include "TypeDefs.h"

namespace sett{
    //usrp settings
    extern std::string usrp_ip;
    extern std::string usrp_subdev;
    extern std::string usrp_ant;
    extern std::string usrp_clock_ref;
    extern std::string usrp_time_ref;
    extern double usrp_sample_rate;
    extern double usrp_center_freq;
    extern double usrp_gain;
    extern double usrp_bw;

    //stream settings 
    extern int rx_stream_time; //seconds  
    extern std::string rx_stream_cpu_fmt;
    extern std::string rx_stream_wire_fmt;

    //processing settings
    extern SAMP_DTYPE proc_threshold;
    extern int proc_offset_samples;//only start looking for threshold after 1000 samples
    extern float proc_pulse_save_time;

    //save and display settings
    extern bool save_raw_data;
    extern bool plot_raw_data;
    extern std::string raw_data_path;
    extern bool save_pulse_data;
    extern bool plot_pulse_data;
    extern std::string pulse_data_path;

    void update_settings_from_file();
}//end namespace
 
#endif
