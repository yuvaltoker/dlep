#ifndef OUTLOGGER_H
#define OUTLOGGER_H

#include "OutLoggerEnviromentVariables.h"
#include "OutWriter.h"

#include <iostream>
#include <boost/asio.hpp>

namespace LLDLEP
{
namespace internal
{

class OutLogger : public OutWriter
{
public:
    OutLogger();

    ~OutLogger();

    bool connect();

    void close();

    bool send_out(const std::string & mesage);

private:
    
    boost::asio::ip::tcp::socket* sock;
};

} // namespace internal
} // namespace LLDLEP

#endif // OUTLOGGER_H