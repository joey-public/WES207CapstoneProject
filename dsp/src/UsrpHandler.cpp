//TODO
//1. Make a USRP class that can initialize usrp, and stream data
//2. Make a DSP class that can perform FIR filter and cross correlation
//3. Look into TCO networking with boost library
//

#include "include/UsrpHandler.h"

void UsrpHandler::initilize(std::string ipaddr)
{
    this->usrp = this->init_usrp(ipaddr); 
    this->set_subdev(this->subdev);
    this->set_ant(this->ant);
    this->set_clock_ref(this->clock_ref);
    this->set_time_ref(this->time_ref);
    this->set_sample_rate(this->sample_rate);
    this->set_center_freq(this->center_freq);
    if(!this->use_agc){ this->set_gain(this->gain); }
    else{ this->usrp->set_rx_agc(true); }
    this->set_gain(this->gain);
    this->set_bw(this->bw);
}

uhd::usrp::multi_usrp::sptr UsrpHandler::init_usrp(std::string ipaddr)
{
    this->usrp = uhd::usrp::multi_usrp::make(ipaddr);
    this->set_ip_addr(ipaddr);
    return usrp;
}

//setters
void UsrpHandler::set_ip_addr(std::string ip)
{
    this->ip_addr = ip;
}
void UsrpHandler::set_subdev(std::string sd)
{
    this->subdev = sd;
    this->usrp->set_rx_subdev_spec(sd);
}
void UsrpHandler::set_ant(std::string a)
{
    this->ant = a;
    this->usrp->set_rx_antenna(a);
}
void UsrpHandler::set_clock_ref(std::string cs)
{
    this->clock_ref = cs;
    this->usrp->set_clock_source(cs);
}
void UsrpHandler::set_time_ref(std::string ts)
{
    this->time_ref = ts;
    this->usrp->set_time_source(ts);
}
void UsrpHandler::set_sample_rate(double fs)
{
    this->sample_rate = fs;
    this->usrp->set_rx_rate(fs);
}
void UsrpHandler::set_center_freq(double fc)
{
    this->center_freq = fc;
    uhd::tune_request_t tune_request(fc);
    this->usrp->set_rx_freq(fc);
}
void UsrpHandler::set_gain(double g)
{
    this->gain = g;
    this->usrp->set_rx_gain(g);
}
void UsrpHandler::set_bw(double b)
{
    this->bw = b;
    this->usrp->set_rx_bandwidth(b);
}

//getters
std::string UsrpHandler::get_ip_addr()
{
    return this->ip_addr;
}
std::string UsrpHandler::get_subdev()
{
    return this->subdev;
}
std::string UsrpHandler::get_ant()
{
    return this->ant;
}
std::string UsrpHandler::get_clock_ref()
{
    return this->clock_ref;
}
std::string UsrpHandler::get_time_ref()
{
    return this->time_ref;
}
double UsrpHandler::get_sample_rate()
{
    return this->sample_rate;
}
double UsrpHandler::get_center_freq()
{
    return this->center_freq;
}
double UsrpHandler::get_gain()
{
    return this->gain;
}
double UsrpHandler::get_bw()
{
    return bw;
}
