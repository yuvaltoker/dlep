#include "OutLoggerMsg.h"

using namespace LLDLEP;
using namespace LLDLEP::internal;
using std::string;
using std::cout;
using std::endl;

namespace LLDLEP
{
namespace internal
{

OutLoggerMsg::OutLoggerMsg(const std::string & protocol,
                           const std::string & stage) :
    protocol(protocol),
    stage(stage)
{
}

OutLoggerMsg::~OutLoggerMsg()
{

}

void
OutLoggerMsg::add_protocol_to_message(std::string & message)
{
    message += "\"Protocol\":\"" + protocol + "\"," ;
}

void
OutLoggerMsg::add_stage_to_message(std::string & message)
{
    message += "\"Stage\":\"" + stage + "\"," ;
}

void
OutLoggerMsg::add_timeStamp_to_message(std::string & message)
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
    std::string timeStamp(buffer);

    message += "\"Time\":\"" + timeStamp + "\"," ;
}

} // namespace internal
} // namespace LL-DLEP


