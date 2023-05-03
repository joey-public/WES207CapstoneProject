#include "include/UsrpRxStreamer.h"

//constructor
UsrpRxStreamer::UsrpRxStreamer(uhd::usrp::multi_usrp::sptr u, std::string cf, std::string wf)
{
    this->usrp = u;
    this->cpu_fmt = cf;
    this->wire_fmt = wf;

    this->init_rx_streamer(); 
}

//public funcitons
void UsrpRxStreamer::init_rx_streamer()
{
    uhd::stream_args_t stream_args(this->cpu_fmt, this->wire_fmt);
    //stream_args.args["spp"] = "100";
    this->rx_streamer = this->usrp->get_rx_stream(stream_args);
}

void UsrpRxStreamer::stream_rx_data(size_t N)
{
    std::cout << "Start Streaming Data..." << std::endl;
    std::cout << "RX Rate: " << std::to_string(this->usrp->get_rx_rate()) << std::endl;

    //setup streaming command
    uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);
    stream_cmd.stream_now = true;
    this->rx_streamer->issue_stream_cmd(stream_cmd);
    //initilize streaming metadata
    uhd::rx_metadata_t md;
    //create a buffer 
    size_t buff_size = this->rx_streamer->get_max_num_samps(); 
    std::cout << std::to_string(buff_size) << std::endl;
    std::vector<std::complex<float>> rx_buff(buff_size);
    
    
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
