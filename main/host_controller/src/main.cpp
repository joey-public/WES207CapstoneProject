#include <iostream>
#include <boost/asio.hpp>
#include "host_controller.h"
#include <boost/thread.hpp>

int main(int argc, char* argv[])
{
    try 
    {
        bool continueLocalization = true;

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
        ControlMessageCommands cmd_id;
        while(continueLocalization)
        {
            cmd_id = ControlMessageCommands::config;
            while(cmd_id > ControlMessageCommands::ControlMessageCommandInvalid && 
            cmd_id < ControlMessageCommands::powerSave)
            {
              //fetch the command string
              PacketUtils::getControlMessageString(cmd_id, command);
              
              //create control message
              ControlMessage cm;
              cm.rx_id = 0;// rx fills it and sends, so make it 0 here
              cm.ack   = 0;//rx fills it and sends, so make it 0 here
              cm.message.emplace_back(command);
              
              //create serialized message;
              std::vector<char> controlPacketBuffer;
              PacketUtils::createControlPacket(cm, controlPacketBuffer); // could be used for reading parsed message.

              //Now send
              if (command == "config" || command == "sync" 
              || command == "stream"  || command == "time")
              {
                int64_t sleep_time = 7;
                if(command == "stream")
                {
                  sleep_time = 15;
                }
                //server.broadcast_control_command(command);
                server.broadcast_control_command_async(command);
                //sleep for sometime before querying result (10sec)
                std::this_thread::sleep_for(std::chrono::seconds(sleep_time));
                //Logic could be added to read ack and re-try.
              }
              else if(command == "send")
              {
                //send and receive sequentially
                server.send_receive_sequentially(command);
                std::this_thread::sleep_for(std::chrono::seconds(15));
              }
               // Increment the command to the next enum value
              cmd_id = static_cast<ControlMessageCommands>(static_cast<int>(cmd_id) + 1);
            }
            
            std::cout <<"Host controller: Want to continue localization?Enter (yes/no) in the console when localization results are out: ";
            std::string choice;
            std::getline(std::cin, choice);
            if(choice == "no")
            {
              continueLocalization = false;
              command = "disconnect";
              // Stop streaming and disconnect clients
              for (auto& client : server.get_connected_clients()) 
              {
                std::cout << "Stopping streaming for client " << client << std::endl;
                server.send_control_command(client, command);
                server.disconnect_client(client);
              }
            }
            else if(choice == "yes")
            {
              continueLocalization = true;
            }
            else
            {
              continueLocalization = false;
              std::cout << "Wrong option entered exiting";
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


