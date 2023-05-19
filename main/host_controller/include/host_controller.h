#ifndef HOST_CONTROLLER_H
#define HOST_CONTROLLER_H

#include <cstdint>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include "PacketUtils.h"
#include <atomic>

class Server:public std::enable_shared_from_this<Server>
{
public:
    Server(boost::asio::io_context& io_context, const std::string& addr, const std::string& port_num);
    void start();
    void run_io_context();
    void send_control_command(uint64_t client_id, std::string command);
    void broadcast_control_command(std::string command);
    void disconnect_client(uint64_t client_id);
    std::vector<uint64_t> get_connected_clients();
    void run_localization();
    void accept_connection();
    std::thread localization_thread_;
    static const uint32_t num_max_supported_client;
    void read_from_client();

private:
    void handle_accept(std::shared_ptr<boost::asio::ip::tcp::socket> socket,const boost::system::error_code& error);
    void configure_usrp(uint64_t client_id, std::string argument);
    void synchronize_pps(uint64_t client_id, std::string argument);
    void start_streaming(uint64_t client_id, std::string argument);
    void stop_streaming(uint64_t client_id, std::string argument);
    void handle_client_disconnection(uint64_t client_id);
    void io_context_run();
    void handleHeaderPacket(const HeaderPacket& packet);
    void readHeaderPacket(std::shared_ptr<boost::asio::ip::tcp::socket> socket, uint64_t client_id);
    void handleHeaderPacket(const HeaderPacket& packet, std::shared_ptr<boost::asio::ip::tcp::socket> socket, uint64_t client_id);
    void startReadingDataPacket(std::shared_ptr<boost::asio::ip::tcp::socket> socket, uint64_t client_id, size_t packet_length);
    void handleDataPacket(uint64_t, const DataPacket& packet);
    //void readControlMessage(std::shared_ptr<boost::asio::ip::tcp::socket> socket, uint64_t client_id);
   

    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::map<uint64_t, std::shared_ptr<boost::asio::ip::tcp::socket>> sockets_;
    std::map<uint64_t, std::queue<DataPacket>> client_buffers_;
    boost::mutex client_buffers_mutex_;
    std::map<uint64_t, std::queue<std::vector<double>>> localization_queue_;
    boost::mutex localization_queue_mutex_;
    std::map<std::string, std::function<void(uint64_t, std::string)>> command_handlers_;
    uint64_t next_client_id_;
    boost::mutex sockets_mutex_;
    std::string server_addr_;
    uint32_t server_port_;
    std::mutex loc_th_mutex_;
    std::atomic<bool> start_localization_;
    std::condition_variable cv_loc_wait;
    std::atomic<int> numClientsDataReceived_;

};

#endif //HOST_CONTROLLER_H
