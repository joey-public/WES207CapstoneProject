#include "host.h"

#include "TestNoNet.h"

#define RUN_TEST

int main(int argc, char* argv[])
{
#ifdef RUN_TEST
    test();
#endif

#ifndef RUN_TEST
    if (argc != 4) 
    {
        std::cerr << "Usage: ./host <server_address> <server_port> <usrp_ip>" << std::endl;
        return 1;
    }
    boost::asio::io_context io_context;

    Client client(io_context, argv[1], argv[2], argv[3]);
    client.start();

    //client.stop();
    client.control_command_thread_.join();
    client.dsp_thread_.join();
#endif

return 0;
}
