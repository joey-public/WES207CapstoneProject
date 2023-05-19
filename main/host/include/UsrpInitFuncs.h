#ifndef USRP_INIT_H
#define USRP_INIT_H

//uhd includes
#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/types/tune_request.hpp>

namespace init{
uhd::usrp::multi_usrp::sptr gen_usrp(std::string ip, std::string sd, std::string a, 
                                     std::string cr, std::string tr, double sr, 
                                     double cf, double g, double b);
}
#endif
