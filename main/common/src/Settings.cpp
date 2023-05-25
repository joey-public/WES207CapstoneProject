#include "Settings.h"
namespace sett{
    //usrp settings
    std::string usrp_ip = "192.168.11.2";
    std::string usrp_subdev = "A:0";
    std::string usrp_ant = "TX/RX";
    std::string usrp_clock_ref = "external";
    std::string usrp_time_ref = "_external_";
    double usrp_sample_rate = 25e6;
    double usrp_center_freq =173935300;
    double usrp_gain = 0;
    double usrp_bw = 10e6;
    //stream settings 
    int rx_stream_time = 1; //seconds  
    std::string rx_stream_cpu_fmt = "sc16";
    std::string rx_stream_wire_fmt = "sc16";
    //processing settings
    SAMP_DTYPE proc_threshold = 1500;
    int proc_offset_samples = 1000;//only start looking for threshold after 1000 samples
    float proc_pulse_save_time = 0.02;

    //save and display settings
    bool save_raw_data = true;
    bool plot_raw_data = true;
    std::string raw_data_path = "./raw_data.bin";
    bool save_pulse_data = true;
    bool plot_pulse_data = true;
    std::string pulse_data_path = "./pulse_data.bin";
}//end namespace
