#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

int main() {
     boost::asio::io_service io_service;
     //tcp::socket socket(io_service);
     
     // Creating a resolver's query.
     boost::asio::ip::tcp::resolver::query resolver_query("sam-server",
                                                   "3090",
                                                   boost::asio::ip::tcp::resolver::query::numeric_service);

     // Creating a resolver.
     boost::asio::ip::tcp::resolver resolver(io_service);
     
     // Step 2. Resolving a DNS name.
     boost::asio::ip::tcp::resolver::iterator it =
      resolver.resolve(resolver_query);
   
     // Step 3. Creating a socket.
     boost::asio::ip::tcp::socket sock(io_service);
     
     // Step 4. asio::connect() method iterates over
     // each endpoint until successfully connects to one
     // of them. It will throw an exception if it fails
     // to connect to all the endpoints or if other
     // error occurs.
     boost::asio::connect(sock, it);

     // connecting
     //socket.connect( tcp::endpoint( boost::asio::ip::address::from_string("172.22.0.2"), 3090 ));
     
     const string msg = "THIS FUCKING WORKS";
     boost::system::error_code error;
     //sending
     boost::asio::write( sock, boost::asio::buffer(msg), error );
     if( !error ) {
        cout << "Client sent hello message!" << endl;
     }
     else {
        cout << "send failed: " << error.message() << endl;
     }
     return 0;
}
