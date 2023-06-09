#include "Settings.h"
namespace sett
{
    //usrp settings
    std::string usrp_ip = "192.168.11.2";
    //std::string usrp_subdev = "A:0";
    std::string usrp_subdev = "A:A"; //B200 expects A:A
    std::string usrp_ant = "TX/RX";
    //std::string usrp_clock_ref = "external";
    //std::string usrp_time_ref = "_external_";
    std::string usrp_clock_ref = "gpsdo";//For internal GPSDO
    std::string usrp_time_ref = "gpsdo";//For internal GPSDO
    double usrp_sample_rate = 25e6;
    double usrp_center_freq =173935300;
    double usrp_gain = 10;
    double usrp_bw = 10e6;
    //stream settings 
    int rx_stream_time = 1; //seconds  
    std::string rx_stream_cpu_fmt = "sc16";
    std::string rx_stream_wire_fmt = "sc16";
    //processing settings
    float proc_threshold = 0.8;
    int proc_offset_samples = 1000;//only start looking for threshold after 1000 samples
    float proc_pulse_save_time = 30e-3;

    //save and display settings
    bool save_raw_data = true;
//    bool plot_raw_data = false;
    std::string raw_data_path = "./raw_data.bin";
    bool save_proc_data = true;
    std::string proc_data_path = "./proc_data.bin";
    bool save_pulse_data = true;
//    bool plot_pulse_data = false;
    std::string pulse_data_path = "./pulse_data.bin";

    //hostc settings
    uint32_t hostc_max_clients = 1;

    //std::vector<float> fir_coeffs = {0.000327,0.000243,0.000133,-0.000000,-0.000149,-0.000304,-0.000454,-0.000583,-0.000673,-0.000705,-0.000662,-0.000531,-0.000309,0.000000,0.000377,0.000791,0.001200,0.001552,0.001793,0.001870,0.001741,0.001382,0.000793,-0.000000,-0.000939,-0.001939,-0.002894,-0.003684,-0.004189,-0.004303,-0.003950,-0.003093,-0.001752,0.000000,0.002028,0.004146,0.006133,0.007746,0.008750,0.008940,0.008173,0.006384,0.003610,-0.000000,-0.004190,-0.008603,-0.012804,-0.016311,-0.018634,-0.019316,-0.017979,-0.014360,-0.008347,0.000000,0.010442,0.022565,0.035804,0.049474,0.062820,0.075068,0.085484,0.093429,0.098406,0.100101,0.098406,0.093429,0.085484,0.075068,0.062820,0.049474,0.035804,0.022565,0.010442,0.000000,-0.008347,-0.014360,-0.017979,-0.019316,-0.018634,-0.016311,-0.012804,-0.008603,-0.004190,-0.000000,0.003610,0.006384,0.008173,0.008940,0.008750,0.007746,0.006133,0.004146,0.002028,0.000000,-0.001752,-0.003093,-0.003950,-0.004303,-0.004189,-0.003684,-0.002894,-0.001939,-0.000939,-0.000000,0.000793,0.001382,0.001741,0.001870,0.001793,0.001552,0.001200,0.000791,0.000377,0.000000,-0.000309,-0.000531,-0.000662,-0.000705,-0.000673,-0.000583,-0.000454,-0.000304,-0.000149,-0.000000,0.000133,0.000243,0.000327 };

   // std::vector<float> fir_coeffs = { -0.000367,-0.000134,0.000150,0.000509,
   //                                    0.000949,0.001454,0.001969,0.002404,
   //                                    0.002637,0.002530,0.001950,0.000801,
   //                                   -0.000949,-0.003241,-0.005910,-0.008680,
   //                                   -0.011171,-0.012932,-0.013478,-0.012346,
   //                                   -0.009151,-0.003647,0.004232,0.014337,
   //                                   0.026292,0.039511,0.053236,0.066596,
   //                                   0.078684,0.088637,0.095722,0.099405,0.099405,
   //                                   0.095722,0.088637,0.078684,0.066596,0.053236,
   //                                   0.039511,0.026292,0.014337,0.004232,-0.003647,
   //                                   -0.009151,-0.012346,-0.013478,-0.012932,-0.011171,
   //                                   -0.008680,-0.005910,-0.003241,-0.000949,0.000801,
   //                                   0.001950,0.002530,0.002637,0.002404,0.001969,
   //                                   0.001454,0.000949,0.000509,0.000150,
   //                                   -0.000134,-0.000367 };
   
   // FIRPM designed filter, using Harris approximation.
    //std::vector<float> fir_coeffs = {0.000754, 0.001591, 0.003105, 0.005373, 0.008528, 0.012655, 0.017767, 0.023788, 0.030544, 0.037763, 0.045093, 0.052125, 0.058426, 0.063583, 0.067239, 0.069137, 0.069137, 0.067239, 0.063583, 0.058426, 0.052125, 0.045093, 0.037763, 0.030544, 0.023788, 0.017767, 0.012655, 0.008528, 0.005373, 0.003105, 0.001591, 0.000754};
   
    // Sinc filter with Kaiser window
    std::vector<float> fir_coeffs = {2.00000000000000e-05,5.00000000000000e-05,9.50000000000000e-05,0.000151000000000000,0.000209000000000000,0.000252000000000000,0.000254000000000000,0.000182000000000000,0,-0.000327000000000000,-0.000825000000000000,-0.00150600000000000,-0.00235300000000000,-0.00331900000000000,-0.00431600000000000,-0.00521600000000000,-0.00585200000000000,-0.00602600000000000,-0.00552600000000000,-0.00413900000000000,-0.00167900000000000,0.00199500000000000,0.00695100000000000,0.0131690000000000,0.0205300000000000,0.0288070000000000,0.0376800000000000,0.0467430000000000,0.0555380000000000,0.0635840000000000,0.0704170000000000,0.0756260000000000,0.0788890000000000,0.0800000000000000,0.0788890000000000,0.0756260000000000,0.0704170000000000,0.0635840000000000,0.0555380000000000,0.0467430000000000,0.0376800000000000,0.0288070000000000,0.0205300000000000,0.0131690000000000,0.00695100000000000,0.00199500000000000,-0.00167900000000000,-0.00413900000000000,-0.00552600000000000,-0.00602600000000000,-0.00585200000000000,-0.00521600000000000,-0.00431600000000000,-0.00331900000000000,-0.00235300000000000,-0.00150600000000000,-0.000825000000000000,-0.000327000000000000,0,0.000182000000000000,0.000254000000000000,0.000252000000000000,0.000209000000000000,0.000151000000000000,9.50000000000000e-05,5.00000000000000e-05,2.00000000000000e-05};


    bool _stob(std::string bool_string)
    {
        if (bool_string=="true"){return true;}
        return false;
    }

    void update_settings_from_file()
    {
        std::ifstream file("../../settings.txt");
        if (!file.is_open()) 
        {
            std::cout << "Failed to open file." << std::endl;
        }
        std::string line;
    //    int line_num = 0;
        while (std::getline(file, line)) 
        {
            std::cout << line << std::endl;
            size_t split_idx = line.find(":");
            std::string setting_name = line.substr(0, split_idx);
            std::string value = line.substr(split_idx + 2);//skip the : and space
            if (setting_name == "usrp_ip")
                usrp_ip = value;
            else if (setting_name == "usrp_subdev")
                usrp_subdev = value;
            else if (setting_name == "usrp_ant")
                usrp_ant = value;
            else if (setting_name == "usrp_clock_ref")
                usrp_clock_ref = value;
            else if (setting_name == "usrp_time_ref")
                usrp_time_ref = value;
            else if (setting_name == "usrp_sample_rate")
                usrp_sample_rate = std::stod(value);
            else if (setting_name == "usrp_center_freq")
                usrp_center_freq = std::stod(value);
            else if (setting_name == "usrp_gain")
                usrp_gain = std::stod(value);
            else if (setting_name == "usrp_bw")
                usrp_bw = std::stod(value);
            else if (setting_name == "rx_stream_time")
                rx_stream_time = std::stoi(value);
            else if (setting_name == "rx_stream_cpu_fmt")
                rx_stream_cpu_fmt = value;
            else if (setting_name == "rx_stream_wire_fmt")
                rx_stream_wire_fmt = value;
            else if (setting_name == "proc_threshold")
                proc_threshold = std::stof(value);
            else if (setting_name == "proc_offset_samples")
                proc_offset_samples = std::stoi(value);
            else if (setting_name == "proc_pulse_save_time")
                proc_pulse_save_time = std::stof(value);
            else if (setting_name == "save_raw_data")
                save_raw_data = _stob(value);
            else if (setting_name == "raw_data_path")
                raw_data_path = value;
            else if (setting_name == "save_proc_data")
                save_proc_data = _stob(value);
            else if (setting_name == "proc_data_path")
                proc_data_path = value;
            else if (setting_name == "save_pulse_data")
                save_pulse_data = _stob(value);
            else if (setting_name == "pulse_data_path")
                pulse_data_path = value;
            else if (setting_name == "hostc_max_clients")
                hostc_max_clients = std::stoi(value);
            else
                std::cout << setting_name << " is not a valid setting" << std::endl;
        }
        file.close();
    }
}//end namespace
