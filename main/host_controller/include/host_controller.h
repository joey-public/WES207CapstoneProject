#ifndef HOST_CONTROLLER_H
#define HOST_CONTROLLER_H

#include <cstdint>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>

struct Sample 
{
    uint64_t client_id;
    uint64_t timestamp;
    std::vector<uint8_t> data;
};

struct Response 
{
    bool success;
    std::string message;
};

class Server 
{
public:
    Server(boost::asio::io_context& io_context);
    void start();
    void run_io_context();
    void send_control_command(uint64_t client_id, std::string command);
    void broadcast_control_command(std::string command);
    void disconnect_client(uint64_t client_id);
    std::vector<uint64_t> get_connected_clients();
    void run_localization();
    void accept_connection();
    std::thread localization_thread_;

private:
    void handle_accept(std::shared_ptr<boost::asio::ip::tcp::socket> socket,const boost::system::error_code& error);
    void read_sample(std::shared_ptr<boost::asio::ip::tcp::socket> socket);
    void handle_sample(std::shared_ptr<boost::asio::ip::tcp::socket> socket, std::shared_ptr<Sample> sample, const boost::system::error_code& error, std::size_t bytes_transferred);
    void configure_usrp(uint64_t client_id, std::string argument);
    void synchronize_pps(uint64_t client_id, std::string argument);
    void start_streaming(uint64_t client_id, std::string argument);
    void stop_streaming(uint64_t client_id, std::string argument);
    void handle_client_disconnection(uint64_t client_id);
    void io_context_run();
   

    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::map<uint64_t, std::shared_ptr<boost::asio::ip::tcp::socket>> sockets_;
    std::map<uint64_t, std::queue<Sample>> sample_buffers_;
    std::queue<Sample> localization_queue_;
    std::map<std::string, std::function<void(uint64_t, std::string)>> command_handlers_;
    uint64_t next_client_id_;
    boost::mutex sockets_mutex_;
};

#endif //HOST_CONTROLLER_H
