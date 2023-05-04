#include "include/UsrpRxStreamFuncs.h"

void stream_rx_data(uhd::usrp::multi_usrp::sptr usrp)
{
    std::cout << "Start Streaming Data..." << std::endl;
    //create stream args
    //create rx_streamer
    //configure stream cmds
    //start stream
}

/*
//constructor
UsrpRxStreamer::UsrpRxStreamer(uhd::usrp::multi_usrp::sptr u, std::string cf, std::string wf)
{
    this->usrp = u;
    this->cpu_fmt = cf;
    this->wire_fmt = wf;
    this->rx_stream_args = this->_generate_stream_args();
    this->rx_stream_cmd = this->_generate_stream_cmd();

    this->init_rx_streamer(); 
    //note these variables are defined in the header file 
    //size_t recv_buff_sz{352};
    //std::array<std::complex<float>, 352> recv_buffer{};
}

//public funcitons
void UsrpRxStreamer::init_rx_streamer()
{
    this->rx_streamer = this->usrp->get_rx_stream(this->rx_stream_args);
}

void UsrpRxStreamer::stream_rx_data()
{
    std::cout << "Start Streaming Data..." << std::endl;
    std::cout << "RX Rate: " << std::to_string(this->usrp->get_rx_rate()) << std::endl;
    std::cout << "RX Buff Size: " << std::to_string(this->recv_buff_sz) << std::endl;

    //setup streaming command
    this->rx_streamer->issue_stream_cmd(stream_cmd);

    //initilize streaming metadata
    uhd::rx_metadata_t md;
    
    int overflow_count = 0;
    size_t rx_sample_count = 0;
    std::cout << std::to_string(60000*buff_size) << std::endl;
    while(rx_sample_count < 60000*buff_size)
    {
        //3.0 second timeout
        size_t num_recv_samps = this->rx_streamer->recv(&rx_buff.front(), rx_buff.size(), md, 3.0);
        overflow_count += this->_handle_recv_errors(md, rx_sample_count); 
        rx_sample_count += num_recv_samps;
    }

    std::cout << "Finished collecting " << std::to_string(rx_sample_count) << " samples." << std::endl;
    std::cout << "Overflow Count: " << std::to_string(overflow_count) << std::endl;
    std::cout << "Overflow ratio: " << std::to_string(overflow_count/rx_sample_count) << std::endl;
}

//private functions
uhd::stream_args_t UsrpRxStreamer::_generate_stream_args()
{
    this->rx_stream_args.cpu_fmt = this->cpu_fmt;
    this->rx_stream_args.wire_fmt = this->wire_fmt;
    this->rx_stream_args.args["spp"] = std::to_string(this->recv_buff_sz); 
}

uhd::stream_cmd_t UsrpRxStreamer::_generate_stream_cmd()
{
    this->rx_stream_cmd.stream_mode_t = uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS;
    this->stream_now = false; //we dont want to stream asap, we want to wait for host-controller command then start at the next pps
}

void UsrpRxStreamer::_preprocess_rx_samples(const uhd::rx_metadata_t& md, 
                                const std::vector<std::complex<float>>& samples) 
{
    std::cout << md.strerror() << std::endl;
}

int UsrpRxStreamer::_handle_recv_errors(uhd::rx_metadata_t m, size_t samp_count)
{
    switch (m.error_code)
    {
        case uhd::rx_metadata_t::ERROR_CODE_NONE:
            break;
        case uhd::rx_metadata_t::ERROR_CODE_TIMEOUT:
            if (samp_count == 0) {break;}
            std::cout << "----------HELLO----------" << std::endl;
            std::cout << "RX Count: " << std::to_string(samp_count) << std::endl;
            std::cout << m.to_pp_string() << std::endl;
            std::cout << "----------HELLO----------" << std::endl;
            throw std::runtime_error("ERROR_CODE_TIMEOUT");
        default:
            std::cout << m.to_pp_string() << std::endl;
            return 1;
            break;
   //         throw std::runtime_error(m.strerror());
    }
    return 0;
}
*/
