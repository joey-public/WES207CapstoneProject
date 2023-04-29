#include <uhd/utils/thread_priority.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/exception.hpp>
#include <uhd/types/tune_request.hpp>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <boost/thread.hpp>
#include <iostream>

#if 0
//Defines a safe wrapper that places a catch-all around main. If an exception is thrown, it prints to stderr and returns. 
int UHD_SAFE_MAIN(int argc, char *argv[]) 
{
    //Set the scheduling priority on the current thread. Same as set_thread_priority but does not throw on failure. 
    uhd::set_thread_priority_safe();
    //Create Variables
    std::string device_args("addr=192.168.11.2");
    std::string subdev("A:0");
    std::string ant("TX/RX");
    std::string ref("external");

    double rate(1e6);
    double freq(915e6);
    double gain(10);
    double bw(1e6);

    //Creating a USRP Object
    uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(device_args);

    //Setting the Motherboard Clocks
    usrp->set_clock_source(ref);

    //Selecting the Sub Device
    usrp->set_rx_subdev_spec(subdev);

    //Getting the selected Sub Device
    usrp->get_pp_string();

    //Setting the Sample Rate
    usrp->set_rx_rate(rate);

    //Getting the Sample Rate
    usrp->get_rx_rate();

    //Setting Center Frequency
    uhd::tune_request_t tune_request(freq);
    usrp->set_rx_freq(tune_request);

    //Getting the Center Frequency
     usrp->get_rx_freq();

    //Setting the RF Gain
    usrp->set_rx_gain(gain);

    //Reading the RF Gain
    usrp->get_rx_gain();

    //Setting the IF Filter Bandwidth
    usrp->set_rx_bandwidth(bw);
    
    //Getting the IF Filter Bandwidth
    usrp->get_rx_bandwidth();

    //Selecting the Antenna
    usrp->set_rx_antenna(ant);

    //Getting the Antenna
    usrp->get_rx_antenna();

    return EXIT_SUCCESS;

}
#endif
int UHD_SAFE_MAIN(int argc, char *argv[]) {
    uhd::set_thread_priority_safe();

    std::string device_args("addr=192.168.10.2");
    std::string subdev("A:0");
    std::string ant("TX/RX");
    std::string ref("external");

    double rate(1e6);
    double freq(915e6);
    double gain(10);
    double bw(1e6);

    //create a usrp device
    std::cout << std::endl;
    std::cout << boost::format("Creating the usrp device with: %s...") % device_args << std::endl;
    uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(device_args);

    // Lock mboard clocks
    std::cout << boost::format("Lock mboard clocks: %f") % ref << std::endl;
    usrp->set_clock_source(ref);
    //usrp->set_time_source("internal");

    for (size_t mboard = 0; mboard < usrp->get_num_mboards(); mboard++)
    {
        std::cout << boost::format("Lock mboard clocks get external clock  : %s") % usrp->get_clock_source(mboard) << std::endl;
        //std::cout << boost::format("Lock mboard clocks get timing source: %s") % usrp->get_time_source(mboard) << std::endl;
    }

 
    //always select the subdevice first, the channel mapping affects the other settings
    std::cout << boost::format("subdev set to: %f") % subdev << std::endl;
    usrp->set_rx_subdev_spec(subdev);
    std::cout << boost::format("Using Device: %s") % usrp->get_pp_string() << std::endl;

    //set the sample rate
    if (rate <= 0.0) {
        std::cerr << "Please specify a valid sample rate" << std::endl;
        return ~0;
    }

    // set sample rate
    std::cout << boost::format("Setting RX Rate: %f Msps...") % (rate / 1e6) << std::endl;
    usrp->set_rx_rate(rate);
    std::cout << boost::format("Actual RX Rate: %f Msps...") % (usrp->get_rx_rate() / 1e6) << std::endl << std::endl;

    // set freq
    std::cout << boost::format("Setting RX Freq: %f MHz...") % (freq / 1e6) << std::endl;
    uhd::tune_request_t tune_request(freq);
    usrp->set_rx_freq(tune_request);
    std::cout << boost::format("Actual RX Freq: %f MHz...") % (usrp->get_rx_freq() / 1e6) << std::endl << std::endl;

    // set the rf gain
    std::cout << boost::format("Setting RX Gain: %f dB...") % gain << std::endl;
    usrp->set_rx_gain(gain);
    std::cout << boost::format("Actual RX Gain: %f dB...") % usrp->get_rx_gain() << std::endl << std::endl;

    // set the IF filter bandwidth
    std::cout << boost::format("Setting RX Bandwidth: %f MHz...") % (bw / 1e6) << std::endl;
    usrp->set_rx_bandwidth(bw);
    std::cout << boost::format("Actual RX Bandwidth: %f MHz...") % (usrp->get_rx_bandwidth() / 1e6) << std::endl << std::endl;

    // set the antenna
    std::cout << boost::format("Setting RX Antenna: %s") % ant << std::endl;
    usrp->set_rx_antenna(ant);
    std::cout << boost::format("Actual RX Antenna: %s") % usrp->get_rx_antenna() << std::endl << std::endl;

    return EXIT_SUCCESS;
}
