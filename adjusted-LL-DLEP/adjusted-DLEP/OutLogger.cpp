#include "OutLoggerEnviromentVariables.h"
#include "OutLogger.h"


using namespace LLDLEP;
using namespace LLDLEP::internal;
using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

namespace LLDLEP
{
namespace internal
{

OutLogger::OutLogger() :
    OutWriter(new LLDLEP::internal::OutLoggerEnviromentVariables(), false),
    sock(NULL)
{

}

OutLogger::~OutLogger()
{
}


bool 
OutLogger::connect()
{
    cout << "Connecting to out server... " << endl;
    try
    {
        boost::asio::io_service io_service;
        //tcp::socket socket(io_service);
        
        // Creating a resolver's query.
        boost::asio::ip::tcp::resolver::query resolver_query(env_vars -> get_host(),
                                                             env_vars -> get_port(),
                                                             boost::asio::ip::tcp::resolver::query::numeric_service);

        // Creating a resolver.
        boost::asio::ip::tcp::resolver resolver(io_service);
     
        // Resolving a DNS name.
        boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(resolver_query);
        
        // Creating a socket.
        OutLogger::sock = new boost::asio::ip::tcp::socket(io_service);

        // asio::connect() method iterates over
        // each endpoint until successfully connects to one
        // of them. It will throw an exception if it fails
        // to connect to all the endpoints or if other
        // error occurs.
        boost::asio::connect(*OutLogger::sock, it);

    }
    catch (boost::system::system_error &e) {
        std::cout << "Error occured! Error code = " << e.code()
                  << ". Message: " << e.what();
        OutLogger::close();
        OutLogger::is_connected = false;
        return false;
    }
    OutLogger::is_connected = true;
    return true;
}

void
OutLogger::close()
{
    OutLogger::sock -> close();
}

bool
OutLogger::send_out(const std::string & message)
{
    if(OutLogger::is_connected == false)
    {
        if(!OutLogger::connect())
        {
            std::cout << "Could not connect to socket." << std::endl;
            OutLogger::close();
            return false;
        }
    }
    
    boost::system::error_code error;
    //sending
    boost::asio::write( *(OutLogger::sock), boost::asio::buffer(message), error );
    if( !error ) {
       cout << "Message has sent successfully!" << endl;
    }
    else {
       cout << "Sending message failed: " << error.message() << endl;
       OutLogger::close();
       return false;
    }
    return true;
}

} // namespace internal
} // namespace LL-DLEP
