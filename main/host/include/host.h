#ifndef __HOST_H__
#define __HOST_H__

#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include "UsrpInitilizer.h"

struct Sample 
{
    int client_id;
    double timestamp;
    double peak;
};

class Client 
{
public:
    Client(boost::asio::io_context& io_context, 
            const std::string& server_address, 
            const std::string& server_port, 
            const std::string& usrp_ip);

    void start();
    void stop();
    void configure_usrp();
    void synchronize_pps();
    void start_streaming();
    void stop_streaming();
    void send_sample(const Sample& sample);
    std::thread control_command_thread_;
    std::thread dsp_thread_;

private:
    void control_command_handler();
    void dsp_handler();

    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::socket socket_;
    std::string server_address_;
    std::string server_port_;
    std::string usrp_ip_;
    int client_id_;
    bool is_configured_;
    bool is_synchronized_;
    bool is_streaming_;
    UsrpInitilizer* usrp_handler = NULL;
    void recv_to_file(void);
};

#endif//__HOST_H__