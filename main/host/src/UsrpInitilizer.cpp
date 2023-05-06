#include "UsrpInitilizer.h"

UsrpInitilizer::UsrpInitilizer(std::string ip, std::string sd, std::string a, 
                           std::string cr, std::string tr, double sr, 
                           double cf, double g, double b)//constructor
{
    this->ip_addr = ip;
    this->subdev = sd;
    this->ant = a;
    this->clock_ref = cr;
    this->time_ref = tr;
    this->sample_rate = sr;
    this->center_freq = cf;
    this->gain = g;
    this->bw = b;
    
    this->initilize(this->ip_addr);    
}

void UsrpInitilizer::initilize(std::string ipaddr)
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

uhd::usrp::multi_usrp::sptr UsrpInitilizer::init_usrp(std::string ipaddr)
{
    this->usrp = uhd::usrp::multi_usrp::make("addr="+ipaddr);
    this->set_ip_addr(ipaddr);
    return usrp;
}


//setters
void UsrpInitilizer::set_ip_addr(std::string ip)
{
    this->ip_addr = ip;
}
void UsrpInitilizer::set_subdev(std::string sd)
{
    this->subdev = sd;
    this->usrp->set_rx_subdev_spec(sd);
}
void UsrpInitilizer::set_ant(std::string a)
{
    this->ant = a;
    this->usrp->set_rx_antenna(a);
}
void UsrpInitilizer::set_clock_ref(std::string cs)
{
    this->clock_ref = cs;
    this->usrp->set_clock_source(cs);
}
void UsrpInitilizer::set_time_ref(std::string ts)
{
    this->time_ref = ts;
    this->usrp->set_time_source(ts);
}
void UsrpInitilizer::set_sample_rate(double fs)
{
    this->sample_rate = fs;
    this->usrp->set_rx_rate(fs);
}
void UsrpInitilizer::set_center_freq(double fc)
{
    this->center_freq = fc;
    uhd::tune_request_t tune_request(fc);
    this->usrp->set_rx_freq(fc);
}
void UsrpInitilizer::set_gain(double g)
{
    this->gain = g;
    this->usrp->set_rx_gain(g);
}
void UsrpInitilizer::set_bw(double b)
{
    this->bw = b;
    this->usrp->set_rx_bandwidth(b);
}

//getters
std::string UsrpInitilizer::get_ip_addr()
{
    return this->ip_addr;
}
std::string UsrpInitilizer::get_subdev()
{
    return this->subdev;
}
std::string UsrpInitilizer::get_ant()
{
    return this->ant;
}
std::string UsrpInitilizer::get_clock_ref()
{
    return this->clock_ref;
}
std::string UsrpInitilizer::get_time_ref()
{
    return this->time_ref;
}
double UsrpInitilizer::get_sample_rate()
{
    return this->sample_rate;
}
double UsrpInitilizer::get_center_freq()
{
    return this->center_freq;
}
double UsrpInitilizer::get_gain()
{
    return this->gain;
}
double UsrpInitilizer::get_bw()
{
    return this->bw;
}
uhd::usrp::multi_usrp::sptr UsrpInitilizer::get_usrp()
{
    return this->usrp;
}
