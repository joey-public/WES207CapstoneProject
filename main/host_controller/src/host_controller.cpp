#include "host_controller.h"
#include <iostream>
#include <thread>
#include <string>
#include <boost/bind.hpp>

Server::Server(boost::asio::io_context& io_context, const std::string& addr, const std::string& port_num): 
acceptor_(io_context), 
next_client_id_(0),
server_addr_(addr)
{
    server_port_ = std::stoi(port_num);
    #if 0
    command_handlers_["configure_usrp"] = [this](uint64_t client_id, std::string argument){return this->Server::configure_usrp(client_id, argument);};
    command_handlers_["synchronize_pps"] = [this](uint64_t client_id, std::string argument){return this->Server::synchronize_pps(client_id, argument);};
    command_handlers_["start_streaming"] = [this](uint64_t client_id, std::string argument){return this->Server::start_streaming(client_id, argument);};
    command_handlers_["stop_streaming"] = [this](uint64_t client_id, std::string argument){return this->Server::stop_streaming(client_id, argument);};
    #endif
}

void Server::start()
{
    //boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 33334);
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address_v4(server_addr_), server_port_);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();//was one

    std::cout << "Server listening on port 33334" << std::endl;
    accept_connection();
}

void Server::accept_connection() 
{
    std::cout<<"accept_connection" << std::endl;
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context_);
    acceptor_.async_accept(*socket,
        boost::bind(&Server::handle_accept, this, socket, boost::asio::placeholders::error));

    std::cout<<"accept_connection exit" << std::endl;

}

/// @brief 
void Server::run_io_context()
{
    io_context_.run();
    io_context_.restart();
}

void Server::handle_accept(std::shared_ptr<boost::asio::ip::tcp::socket> socket,const boost::system::error_code& error) 
{
    if (!error) 
    {
        std::cout << "Client connected: " << next_client_id_ << std::endl;
        {
            boost::unique_lock<boost::mutex> lock(sockets_mutex_);
            sockets_[next_client_id_] = socket;
            sample_buffers_[next_client_id_] = std::queue<Sample>();
        }
        next_client_id_++;
        read_sample(socket);
    } 
    else 
    {
        std::cout << "Error accepting connection: " << error.message() << std::endl;
    }
    accept_connection();
}

void Server::read_sample(std::shared_ptr<boost::asio::ip::tcp::socket> socket)
{
    auto sample = std::make_shared<Sample>();
    sample->client_id = 0;
    sample->timestamp = 0;
    boost::asio::async_read(*socket, boost::asio::buffer(&sample->client_id, sizeof(sample->client_id)), [this, socket, sample](const boost::system::error_code& error, std::size_t bytes_transferred) 
    {
        if (!error) 
        {
            boost::asio::async_read(*socket, boost::asio::buffer(&sample->timestamp, sizeof(sample->timestamp)), [this, socket, sample](const boost::system::error_code& error, std::size_t bytes_transferred) 
            {
                if (!error) 
                {
                    auto buffer = std::make_shared<std::vector<uint8_t>>(1024);
                    boost::asio::async_read(*socket, boost::asio::buffer(*buffer), [this, socket, sample, buffer](const boost::system::error_code& error, std::size_t bytes_transferred) 
                    {
                        if (!error) 
                        {
                            sample->data = std::vector<uint8_t>(buffer->begin(), buffer->begin() + bytes_transferred);
                            handle_sample(socket, sample, error, bytes_transferred);
                        } 
                        else 
                        {
                            handle_sample(socket, sample, error, bytes_transferred);
                        }
                    });
                } 
                else 
                {
                    handle_sample(socket, sample, error, bytes_transferred);
                }
             });
        }
        else 
        {
            handle_sample(socket, sample, error, bytes_transferred);
        }
    });

}

void Server::handle_sample(std::shared_ptr<boost::asio::ip::tcp::socket> socket, std::shared_ptr<Sample> sample, const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if (!error)
    {
        std::cout << "Sample received from client " << sample->client_id << std::endl;
        {
            boost::unique_lock<boost::mutex> lock(sockets_mutex_);
            sample_buffers_[sample->client_id].push(*sample);
        }
        localization_queue_.push(*sample);
        read_sample(socket);
    } 
    else 
    {
        handle_client_disconnection(sample->client_id);
    }
}

void Server::send_control_command(uint64_t client_id, std::string command)
{
    std::cout << "Sending control command "" << command << "" to client " << client_id << std::endl;
    boost::asio::write(*sockets_[client_id], boost::asio::buffer(command + "\n"));
}

void Server::broadcast_control_command(std::string command)
{
    std::cout << "Broadcasting control command "" << command << "" to all clients" << std::endl;
    boost::unique_lock<boost::mutex> lock(sockets_mutex_);
    for (const auto& pair : sockets_) 
    {
        boost::asio::write(*pair.second, boost::asio::buffer(command + "\n"));
    }
}

void Server::disconnect_client(uint64_t client_id)
{
    std::cout << "Disconnecting client " << client_id << std::endl;
    {
        boost::unique_lock<boost::mutex> lock(sockets_mutex_);
        sockets_[client_id]->close();
        sockets_.erase(client_id);
        sample_buffers_.erase(client_id);
    }
}

std::vector<uint64_t>Server::get_connected_clients()
{
    std::vector<uint64_t> clients;
    boost::unique_lock<boost::mutex> lock(sockets_mutex_);
    for (const auto& pair : sockets_) 
    {
        clients.push_back(pair.first);
    }
    return clients;
}

void Server::handle_client_disconnection(uint64_t client_id)
{
    std::cout << "Client " << client_id << " disconnected" << std::endl;
    disconnect_client(client_id);
}

void Server::run_localization()
{
    #if 0
    localization_thread_ = std::thread([this]()
    {
            while (true) 
            {
                Sample sample;
                //std::unique_lock<std::mutex> lock(localization_mutex_);
                sample = localization_queue_.front();
                localization_queue_.pop();
                std::cout << "Sample processed by localization thread: client " << sample.client_id << ", timestamp " << sample.timestamp << std::endl;
                // perform localization calculations here
             }
    });
    #endif

}
