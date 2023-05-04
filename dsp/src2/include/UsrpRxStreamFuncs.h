#ifndef USRP_RX_STREAM_H
#define USRP_RX_STREAM_H

//std includes
#include <iostream>
//uhd includes
#include <uhd/usrp/multi_usrp.hpp>

void stream_rx_data(uhd::usrp::multi_usrp::sptr usrp);

void rx_to_file();

#endif
