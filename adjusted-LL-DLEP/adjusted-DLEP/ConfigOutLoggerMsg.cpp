#include "ConfigOutLoggerMsg.h"

using namespace LLDLEP;
using namespace LLDLEP::internal;
using std::string;
using std::cout;
using std::endl;

namespace LLDLEP
{
namespace internal
{

ConfigOutLoggerMsg::ConfigOutLoggerMsg(const std::string & protocol,
                                       const std::vector<Parameter> & parameters) :
    OutLoggerMsg(protocol, "Configuration"),
    parameters(parameters),
    message("")
{
}

ConfigOutLoggerMsg::~ConfigOutLoggerMsg()
{
    
}

std::string
ConfigOutLoggerMsg::get_message()
{
    build_message();
    return message;
}

void 
ConfigOutLoggerMsg::build_message()
{
    message += "{\n    ";
    add_protocol_to_message(message);
    message += "\n    ";
    add_stage_to_message(message);
    message += "\n    ";
    add_parameters_to_message();
    message += "\n}\n";
}

void 
ConfigOutLoggerMsg::add_parameter_to_message(const Parameter & par)
{
    message += "\"ConfigureField\":\"" + par.getConfigField() +
               "\",\"Value\":\"" + par.getValue() + "\"";
}

void 
ConfigOutLoggerMsg::add_parameters_to_message()
{
    message += "\"Parameters\":[";
    for (const Parameter & par : parameters)
    {
        message += "\n        {";
        add_parameter_to_message(par);
        message += "},";
    }
    if(message.back() == ',')
        message.resize(message.size() - 1); // removing the last ','. the last object should not have ','.
    message += "\n    ]";
}

} // namespace internal
} // namespace LL-DLEP


