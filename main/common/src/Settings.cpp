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
    SAMP_DTYPE proc_threshold = 800;
    int proc_offset_samples = 1000;//only start looking for threshold after 1000 samples
    float proc_pulse_save_time = 0.02;

    //save and display settings
    bool save_raw_data = true;
    bool plot_raw_data = true;
    std::string raw_data_path = "./raw_data.bin";
    bool save_pulse_data = true;
    bool plot_pulse_data = true;
    std::string pulse_data_path = "./pulse_data.bin";

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
                case 12: proc_threshold = int16_t(std::stoi(line)); break;
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
