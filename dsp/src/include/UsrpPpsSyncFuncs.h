#ifndef USRP_SYNC_H
#define USRP_SYNC_H

#include <iostream>
#include <uhd/usrp/multi_usrp.hpp>

//Applicaiton Note on USRP Synchronization
//https://files.ettus.com/manual_archive/release_003_001_001/manual/html/sync.html
namespace sync{

    void sync_to_pps_external(uhd::usrp::multi_usrp::sptr usrp);    
    void sync_to_pps_internal();    

}
