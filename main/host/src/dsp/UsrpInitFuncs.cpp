#include "UsrpInitFuncs.h"

namespace init {
uhd::usrp::multi_usrp::sptr gen_usrp(std::string ip, std::string sd, std::string a, 
                           std::string cs, std::string ts, double fs, 
                           double fc, double g, double b)//constructor
{
    std::cout << "Configureing USRP" << std::endl;
    std::string args = "addr="+ip;
    uhd::device_addr_t dev_addr = uhd::device_addr_t();
    std::cout << "-------------------------------------" << std::endl;
    std::cout << dev_addr.to_pp_string() << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    //create usrp object
    std::string b200_args;
    uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(b200_args);

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
}//end namespace
