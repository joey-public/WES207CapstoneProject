#ifndef USRP_RX_STREAM_H
#define USRP_RX_STREAM_H

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

class UsrpRxStreamer
{
    private: //instance variables
        uhd::usrp::multi_usrp::sptr usrp {};

        std::string cpu_fmt{"fc32"};
        std::string wire_fmt{"sc16"};
//        size_t buffer_size{50000};

        uhd::rx_streamer::sptr rx_streamer{};

    public: //public methods
        UsrpRxStreamer(uhd::usrp::multi_usrp::sptr usrp, std::string cf, std::string wf);
        void init_rx_streamer();
        void stream_rx_data(size_t N);

    private: //private methods
        void _preprocess_rx_samples(const uhd::rx_metadata_t& md, 
                                    const std::vector<std::complex<float>>& samples); 
        int _handle_recv_errors(uhd::rx_metadata_t m, size_t samp_count);
};

#endif
