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
    bool plot_raw_data = false;
    std::string raw_data_path = "./raw_data.bin";
    bool save_pulse_data = true;
    bool plot_pulse_data = false;
    std::string pulse_data_path = "./pulse_data.bin";

    std::vector<float> fir_coeffs = {0.000327,0.000243,0.000133,-0.000000,-0.000149,-0.000304,-0.000454,-0.000583,-0.000673,-0.000705,-0.000662,-0.000531,-0.000309,0.000000,0.000377,0.000791,0.001200,0.001552,0.001793,0.001870,0.001741,0.001382,0.000793,-0.000000,-0.000939,-0.001939,-0.002894,-0.003684,-0.004189,-0.004303,-0.003950,-0.003093,-0.001752,0.000000,0.002028,0.004146,0.006133,0.007746,0.008750,0.008940,0.008173,0.006384,0.003610,-0.000000,-0.004190,-0.008603,-0.012804,-0.016311,-0.018634,-0.019316,-0.017979,-0.014360,-0.008347,0.000000,0.010442,0.022565,0.035804,0.049474,0.062820,0.075068,0.085484,0.093429,0.098406,0.100101,0.098406,0.093429,0.085484,0.075068,0.062820,0.049474,0.035804,0.022565,0.010442,0.000000,-0.008347,-0.014360,-0.017979,-0.019316,-0.018634,-0.016311,-0.012804,-0.008603,-0.004190,-0.000000,0.003610,0.006384,0.008173,0.008940,0.008750,0.007746,0.006133,0.004146,0.002028,0.000000,-0.001752,-0.003093,-0.003950,-0.004303,-0.004189,-0.003684,-0.002894,-0.001939,-0.000939,-0.000000,0.000793,0.001382,0.001741,0.001870,0.001793,0.001552,0.001200,0.000791,0.000377,0.000000,-0.000309,-0.000531,-0.000662,-0.000705,-0.000673,-0.000583,-0.000454,-0.000304,-0.000149,-0.000000,0.000133,0.000243,0.000327 }

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

    void update_settings_from_file()
    {
        std::ifstream file("../../settings.txt");
        if (!file.is_open()) 
        {
            std::cout << "Failed to open file." << std::endl;
        }
        std::string line;
        int line_num = 0;
        while (std::getline(file, line)) 
        {
            std::cout << line << std::endl;
            switch (line_num) 
            {
                case 0: usrp_ip = line;        break;
                case 1: usrp_subdev = line;    break;
                case 2: usrp_ant = line;       break;
                case 3: usrp_clock_ref = line; break;
                case 4: usrp_time_ref = line;  break;
                case 5: usrp_sample_rate = std::stod(line); break;
                case 6: usrp_center_freq = std::stod(line); break;
                case 7: usrp_gain = std::stod(line);        break;
                case 8: usrp_bw = std::stod(line);          break;
                case 9: rx_stream_time = std::stoi(line);   break;
                case 10: rx_stream_cpu_fmt = line;          break;
                case 11: rx_stream_wire_fmt = line;         break;
                case 12: proc_threshold = std::stof(line); break;
                case 13: proc_offset_samples = std::stoi(line);     break;
                case 14: proc_pulse_save_time = std::stof(line);    break;
                case 15: save_raw_data = true;   break;
                case 16: plot_raw_data = true;   break;
                case 17: raw_data_path = line;   break;
                case 18: save_pulse_data = true; break;
                case 19: plot_pulse_data = true; break;
                case 20: pulse_data_path = line; break;
            }
            line_num ++;
        }
        file.close();
    }
}//end namespace
