#include "UsrpRxStreamFuncs.h"

#define DEBUG 
//#define DEBUG_VERBOSE

namespace rx_strm{

uhd::stream_cmd_t _gen_stream_cmd_no_time_source(size_t buff_sz)
{
    uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_NUM_SAMPS_AND_DONE);
    stream_cmd.num_samps = buff_sz;//set number of samples to recv
    stream_cmd.stream_now = true;//don't stream right now
    return stream_cmd;
}

uhd::stream_cmd_t _gen_stream_cmd_external_time_source(size_t buff_sz, uhd::time_spec_t start_time)
{
    uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_NUM_SAMPS_AND_DONE);
    stream_cmd.num_samps  = buff_sz;
    stream_cmd.stream_now = false;
    stream_cmd.time_spec  = uhd::time_spec_t(start_time);
    return stream_cmd;
}

uhd::stream_cmd_t _gen_stream_cmd_gpsdo(size_t buff_sz)
{
    std::cout << "only works with B200..." << std::endl;
    return _gen_stream_cmd_no_time_source(buff_sz);
}

uhd::time_spec_t stream_rx_data_nsamps(uhd::usrp::multi_usrp::sptr usrp, 
                        size_t buff_sz, RX_DTYPE* recv_ptr, 
                        std::string cpu_fmt, std::string wire_fmt)
{

    //create stream args
    uhd::stream_args_t stream_args(cpu_fmt, wire_fmt);
    uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(stream_args);
    size_t recv_pkt_sz = rx_stream->get_max_num_samps();
    float recv_pkt_dt = recv_pkt_sz / usrp->get_rx_rate();

    //print some stats
    std::cout << "\tCollecting " << buff_sz << " samples" << std::endl;
    std::cout << "\tcpu_fmt = " << cpu_fmt << std::endl;
    std::cout << "\twire_fmt = " << wire_fmt << std::endl;
    std::cout << "\tRX Buff Size: " << std::to_string(recv_pkt_sz) << std::endl;
    std::cout << "\tpkt dt = " << std::to_string(recv_pkt_sz)
                                << " samples / " 
                                << std::to_string(usrp->get_rx_rate() / 1e6)  
                                << " Mhz = " 
                                << std::to_string((recv_pkt_sz * 1e6) / usrp->get_rx_rate())
                                << " us" << std::endl;

    //initilize streaming metadata
    uhd::rx_metadata_t md;

    //initilaze other useful variables
    float stream_timeout = 3.0;
    size_t num_recv_samps = 0;
    size_t rx_sample_count = 0;
    size_t total_samples = 0;
    int err = 0;

    //configure stream cmds based on uhd time_source
//     uhd::time_spec_t start_time = usrp->get_time_now().get_real_secs() + 0.2; 
    uhd::time_spec_t start_time = usrp->get_mboard_sensor("gps_time").to_real() + 1.0; 
    std::cout << "\tTelling stream to start at: " << 
                 "\n\t\tReal Secs: " << start_time.get_real_secs() <<
                 "\n\t\tFull Secs: " << start_time.get_full_secs() <<
                 "\n\t\tFrac Secs: " << start_time.get_frac_secs() << std::endl;

    uhd::stream_cmd_t stream_cmd = _gen_stream_cmd_external_time_source(buff_sz, start_time);
    rx_stream->issue_stream_cmd(stream_cmd);
    
    //stream the data into the passed buffer
    while(total_samples < buff_sz)
    { 
        num_recv_samps = rx_stream->recv(recv_ptr, recv_pkt_sz, md, stream_timeout, true);
        err = _handle_recv_errors(md, rx_sample_count);
        if (err==1)
        {
            std::cout << "\tTry streaming again later..";
            return uhd::time_spec_t();
;
        }
        rx_sample_count += num_recv_samps;        
        recv_ptr += num_recv_samps; 
        total_samples += num_recv_samps;
    }
    if (md.has_time_spec)
    {
        std::cout << "\tStream Actually Started at: " << 
                 "\n\t\tReal Secs: " << md.time_spec.get_real_secs() <<
                 "\n\t\tFull Secs: " << md.time_spec.get_full_secs() <<
                 "\n\t\tFrac Secs: " << md.time_spec.get_frac_secs() << std::endl;
        return md.time_spec;
    }
    else
    {
        std::cout << "No md timespec found returning 0";
        return uhd::time_spec_t();
    }
}

//currently NOP
void stream_rx_data_continuous(uhd::usrp::multi_usrp::sptr usrp)
{
    std::cout << "Start Streaming Data..." << std::endl;

    std::string cpu_fmt = "fc32";
    std::string wire_fmt = "sc16";
    std::cout << "probing...0" << std::endl;

    //create stream args
    uhd::stream_args_t stream_args(cpu_fmt, wire_fmt);
    std::cout << "probing...1" << std::endl;
    uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(stream_args);
    std::cout << "probing...2" << std::endl;
    size_t recv_pkt_sz = rx_stream->get_max_num_samps();
    std::cout << "probing...3" << std::endl;
    float recv_pkt_dt = recv_pkt_sz / usrp->get_rx_rate();
    std::cout << "probing...4" << std::endl;

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
    std::vector<RX_DTYPE> recv_buff_0(recv_buff_sz);
    std::vector<RX_DTYPE> recv_buff_1(recv_buff_sz);
    RX_DTYPE* recv_ptr = &recv_buff_0.front(); 
    bool using_buff_0 = true;

    //initilize streaming metadata
    uhd::rx_metadata_t md;
    float stream_timeout = 3.0;
    //configure stream cmds
    rx_stream->issue_stream_cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);
    
    size_t num_recv_samps = 0;
    size_t rx_sample_count = 0;
    size_t total_samples = 0;
#ifdef DEBUG
    int swap_count = 0;
#endif
#ifdef DEBUG_VERBOSE
    int loop_ctr = 0;
#endif
    while(total_samples < 10*recv_buff_sz)
    { 
        num_recv_samps = rx_stream->recv(recv_ptr, recv_pkt_sz, md, stream_timeout);
        _handle_recv_errors(md, rx_sample_count);
        rx_sample_count += num_recv_samps;        
        recv_ptr += num_recv_samps; 
        total_samples += num_recv_samps;
#ifdef DEBUG_VERBOSE
        loop_ctr += 1;
        std::cout << loop_ctr << ": Loop number " << std::endl;
        std::cout << "\trecv_ptr: " << recv_ptr << std::endl;
        std::cout << "\tBuff 0 back: " << &recv_buff_0.back() <<  std::endl;
        bool check = recv_ptr >= &recv_buff_0.back();
        std::cout << "\trecv_ptr >= back ?: " << check << std::endl;
        std::cout << "\ttotal samps: " << rx_sample_count << std::endl;
        std::cout << "\trecv samps: " << num_recv_samps << std::endl;
        std::cout << "\tbuff sz: " << recv_buff_sz << std::endl;
#endif
        if(rx_sample_count >= recv_buff_sz)
        {
            rx_sample_count = 0;
            if(using_buff_0)
            {
#ifdef DEBUG
                std::cout << swap_count <<": buff_0 is full" << std::endl;
#endif
                recv_ptr = &recv_buff_1.front();
                using_buff_0 = false;
            }
            else
            {
#ifdef DEBUG
                std::cout << swap_count <<": buff_1 is full" << std::endl;
#endif
                recv_ptr = &recv_buff_0.front();
                using_buff_0 = true;
            }
#ifdef DEBUG
            swap_count +=1;
#endif
        }
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
            std::cout << "RX Count: " << std::to_string(samp_count) << std::endl;
            std::cout << m.to_pp_string() << std::endl;
            return 1;
//            throw std::runtime_error("ERROR_CODE_TIMEOUT");
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

}//end namespace
