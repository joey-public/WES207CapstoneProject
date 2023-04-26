#include "host_controller.h"

int main()
{
      boost::asio::io_service io_service;
//listen for new connection
      tcp::acceptor acceptor_(io_service, tcp::endpoint(tcp::v4(), 33334 ));
//socket creation 
      tcp::socket socket_(io_service);
//waiting for connection
      acceptor_.accept(socket_);
//read operation
      string message = read_(socket_);
      cout << message << endl;
//write operation
      send_(socket_, "Hello From Server!");
      cout << "Servent sent Hello message to Client!" << endl;
    return 0;
}