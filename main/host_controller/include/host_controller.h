#ifndef HOST_CONTROLLER_H
#define HOST_CONTROLLER_H

#define VERSION 0.1
#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;


string read_(tcp::socket & socket);
void send_(tcp::socket & socket, const string& message);

#endif
