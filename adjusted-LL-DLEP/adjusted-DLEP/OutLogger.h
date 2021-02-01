#ifndef OUTLOGGER_H
#define OUTLOGGER_H

#include <iostream>
#include <boost/asio.hpp>

#include <cstdlib>

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
    static std::string host;
    static std::string port;
    static bool is_connected;
};

} // namespace internal
} // namespace LLDLEP

#endif // OUTLOGGER_H