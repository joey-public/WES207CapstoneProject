#include "include/UsrpInitFuncs.h"

uhd::usrp::multi_usrp::sptr gen_usrp(std::string ip, std::string sd, std::string a, 
                           std::string cs, std::string ts, double fs, 
                           double fc, double g, double b)//constructor
{
    //create usrp object
    uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make("addr="+ip);

    //configure usrp
    usrp->set_rx_subdev_spec(sd);
    usrp->set_rx_antenna(a);
    usrp->set_clock_source(cs);
    usrp->set_time_source(ts);
    usrp->set_rx_rate(fs);

    uhd::tune_request_t tune_request(fc);
    usrp->set_rx_freq(fc);

    usrp->set_rx_gain(g);
    usrp->set_rx_bandwidth(b);

    //return usrp
    return usrp;
}
