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

    bool connect();

    void close();

    bool send_out(const std::string & mesage);

private:
    OutLogger();
    
    boost::asio::ip::tcp::socket* sock;
    LLDLEP::internal::OutLoggerEnviromentVariables env_vars;
    bool is_connected;
};

} // namespace internal
} // namespace LLDLEP

#endif // OUTLOGGER_H