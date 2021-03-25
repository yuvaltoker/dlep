#ifndef OUTLOGGER_H
#define OUTLOGGER_H

#include "OutLoggerEnviromentVariables.h"

#include <iostream>
#include <boost/asio.hpp>

namespace LLDLEP
{
namespace internal
{

class OutLogger
{
public:

    static bool connect();

    static void close();

    static bool send_out(const std::string & mesage);

private:
    OutLogger();
    
    static boost::asio::ip::tcp::socket* sock;
    static LLDLEP::internal::OutLoggerEnviromentVariables env_vars;
    static bool is_connected;
};

} // namespace internal
} // namespace LLDLEP

#endif // OUTLOGGER_H