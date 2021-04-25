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

    

    auto now = std::chrono::system_clock::now(); 
    std::chrono::time_point<std::chrono::system_clock> epoch; 
    int ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - epoch).count() % 1000;

    strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
    std::string timeStamp(buffer);

    message += "\"Time\":\"" + timeStamp + "." + std::to_string(ms) + "\"," ;
}

} // namespace internal
} // namespace LL-DLEP


