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
    sock(NULL),
    env_vars(new LLDLEP::internal::OutLoggerEnviromentVariables()),
    is_connected(false)
{
}

OutLogger::~OutLogger()
{
    delete env_vars;
}


bool 
OutLogger::connect()
{
    cout << "Connecting ... " << endl;
    try
    {
        boost::asio::io_service io_service;
        //tcp::socket socket(io_service);
        
        // Creating a resolver's query.
        boost::asio::ip::tcp::resolver::query resolver_query(env_vars -> get_host(),
                                                             env_vars -> get_port(),
                                                             boost::asio::ip::tcp::resolver::query::numeric_service);

        std::cout << "after resolver's query..." << endl;

        // Creating a resolver.
        boost::asio::ip::tcp::resolver resolver(io_service);
     
        std::cout << "after creating a resolver..." << endl;

        // Resolving a DNS name.
        boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(resolver_query);
   
        std::cout << "after resolving a DNS name..." << endl;
        
        // Creating a socket.
        OutLogger::sock = new boost::asio::ip::tcp::socket(io_service);
     
        std::cout << "after creating a socket..." << endl;

        // asio::connect() method iterates over
        // each endpoint until successfully connects to one
        // of them. It will throw an exception if it fails
        // to connect to all the endpoints or if other
        // error occurs.
        boost::asio::connect(*OutLogger::sock, it);

        std::cout << "after connecting..." << endl;
    }
    catch (boost::system::system_error &e) {
        std::cout << "Error occured! Error code = " << e.code()
                  << ". Message: " << e.what();
        OutLogger::close();
        OutLogger::is_connected = false;
        return false;
    }
    OutLogger::is_connected = true;
    std::cout << "after connecting..." << endl;
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

/*bool OutLogger::is_connected = false;
LLDLEP::internal::OutLoggerEnviromentVariables env_vars();
boost::asio::ip::tcp::socket* OutLogger::sock = NULL;*/

} // namespace internal
} // namespace LL-DLEP
