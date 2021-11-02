#ifndef OUTWRITER_H
#define OUTWRITER_H

#include "OutLoggerEnviromentVariables.h"

//#include <iostream>
//#include <boost/asio.hpp>

namespace LLDLEP
{
namespace internal
{

class OutWriter
{
public:
    OutWriter();

    ~OutWriter();

    virtual bool send_out(const std::string & mesage) = 0;

protected:

    virtual bool connect() = 0;

    virtual void close() = 0;

    LLDLEP::internal::OutLoggerEnviromentVariables* env_vars;
    bool is_connected;
};

} // namespace internal
} // namespace LLDLEP

#endif // OUTWRITER_H