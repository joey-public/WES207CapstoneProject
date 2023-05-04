#include <iostream>
#include<boost/asio.hpp>
#include<host_controller.h>
#include <boost/thread.hpp>

int main(int argc, char* argv[])
{
    try 
    {
        boost::asio::io_context io_context;
        Server server(io_context);

        //Start the server
        server.start();

        // Run the io_context object on a separate thread (lambda function)
        std::thread io_thread([&io_context]()
        {
            io_context.run();
        });

        // Wait for all clients to connect
        while (server.get_connected_clients().size() < 1) 
        {
          //std::cout << "Still waiting for more clients" << std::endl;
          std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // start the localization thread
        std::thread localization_thread(&Server::run_localization, &server);

        // handle control commands from the console
        std::string command;
        while (std::getline(std::cin, command)) 
        {
            if (command == "configure_usrp" || command == "synchronize_pps" || command == "start_streaming" || command == "stop_streaming") 
            {
                server.broadcast_control_command(command);
            }
            else if(command == "disconnect")
            {
                //wait for all connections to be closed before exiting

                // Stop streaming and disconnect clients
                for (auto& client : server.get_connected_clients()) 
                {
                  std::cout << "Stopping streaming for client " << client << std::endl;
                  server.send_control_command(client, "stop_streaming arg1 arg2");
                  server.disconnect_client(client);
                }
            }
            else
            {
              std::cout << "Unknown command \"" << command << "\"" << std::endl;
            }
        }
          // Join the localization thread
          localization_thread.join();

          //Join iocontect
          io_thread.join();

    } 
    catch (std::exception& e) 
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}


