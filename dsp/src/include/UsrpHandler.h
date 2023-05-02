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
#include <boost/format.hpp>
#include <boost/thread.hpp>

class UsrpHandler
{
    private: //instance variables
        std::string ip_addr {"addr=192.168.10.2"};
        std::string subdev {"A:0"};
        std::string ant {"TX/RX"};
        std::string ref {"internal"};

        double sample_rate {1e6};
        double center_freq {915e6};
        double gain {0};
        double bw {1e6};

        bool use_agc {false};

        uhd::usrp::multi_usrp::sptr usrp {};

    public: //public methods
        void initilize();
        
        void set_clock_source(std::string r);
        std::string get_clock_source();
};

#endif
