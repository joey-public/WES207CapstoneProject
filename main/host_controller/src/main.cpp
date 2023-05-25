#include <iostream>
#include <boost/asio.hpp>
#include "host_controller.h"
#include <boost/thread.hpp>

int main(int argc, char* argv[])
{
    try 
    {
        std::atomic<bool> is_receive_active(false); 
        if (argc != 3) 
        {
          std::cerr << "Usage: ./host_controller <server_address> <server_port>" << std::endl;
          return 1;
        }
        boost::asio::io_context io_context;
        Server server(io_context, argv[1], argv[2]);

        //Start the server
        server.start();

        // Run the io_context object on a separate thread (lambda function)
        std::thread io_thread([&io_context]()
        {
            io_context.run();
        });

         #if 0
        /** Would be useful when all clients are sending data*/
        std::vector<std::thread> threads;
        for (std::size_t i = 0; i < std::thread::hardware_concurrency(); ++i)
        {
            threads.emplace_back([&io_context]() 
            { io_context.run(); 
            
            });
        }
        #endif

        // Wait for all clients to connect
        while (server.get_connected_clients().size() < Server::num_max_supported_client)
        {
          //std::cout << "Still waiting for more clients" << std::endl;
          std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // start the localization thread
        std::thread localization_thread(&Server::run_localization, &server);

        // handle control commands from the console
        std::string command;
        while (false == is_receive_active.load() && std::getline(std::cin, command)) 
        {
            if (command == "config" || command == "sync" 
            || command == "stream" || command == "stop"
            || command == "time")
            {
                server.broadcast_control_command(command);
            }
            else if(command == "send")
            {
              //send and receive sequentially
              server.send_receive_sequentially(command);
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
            #if 0
            //receive data
            if(command == "send")
            {
              std::cout<<"Host controller going to read samples" <<std::endl;
              is_receive_active.store(true);
              server.read_from_client();
            }
            #endif
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


