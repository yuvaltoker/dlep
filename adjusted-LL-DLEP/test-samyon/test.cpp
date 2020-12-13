#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

int main() {
     boost::asio::io_service io_service;
     tcp::socket socket(io_service);
     // connecting
     socket.connect( tcp::endpoint( boost::asio::ip::address::from_string("172.22.0.2"), 3090 ));
     
     const string msg = "THIS FUCKING WORKS";
     boost::system::error_code error;
     //sending
     boost::asio::write( socket, boost::asio::buffer(msg), error );
     if( !error ) {
        cout << "Client sent hello message!" << endl;
     }
     else {
        cout << "send failed: " << error.message() << endl;
     }
     return 0;
}
