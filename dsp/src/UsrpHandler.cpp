//TODO
//1. Make a USRP class that can initialize usrp, and stream data
//2. Make a DSP class that can perform FIR filter and cross correlation
//3. Look into TCO networking with boost library
//

#include "include/UsrpHandler.h"

void UsrpHandler::initilize()
{
    this->usrp = uhd::usrp::multi_usrp::make(this->ip_addr);
    this->usrp->set_clock_source(this->clock_ref);
    this->usrp->set_time_source(this->time_ref);
    this->usrp->set_rx_subdev_spec(this->subdev);
    this->usrp->set_rx_rate(this->sample_rate);
    uhd::tune_request_t tune_request(this->center_freq);
    this->usrp->set_rx_freq(tune_request);
    this->usrp->set_rx_bandwidth(this->bw);
    this->usrp->set_rx_antenna(ant);
    if(!this->use_agc)
    {
        this->usrp->set_rx_gain(this->gain);
    }
    else{
        this->usrp->set_rx_agc(true);
    }
}

//setters
void UsrpHandler::set_clock_source(std::string r)
{
    this->usrp->set_clock_source(r);
}

//getters
std::string UsrpHandler::get_clock_source()
{
    return this->clock_ref;
}
