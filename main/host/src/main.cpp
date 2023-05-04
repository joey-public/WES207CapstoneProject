#include "host.h"

int main(int argc, char* argv[])
{
    if (argc != 3) 
    {
        std::cerr << "Usage: client <server_address> <server_port>" << std::endl;
        return 1;
    }
    boost::asio::io_context io_context;

    Client client(io_context, argv[1], argv[2]);
    client.start();

    //client.stop();
    client.control_command_thread_.join();
    client.dsp_thread_.join();

return 0;
}