#ifndef USRP_RX_STREAM_H
#define USRP_RX_STREAM_H

//std includes
#include <iostream>
//uhd includes
#include <uhd/usrp/multi_usrp.hpp>

#include <complex>
#include <vector>
#include <thread>
#include <atomic>

void stream_rx_data(uhd::usrp::multi_usrp::sptr usrp);
void stream_rx_data_2(uhd::usrp::multi_usrp::sptr usrp);


int _handle_recv_errors(uhd::rx_metadata_t m, size_t samp_count);

void rx_to_file();

#endif
