#include "host_controller.h"
#include <iostream>
#include <thread>
#include <string>
#include <boost/bind.hpp>
#include <memory>

const uint32_t Server:: num_max_supported_client = 2;
Server::Server(boost::asio::io_context& io_context, const std::string& addr, const std::string& port_num): 
acceptor_(io_context), 
next_client_id_(0),
server_addr_(addr)
{
    server_port_ = std::stoi(port_num);
}

void Server::start()
{
    //boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 33334);
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address_v4(server_addr_), server_port_);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    std::cout << "Server listening on port 33334" << std::endl;
    accept_connection();
}

void Server::accept_connection() 
{
    if (next_client_id_ >= num_max_supported_client)
    {
        // Reached the desired number of clients, stop accepting new connections
        std::cout<<"Max num client connected"<< std::endl;
        return;
    }

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
        }
        //send client the client id
        boost::asio::write(*sockets_[next_client_id_], boost::asio::buffer(std::to_string(next_client_id_) + "\n"));
        next_client_id_++;
    } 
    else 
    {
        std::cout << "Error accepting connection: " << error.message() << std::endl;
    }
    accept_connection();
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
    }
}

//returns the vector of IDs of connected clients.
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
void Server::read_from_client()
{
    for (const auto& entry : sockets_)
    {
        readHeaderPacket(entry.second, entry.first);
    }
}
void Server::readHeaderPacket(std::shared_ptr<boost::asio::ip::tcp::socket> socket, uint64_t client_id)
{
     auto self = shared_from_this();//would make sure the object is alive
    
    // Allocate a buffer to store the incoming header
    std::shared_ptr<HeaderPacket> header = std::make_shared<HeaderPacket>();
    size_t header_size = PacketUtils::HEADER_PACKET_SIZE;
    std::vector<char> headerPacketBuffer(header_size);
   
   // Start the asynchronous read operation for the header
    boost::asio::async_read(*socket, boost::asio::buffer(headerPacketBuffer.data(), header_size),
        [self, socket, header, headerPacketBuffer, header_size,client_id](const boost::system::error_code& error, std::size_t bytesTransferred)
        {
            if (!error)
            {
                if (bytesTransferred == header_size)
                {
                    // Read the data packet based on the header information
                    *header = PacketUtils::parseHeaderPacket(headerPacketBuffer);
                    // Print the parsed header packet
                    std::cout << "Parsed Header Packet:" << std::endl;
                    std::cout << "Packet ID: " << header->packet_id << std::endl;
                    std::cout << "Packet Timestamp: " << header->pkt_ts << std::endl;
                    std::cout << "Packet Type: " << header->packet_type << std::endl;
                    std::cout << "Packet Length: " << header->packet_length << std::endl;

                    self->handleHeaderPacket(std::move(*header), socket, client_id); // Call handleHeaderPacket using shared pointer self
                }
                else
                {
                    // Handle the error case where the size of the received header is incorrect
                    std::cout << "Error: Received header size is incorrect" << std::endl;
                    // Perform error handling or close the socket, if necessary
                }
            }
            else
            {
                // Handle the error case where the asynchronous read operation failed
                std::cout << "Error: Failed to read header packet - " << error.message() << std::endl;
                // Perform error handling or close the socket, if necessary
            }
        });
}

void Server::handleHeaderPacket(const HeaderPacket& packet, std::shared_ptr<boost::asio::ip::tcp::socket> socket, uint64_t client_id)
{
    // Process the received header packet
    std::cout << "Received header packet ID: "<< packet.packet_id << "length:" << packet.packet_length <<"Packet type:" << packet.packet_type<<std::endl;
        

    if (packet.packet_type == PACKET_TYPE_CONTROL_MESSAGE)
    {
        //readControlMessage(socket, client_id);
    }
    else if (packet.packet_type == PACKET_TYPE_DATA)
    {
        startReadingDataPacket(socket, client_id, packet.packet_length);
    }
}

void Server::startReadingDataPacket(std::shared_ptr<boost::asio::ip::tcp::socket> socket, uint64_t client_id, size_t packet_length)
{
    auto self = shared_from_this();
    
    // Allocate a buffer to store the incoming data packet
    std::shared_ptr<DataPacket> dataPacket = std::make_shared<DataPacket>();
    std::vector<char> dataPacketSerial(packet_length);

    // Start the asynchronous read operation for the data packet
    boost::asio::async_read(*socket, boost::asio::buffer(dataPacketSerial.data(), packet_length),
        [self, socket, dataPacket, dataPacketSerial, packet_length, client_id](const boost::system::error_code& error, std::size_t bytesTransferred) 
        {
            if (!error && bytesTransferred == packet_length) 
            {
                PacketUtils::parseDataPacket(dataPacketSerial, *dataPacket);
                // Handle the received data packet
                self->handleDataPacket(client_id, std::move(*dataPacket));

                // Initiate the next read operation for this client
                self->readHeaderPacket(socket, client_id);
            }
        });
}

void Server::handleDataPacket(uint64_t client_id, const DataPacket& packet)
{
    // Process the received data packet
    std::cout << "Received data packet: " << std::endl;
     // Print the parsed data packet
    std::cout << "Parsed Data Packet:"  << std::endl;
    std::cout << "Receiver ID: "        << packet.rx_id << std::endl;
    std::cout << "GPS Data latitude: "  << packet.latitude << std::endl;
    std::cout << "GPS Data longitude:"  << packet.longitude << std::endl;
    std::cout << "GPS Data altitude: "  << packet.altitude << std::endl;
    std::cout << "Total peak samples: " << packet.numTimeSamples <<std::endl;


    uint8_t receiverId  = packet.rx_id;
    double latitude     = packet.latitude;
    double longitude    = packet.longitude;
    double altitude     = packet.altitude;


    // Store the buffer in memory if required ...
    // Lock the mutex before accessing the client_buffers_ map
    std::lock_guard<boost::mutex> lock(client_buffers_mutex_);
    // Store the data packet in the client's buffer
    client_buffers_[client_id].push(packet);

    //Store TOA to TOA Queue
    std::lock_guard<boost::mutex> lockq(localization_queue_mutex_);
    localization_queue_[client_id].emplace(packet.peak_timestamps->begin(), packet.peak_timestamps->end());

}
#if 0
void readControlMessage(std::shared_ptr<boost::asio::ip::tcp::socket> socket, uint64_t client_id)
{

}
#endif