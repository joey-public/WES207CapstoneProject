#include "host.h"

Client::Client(boost::asio::io_context& io_context, const std::string& server_address, const std::string& server_port)
    : io_context_(io_context),
      socket_(io_context),
      server_address_(server_address),
      server_port_(server_port),
      client_id_(0),
      is_configured_(false),
      is_synchronized_(false),
      is_streaming_(false)
{
}

void Client::start()
{
    std::cout << "Connecting to server " << server_address_ << ":" << server_port_ << "..." << std::endl;

    boost::asio::ip::tcp::resolver resolver(io_context_);
    auto endpoints = resolver.resolve(server_address_, server_port_);
    boost::asio::connect(socket_, endpoints);

    std::cout << "Connected to server." << std::endl;

    // Get assigned client ID from server
    boost::asio::streambuf response_buffer;
    boost::asio::read_until(socket_, response_buffer, '\n');
    std::istream response_stream(&response_buffer);
    response_stream >> client_id_;
    std::cout << "Assigned client ID: " << client_id_ << std::endl;

    control_command_thread_ = std::thread(&Client::control_command_handler, this);
    dsp_thread_ = std::thread(&Client::dsp_handler, this);
}

void Client::stop()
{
    std::cout << "Disconnecting from server..." << std::endl;
    socket_.close();
    std::cout << "Disconnected from server." << std::endl;
}

void Client::configure_usrp()
{
    std::cout << "Configuring USRP..." << std::endl;
    // configure USRP here
    is_configured_ = true;
    std::cout << "USRP configured." << std::endl;
}

void Client::synchronize_pps()
{
    std::cout << "Synchronizing to PPS..." << std::endl;
    // synchronize to PPS here
    is_synchronized_ = true;
    std::cout << "Synchronized to PPS." << std::endl;
}

void Client::start_streaming()
{
    std::cout << "Starting streaming..." << std::endl;
    // start streaming here
    is_streaming_ = true;
    std::cout << "Streaming started." << std::endl;
}

void Client::stop_streaming()
{
    std::cout << "Stopping streaming..." << std::endl;
    // stop streaming here
    is_streaming_ = false;

std::cout << "Streaming stopped." << std::endl;
}

void Client::send_sample(const Sample& sample)
{
    std::ostringstream sample_stream;
    sample_stream << sample.client_id << " " << sample.timestamp << " " << sample.peak << "\n";
    std::string sample_string = sample_stream.str();
    boost::asio::write(socket_, boost::asio::buffer(sample_string));
}

void Client::control_command_handler()
{
    while (true) 
    {
        boost::asio::streambuf command_buffer;
        boost::asio::read_until(socket_, command_buffer, '\n');
        std::istream command_stream(&command_buffer);
        std::string command;
        command_stream >> command;
        if (command == "configure_usrp") 
        {
            configure_usrp();
        } 
        else if (command == "synchronize_pps") 
        {
            synchronize_pps();
        } 
        else if (command == "start_streaming") 
        {
            start_streaming();
        } 
        else if (command == "stop_streaming") 
        {
            stop_streaming();
        } 
        else 
        {
            std::cout << "Invalid command received from server." << std::endl;
        }
    }
}

void Client::dsp_handler()
{   
    while (true) 
    {
        if (is_streaming_) 
        {
            // perform DSP and find peak
            Sample sample;
            sample.client_id = client_id_;
            sample.timestamp = 0.0; // replace with actual timestamp
            sample.peak = 0.0; // replace with actual peak value
            send_sample(sample);
        }
        // sleep for some time before performing next DSP
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
