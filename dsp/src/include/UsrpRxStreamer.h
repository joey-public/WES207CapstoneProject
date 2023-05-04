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

        size_t recv_buff_sz{352};
        std::array<std::complex<float>, 352> recv_buffer{};

        uhd::stream_args_t rx_stream_args(cpu_fmt, wire_fmt);
        udh::stream_cmd_t  rx_stream_cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);
        uhd::rx_streamer::sptr rx_streamer{};

    public: //public methods
        UsrpRxStreamer(uhd::usrp::multi_usrp::sptr usrp, std::string cf="fc32", 
                           std::string wf="sc16");
        void init_rx_streamer();
        void stream_rx_data();

    private: //private methods
        uhd::stream_args_t _generate_stream_args();
        uhd::stream_cmd_t _generate_stream_cmd();
        void _preprocess_rx_samples(const uhd::rx_metadata_t& md, 
                                    const std::vector<std::complex<float>>& samples); 
        int _handle_recv_errors(uhd::rx_metadata_t m, size_t samp_count);
};

#endif
