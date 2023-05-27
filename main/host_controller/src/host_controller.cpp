#include "host_controller.h"

const uint32_t Server:: num_max_supported_client = 1;

Server::Server(boost::asio::io_context& io_context, const std::string& addr, const std::string& port_num): 
acceptor_(io_context), 
next_client_id_(0),
server_addr_(addr),
numClientsDataReceived_(0),
start_localization_(false)
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
    std::cout << "Broadcasting control command:" << command << " to all clients" << std::endl;
    boost::unique_lock<boost::mutex> lock(sockets_mutex_);
    for (const auto& pair : sockets_) 
    {
        try 
        {
            boost::asio::write(*pair.second, boost::asio::buffer(command + "\n"));
        } 
        catch (const boost::system::system_error& e) 
        {
            std::cerr << "Error while sending command to client: " << e.what() << std::endl;
            // Handle the error, such as closing the socket or taking appropriate action
        }
    }
}

void Server::broadcast_control_command_async(std::string command)
{
    std::cout << "Broadcasting control command asynchronously: " << command << " to all clients" << std::endl;

    // Create a shared_ptr to the command string to ensure its availability during the asynchronous operation
    auto commandPtr = std::make_shared<std::string>(command + "\n");

    // Keep track of the number of asynchronous writes completed
    std::size_t completedWrites = 0;

    std::mutex completedWritesMutex;  // Mutex to protect the completedWrites counter

    // Define the callback function to be called when each asynchronous write operation completes
    auto writeHandler = [this, commandPtr, &completedWrites, &completedWritesMutex](const boost::system::error_code& error, std::size_t bytesTransferred) 
    {
        if (error) 
        {
            std::cerr << "Error while sending command to client: " << error.message() << std::endl;
            // Handle the error, such as closing the socket or taking appropriate action
        }

        // Check if all asynchronous writes have completed
        std::lock_guard<std::mutex> lock(completedWritesMutex);
        ++completedWrites;
        
        if (completedWrites == sockets_.size()) 
        {
            // All writes completed, do any necessary post-processing
            std::cout << "Async broadcast completed" << std::endl;
        }
    };

    // Iterate over the sockets using asynchronous writes
    for (const auto& pair : sockets_) 
    {
        try 
        {
            boost::asio::async_write(*pair.second, boost::asio::buffer(*commandPtr), writeHandler);
        } 
        catch (const boost::system::system_error& e) 
        {
            std::cerr << "Error while initiating write to client: " << e.what() << std::endl;
            // Handle the error, such as closing the socket or taking appropriate action
        }
    }
}

void Server::print_async_broadcast_completed()
{
    std::cout << "Async broadcast completed" << std::endl;
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
    while(true)
    {
        std::unique_lock <std::mutex> lock(loc_th_mutex_);
        cv_loc_wait.wait(lock,[this] {return start_localization_.load();});

        // Start processing the received data
        std::cout << "Localization Thread started processing:" <<std::endl;

	 //std::vector<double> ToAs =  localization_queue_[0].front();
	 //localization_queue_[0].pop();
        
        //double ToAs[4];
        
        //TEST DATA
        /*
        std::vector<double> testVec1 = {0.1, 0.2, 0.3, 0.4, 0.5};
        std::vector<double> testVec2 = {0.11, 0.22, 0.33, 0.44, 0.55};
        
        localization_queue_[0].push(testVec1);
        localization_queue_[1].push(testVec2);
        */
        //END TEST DATA
        
        std::vector<double> r0_ToAs = localization_queue_[0].front();
        std::vector<double> r1_ToAs = localization_queue_[1].front();
        
        //until we get 4 receivers working, repeat times to test
        std::vector<double> r2_ToAs = localization_queue_[0].front();
        std::vector<double> r3_ToAs = localization_queue_[1].front();
        
        localization_queue_[0].pop();
        localization_queue_[1].pop();
        
        //check max num element of ToA.
        /*
        for(int i = 0; i < num_max_supported_client; i++) 
        {
        	ToAs[i] = localization_queue_[i].front();
        	localization_queue_[i].pop();
        	if(i > 3)
        	{
        		std::cout << "No localization function to support more than 4 clients. 				Using first 4 only" << std::endl;
        		break;
        	}
        }
        */
        //loop and feed value to the localization function
        for(int i = 0; i < r0_ToAs.size(); i++)
        {
        	double tmax = std::max({r0_ToAs.at(i), r1_ToAs.at(i), r2_ToAs.at(i), r3_ToAs.at(i)});
        	double tmin = std::max({r0_ToAs.at(i), r1_ToAs.at(i), r2_ToAs.at(i), r3_ToAs.at(i)});
        	
        	//if time difference is too large, something went wrong and data will be meaningless
        	//so only perform localization if time difference is small
        	if(tmax - tmin < 0.5)
        	{
        	std::cout << "Starting localization. Inputs: " << std::endl << r0_ToAs.at(i) << std::endl << r1_ToAs.at(i) << std::endl << r2_ToAs.at(i) << std::endl << r3_ToAs.at(i) << std::endl;
        	Eigen::Vector3d loc_est = Localization_4Receivers_2D(r0_ToAs.at(i), r1_ToAs.at(i), r2_ToAs.at(i), r3_ToAs.at(i));
        	std::cout << "Estimated Location: " << loc_est << std::endl;
        	}

        }
        
        
        //finished processing,
        start_localization_.store(false);
    }
}

void Server::read_from_client()
{
    numClientsDataReceived_.store(0);
    start_localization_.store(false);

    for (const auto& entry : sockets_)
    {
        readHeaderPacket(entry.second, entry.first);
    }
}

void Server::readHeaderPacket(std::shared_ptr<boost::asio::ip::tcp::socket> socket, uint64_t client_id)
{
     TRACE_ENTER;

     auto self(shared_from_this());//would make sure the object is alive

    // Allocate a buffer to store the incoming header
    std::shared_ptr<HeaderPacket> header = std::make_shared<HeaderPacket>();
    size_t header_size = PacketUtils::HEADER_PACKET_SIZE;
    std::vector<char> headerPacketBuffer(header_size);
    std::cout << "Read Header Packet: "<<header_size <<std::endl;
   
   try
   {
        std::cout <<"Socket address:" << socket;
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
#ifdef DEBUG
                        // Print the parsed header packet
                        std::cout << "Parsed Header Packet:" << std::endl;
                        std::cout << "Packet ID: " << header->packet_id << std::endl;
                        std::cout << "Packet Timestamp: " << header->pkt_ts << std::endl;
                        std::cout << "Packet Type: " << header->packet_type << std::endl;
                        std::cout << "Packet Length: " << header->packet_length << std::endl;
#endif
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
   catch(const boost::system::system_error& e)
   {
       std::cerr << "Send error: " << e.what() << std::endl;;
   }
   
}

void Server::handleHeaderPacket(const HeaderPacket& packet, std::shared_ptr<boost::asio::ip::tcp::socket> socket, uint64_t client_id)
{
    TRACE_ENTER;
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
    TRACE_ENTER;
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


    std::cout <<"Packet timestamps" <<std::endl;
    for(auto ts:packet.peak_timestamps)
    {
        std::cout << " " << ts;
    }
    std::cout << std::endl;

    std::cout << "Waveform Samples:";
    for (std::complex<short> sample : packet.waveformSamples) 
    {
        std::cout << " " << sample;
    }


    uint8_t receiverId  = packet.rx_id;
    double latitude     = packet.latitude;
    double longitude    = packet.longitude;
    double altitude     = packet.altitude;


    // Store the buffer in memory if required ...
    // Lock the mutex before accessing the client_buffers_ map
    //std::lock_guard<boost::mutex> lock(client_buffers_mutex_);
    // Store the data packet in the client's buffer
    client_buffers_[client_id].push(packet);

    //Store TOA to TOA Queue
    //std::lock_guard<boost::mutex> lockq(localization_queue_mutex_);
    localization_queue_[client_id].emplace(packet.peak_timestamps.begin(), packet.peak_timestamps.end());

    //increase the count
    int numClientsDataReceived = numClientsDataReceived_.fetch_add(1);
    std::cout << "Total clients that have queue the data = " << numClientsDataReceived;

     // Check if all clients have sent their data
    if (numClientsDataReceived == num_max_supported_client) 
    {
        // Notify the localization thread to start processing
        //std::unique_lock <std::mutex> lock(loc_th_mutex_);
        start_localization_.store(true);
        cv_loc_wait.notify_one();
    }

}

bool Server::is_data_read_complete()
{
    bool retval = false;
    int num_client = numClientsDataReceived_.load();
    if (num_client == num_max_supported_client) 
    {
        retval = true;
    }
    return retval;
}

void Server::send_receive_sequentially(std::string& command)
{
    TRACE_ENTER;
    boost::unique_lock<boost::mutex> lock(sockets_mutex_);

    for (const auto& pair : sockets_) 
    {
        std::cout << "Send control command: " << command << " to client: " << pair.first << std::endl;
        
        try 
        {
            boost::asio::write(*pair.second, boost::asio::buffer(command + "\n"));
        } 
        catch (const boost::system::system_error& e) 
        {
            std::cerr << "Error while sending command to client: " << e.what() << std::endl;
            // Handle the error, such as closing the socket or taking appropriate action
            continue;  // Skip receiving from this client if sending the command failed
        }
        
        // Now receive the header packet from the same client
        std::cout << "Receive header packet from client: " << pair.first << std::endl;
        std::shared_ptr<HeaderPacket> header = std::make_shared<HeaderPacket>();
        std::vector<char> headerPacketBuffer(PacketUtils::HEADER_PACKET_SIZE);

        try
        {
            std::size_t bytesRead = boost::asio::read(*pair.second, boost::asio::buffer(headerPacketBuffer));

            if (bytesRead == PacketUtils::HEADER_PACKET_SIZE)
            {
                *header = PacketUtils::parseHeaderPacket(headerPacketBuffer);
#ifdef DEBUG
                // Print the parsed header packet
                std::cout << "Parsed Header Packet:" << std::endl;
                std::cout << "Packet ID: " << header->packet_id << std::endl;
                std::cout << "Packet Timestamp: " << header->pkt_ts << std::endl;
                std::cout << "Packet Type: " << header->packet_type << std::endl;
                std::cout << "Packet Length: " << header->packet_length << std::endl;
#endif
            }
            else
            {
                std::cerr << "Error: Received header size is incorrect" << std::endl;
                // Handle the error case where the size of the received header is incorrect
                continue;  // Skip receiving data packet from this client
            }
        }
        catch (const boost::system::system_error& e)
        {
            std::cerr << "Error while receiving header packet from client: " << pair.first << " - " << e.what() << std::endl;
            continue;  // Skip receiving data packet from this client
        }
        
        // Now receive the data packet from the same client
        std::cout << "Receive data packet from client: " << pair.first << std::endl;
        DataPacket dataPacket;
        std::size_t packetSize = header->packet_length;
        std::vector<char> dataPacketBuffer(packetSize);

        try
        {
            std::size_t bytesRead = boost::asio::read(*pair.second, boost::asio::buffer(dataPacketBuffer));

            if (bytesRead == packetSize)
            {
                PacketUtils::parseDataPacket(dataPacketBuffer, dataPacket);

                // Handle the received data packet
                handleReceivedData(pair.first, *header, dataPacket);
            }
            else
            {
                std::cerr << "Error: Received data packet size is incorrect" << std::endl;
                // Handle the error case where the size of the received data packet is incorrect
            }
        }
        catch (const boost::system::system_error& e)
        {
            std::cerr << "Error while receiving data packet from client: " << pair.first << " - " << e.what() << std::endl;
        }
        std::cout << "Recieved Data Packet: " << std::endl;
        std::cout << "\trx_id: " << dataPacket.rx_id << std::endl;
        std::cout << "\tlong: " << dataPacket.longitude << std::endl;
        std::cout << "\tlat: " << dataPacket.latitude << std::endl;
        std::cout << "\talt: " << dataPacket.altitude << std::endl;
        std::cout << "\tNum Time Stamps: " << dataPacket.numTimeSamples << std::endl;
        std::cout << "\tpeak timestamps: " << dataPacket.peak_timestamps[0] << std::endl;
        std::cout << "\twaveform samples len: " << dataPacket.waveformSamples.size() << std::endl;
        std::cout << "\tSaving Pulse data to txt file\n";
        //std::string data_file_path = "./pulse_data.txt"; 
        //util::save_complex_vec_to_file(dataPacket.waveformSamples, data_file_path);
        sett::update_settings_from_file();
        util::save_and_plot_data(dataPacket.waveformSamples, 
                                 sett::pulse_data_path, 
                                 sett::save_pulse_data, 
                                 sett::plot_pulse_data, 
                                 sett::usrp_sample_rate);
    }

    start_localization_.store(true);
    std::unique_lock <std::mutex> lockth(loc_th_mutex_);
    cv_loc_wait.notify_one();

    TRACE_EXIT;
}

void Server::handleReceivedData(uint64_t client_id, const HeaderPacket& header, const DataPacket& packet)
{
#ifdef DEBUG
    // Process the received header and data packets
    std::cout << "Received data from client " << client_id << std::endl;
    std::cout << "Parsed Header Packet:" << std::endl;
    std::cout << "Packet ID: " << header.packet_id << std::endl;
    std::cout << "Packet Timestamp: " << header.pkt_ts << std::endl;
    std::cout << "Packet Type: " << header.packet_type << std::endl;
    std::cout << "Packet Length: " << header.packet_length << std::endl;
    std::cout << "Parsed Data Packet:" << std::endl;
    std::cout << "Receiver ID: " << packet.rx_id << std::endl;
    std::cout << "GPS Data latitude: " << packet.latitude << std::endl;
    std::cout << "GPS Data longitude:" << packet.longitude << std::endl;
    std::cout << "GPS Data altitude: " << packet.altitude << std::endl;
    std::cout << "Total peak samples: " << packet.numTimeSamples << std::endl;
    for(auto ts: packet.peak_timestamps)
    {
        std::cout << "Timesample: " << ts <<std::endl;
    }

    for(auto wv: packet.waveformSamples)
    {
        std::cout << "Waveform: " << wv <<std::endl;
    }
#endif
    //std::lock_guard<boost::mutex> lock(client_buffers_mutex_);
    // Store the data packet in the client's buffer
    client_buffers_[client_id].push(packet);

    //Store TOA to TOA Queue
    //std::lock_guard<boost::mutex> lockq(localization_queue_mutex_);
    localization_queue_[client_id].push(packet.peak_timestamps);

}


#if 0
void readControlMessage(std::shared_ptr<boost::asio::ip::tcp::socket> socket, uint64_t client_id)
{

}
#endif
