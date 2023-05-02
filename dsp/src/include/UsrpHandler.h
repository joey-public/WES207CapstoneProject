#ifndef USRP_HANDLE_H
#define USRP_HANDLE_H

//std includes
#include <iostream>
//uhd includes
#include <uhd/utils/thread.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/exception.hpp>
#include <uhd/types/tune_request.hpp>
//boost includes
#include <boost/program_options.hpp>
//#include <boost/format.hpp>
#include <boost/thread.hpp>

class UsrpHandler
{
    private: //instance variables
        std::string ip_addr {"addr=192.168.11.2"};
        std::string subdev {"A:0"};
        std::string ant {"TX/RX"};
        std::string clock_ref {"internal"};
        std::string time_ref{"none"};

        double sample_rate {1e6};
        double center_freq {915e6};
        double gain {0};
        double bw {1e6};

        bool use_agc {false};

        uhd::usrp::multi_usrp::sptr usrp {};

    public: //public methods
        void initilize(std::string ipaddr);
        uhd::usrp::multi_usrp::sptr init_usrp(std::string ipaddr);

        //setters 
        void set_ip_addr(std::string ip);
        void set_subdev(std::string sd);
        void set_clock_ref(std::string cs);
        void set_time_ref(std::string ts);
        void set_ant(std::string a);
        void set_sample_rate(double fs);
        void set_center_freq(double fc);
        void set_bw(double b);
        void set_gain(double g);
        //getters
        std::string get_ip_addr();
        std::string get_subdev();
        std::string get_ant();
        std::string get_clock_ref();
        std::string get_time_ref();
        double get_sample_rate();
        double get_center_freq();
        double get_gain();
        double get_bw();
};

#endif
