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

} // namespace internal
} // namespace LL-DLEP


