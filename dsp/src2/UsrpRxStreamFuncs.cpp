#include "include/UsrpRxStreamFuncs.h"

void stream_rx_data(uhd::usrp::multi_usrp::sptr usrp)
{
    std::cout << "Start Streaming Data..." << std::endl;

    std::string cpu_fmt = "fc32";
    std::string wire_fmt = "sc16";

    //create stream args
    uhd::stream_args_t stream_args(cpu_fmt, wire_fmt);
    uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(stream_args);
    size_t recv_pkt_sz = rx_stream->get_max_num_samps();
    float recv_pkt_dt = recv_pkt_sz / usrp->get_rx_rate();

    std::cout << "RX Buff Size: " << std::to_string(recv_pkt_sz) << std::endl;
    std::cout << "\t pkt dt = " << std::to_string(recv_pkt_sz)
                                << " samples / " 
                                << std::to_string(usrp->get_rx_rate() / 1e6)  
                                << " Mhz = " 
                                << std::to_string((recv_pkt_sz * 1e6) / usrp->get_rx_rate())
                                << " us" << std::endl;
    
    //create double buffers for receiveing packets from usrp
    std::vector<std::complex<float>> pkt_buff_0(recv_pkt_sz);
    std::vector<std::complex<float>> pkt_buff_1(recv_pkt_sz);
    bool use_recv_buff_0 = true;

    //initilize streaming metadata
    uhd::rx_metadata_t md;
    float stream_timeout = 3.0;
    //configure stream cmds
    rx_stream->issue_stream_cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);
    
    size_t num_recv_samps = 0;
    size_t rx_sample_count = 0;
    size_t total_samples = 1e6;
    while(rx_sample_count < total_samples)
    { 
        if(use_recv_buff_0)
        { 
            num_recv_samps = rx_stream->recv(&pkt_buff_0.front(), recv_pkt_sz, md, stream_timeout);
        }
        else
        {
            num_recv_samps = rx_stream->recv(&pkt_buff_1.front(), recv_pkt_sz, md, stream_timeout);
        }
        rx_sample_count += num_recv_samps;        
        use_recv_buff_0 = !use_recv_buff_0;
    }
}

void stream_rx_data_2(uhd::usrp::multi_usrp::sptr usrp)
{
    std::cout << "Start Streaming Data..." << std::endl;

    std::string cpu_fmt = "fc32";
    std::string wire_fmt = "sc16";

    //create stream args
    uhd::stream_args_t stream_args(cpu_fmt, wire_fmt);
    uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(stream_args);
    size_t recv_pkt_sz = rx_stream->get_max_num_samps();
    float recv_pkt_dt = recv_pkt_sz / usrp->get_rx_rate();

    std::cout << "RX Buff Size: " << std::to_string(recv_pkt_sz) << std::endl;
    std::cout << "\t pkt dt = " << std::to_string(recv_pkt_sz)
                                << " samples / " 
                                << std::to_string(usrp->get_rx_rate() / 1e6)  
                                << " Mhz = " 
                                << std::to_string((recv_pkt_sz * 1e6) / usrp->get_rx_rate())
                                << " us" << std::endl;
    
    //create double buffers for receiveing packets from usrp
    int num_pkts_to_recv = 25;
    size_t recv_buff_sz = num_pkts_to_recv*recv_pkt_sz; 
    std::complex<float> recv_buff_0(recv_buff_sz);
    std::complex<float> recv_buff_1(recv_buff_sz);
    std::complex<float>* recv_ptr = &recv_buff_0; 

    //initilize streaming metadata
    uhd::rx_metadata_t md;
    float stream_timeout = 3.0;
    //configure stream cmds
    rx_stream->issue_stream_cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);
    
    size_t num_recv_samps = 0;
    size_t rx_sample_count = 0;
    size_t total_samples = recv_buff_sz;
    while(rx_sample_count < 10)
    { 
        if(recv_ptr > (&recv_buff_0 + recv_buff_sz))
        {
            std::cout<<"oops something is wrong here..."
                <<"\n\tnum_recv_sampls = "
                <<std::to_string(num_recv_samps)
                << std::endl;
            return;
        }
        num_recv_samps = rx_stream->recv(std::complex<float>, recv_pkt_sz, md, stream_timeout);
        _handle_recv_errors(md, rx_sample_count);
        rx_sample_count += num_recv_samps;        
        recv_ptr += 2; //iterate by 2 for complex float
    }
}

int _handle_recv_errors(uhd::rx_metadata_t m, size_t samp_count)
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
            break;
        case uhd::rx_metadata_t::ERROR_CODE_OVERFLOW:
            std::cout << m.to_pp_string() << std::endl;
            return 1;
            break;
        default:
            throw std::runtime_error(m.strerror());
            break;
    }
    return 0;
}

