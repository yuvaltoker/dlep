#ifndef CONFIGOUTLOGGERMSG_H
#define CONFIGOUTLOGGERMSG_H

#include <iostream>
#include <vector>
#include <string>
#include "DataItem.h"
#include "OutLoggerMsg.h"
#include "Parameter.h"

using namespace LLDLEP;
using namespace LLDLEP::internal;
using namespace LLDLEP::internal::ConfigStrings;

namespace LLDLEP
{
namespace internal
{

class ConfigOutLoggerMsg : public OutLoggerMsg
{
public:
    ConfigOutLoggerMsg(const std::string & protocol,
                       const std::vector<Parameter> & parameters);

    ~ConfigOutLoggerMsg();

    std::string get_message();


private:
    std::string message;
    std::vector<Parameter> parameters;
    

    void build_message();
    void add_parameter_to_message(const Parameter & di);
    void add_parameters_to_message();
    
};

} // namespace internal
} // namespace LLDLEP

#endif // CONFIGOUTLOGGERMSG_H