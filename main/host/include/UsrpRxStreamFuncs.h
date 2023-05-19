#ifndef USRP_RX_STREAM_H
#define USRP_RX_STREAM_H

//std includes
#include <iostream>
//uhd includes
#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/types/time_spec.hpp>

#include <complex>
#include <vector>

namespace rx_strm{

uhd::stream_cmd_t _gen_stream_cmd_no_time_source(size_t buff_sz);
uhd::stream_cmd_t _gen_stream_cmd_external_time_source(size_t buff_sz, uhd::time_spec_t start_time);
uhd::stream_cmd_t _gen_stream_cmd_gpsdo(size_t buff_sz);

void stream_rx_data_nsamps(uhd::usrp::multi_usrp::sptr usrp, 
                        size_t buff_sz, std::complex<float>* recv_ptr);

void stream_rx_data_continuous(uhd::usrp::multi_usrp::sptr usrp);


int _handle_recv_errors(uhd::rx_metadata_t m, size_t samp_count);

}
#endif
