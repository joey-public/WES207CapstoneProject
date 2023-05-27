#ifndef __HOST_H__
#define __HOST_H__

#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include "UsrpInitilizer.h"
#include <atomic>

#include <boost/asio/ip/tcp.hpp>
#include <fstream>
#include <istream>
#include "PacketUtils.h"

#include "debug.h"

#include "UsrpRxStreamFuncs.h"
#include "UtilFuncs.h"
#include "ProcessingFuncs.h"
#include "TypeDefs.h"
#include "Settings.h"

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
    void synchronize_gps();
    void start_streaming();
    void show_time();

    void stop_streaming();
    void parse_gpgga(std::string& gps_msg);
    void parse_gprmc(std::string& gps_msg);

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
    double longitude_;
    double latitude_;
    double altitude_;
    bool is_configured_;
    bool is_synchronized_;
    std::atomic<bool> is_streaming_;
    std::condition_variable conditionVariable_host; //to block thread and wait for event.
    std::mutex mutex_host;
    UsrpInitilizer* usrp_handler = NULL;

    void recv_to_file(void);
    std::vector<double> peak_timestamp_;
    std::vector<RX_DTYPE> pulse_data_;
    uint64_t stream_seq_id;
    uint64_t stream_pkt_id;
    void send_dsp_data();
    void send_dsp_data_sequentially();
    std::atomic<bool> is_send_command_active;
    void create_header_data_packet(std::vector<char>& header_packet, std::vector<char>& data_packet);
    double rx_stream_start_time_; 
};

#endif//__HOST_H__
