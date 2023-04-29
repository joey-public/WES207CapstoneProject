#include <iostream>
#include <chrono>
#include <thread>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

// Heartbeat interval in seconds
const int HEARTBEAT_INTERVAL = 10;

// Power-saving interval in seconds
const int POWER_SAVE_INTERVAL = 60;

// TCP client class
class TcpClient
{
public:
    TcpClient(boost::asio::io_context& io_context, const std::string& host, const std::string& port)
        : io_context_(io_context), socket_(io_context), host_(host), port_(port)
    {
        // Start the heartbeat and power-saving threads
        //heartbeat_thread_ = std::thread(&TcpClient::heartbeat, this);
        //power_save_thread_ = std::thread(&TcpClient::power_save, this);
        //receive_control_messages();
    }

    ~TcpClient()
    {
        // Stop the heartbeat and power-saving threads
        running_ = false;
        //heartbeat_thread_.join();
        //power_save_thread_.join();
    }

    // Connect to the server
    void connect()
    {
        tcp::resolver resolver(io_context_);
        tcp::resolver::results_type endpoints = resolver.resolve(host_, port_);
        boost::asio::connect(socket_, endpoints);
    }

    // Send a control message to the server
    void send_control_message(const std::string& message)
    {
        boost::asio::write(socket_, boost::asio::buffer(message + "\n"));
    }

    // Handle a received control message
    virtual void handle_control_message(const std::string& message)
    {
        std::cout << "Received control message: " << message << std::endl;
    }

private:
    // Heartbeat thread
    void heartbeat()
    {
        while (running_)
        {
            // Send a heartbeat message to the server
            send_control_message("heartbeat");

            // Sleep for the heartbeat interval
            std::this_thread::sleep_for(std::chrono::seconds(HEARTBEAT_INTERVAL));
        }
    }

    // Power-saving thread
    void power_save()
    {
        while (running_)
        {
            // Send a power-saving message to the server
            send_control_message("power_save");

            // Sleep for the power-saving interval
            std::this_thread::sleep_for(std::chrono::seconds(POWER_SAVE_INTERVAL));
        }
    }

    // Receive and handle control messages from the server
    void receive_control_messages()
    {
        try
        {
            // Read control messages from the server
            while (running_)
            {
                boost::asio::streambuf buffer;
                boost::asio::read_until(socket_, buffer, '\n');
                std::string message = boost::asio::buffer_cast<const char*>(buffer.data());

                // Remove the trailing newline character
                message.erase(message.find_last_not_of("\n\r") + 1);

                // Handle the control message
                handle_control_message(message);
            }
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
    }

    boost::asio::io_context& io_context_;
    tcp::socket socket_;
    std::string host_;
    std::string port_;
    std::thread heartbeat_thread_;
    std::thread power_save_thread_;
    bool running_ = true;
};

int main(int argc, char* argv[])
{
  try
  {
    boost::asio::io_context io_context;
    TcpClient client(io_context, "localhost", "33334");
    client.connect();
    io_context.run();
     // Start receiving control messages from the server
    //std::thread receive_thread([&client]() {
      //  client.receive_control_messages();
    //});
    //start receiving control messages from the server
    //also see the possibility of threading...
    // Send some control messages to the server
      // Start receiving control messages from the server

    client.send_control_message("start_streaming");
    //client.receive_control_messages();
  }
  catch (std::exception& e)
  {
    std::cout << "Exception: " << e.what() << "\n";
  }

  return 0;
}
