#ifndef CONFIGOUTLOGGERMSG_H
#define CONFIGOUTLOGGERMSG_H

/*
* This file contains a Header of the ConfigOutLoggerMsg class, which is a successor 
* of OutLoggerMsg. The purpose of this class is to contain the parameters of the configure message
* which sent to the server, the whole purpose does not belong to the purpose of the DLEP protocol.
*/

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
    /* Constructor of the class, contains: protocol variable (dlep/net-snmp)
    *                                      vector of parameters of the protocol
    */
    ConfigOutLoggerMsg(const std::string & protocol,
                       const std::vector<Parameter> & parameters);

    ~ConfigOutLoggerMsg();

    // Calling @build_message function, then returns the @message
    std::string get_message();


private:
    std::string message;
    std::vector<Parameter> parameters;
    
    // Building the @message
    void build_message();

    // Add given parameter to @message 
    void add_parameter_to_message(const Parameter & di);

    // Looping through all parameters in @parameters, adding them one by one to @message
    void add_parameters_to_message();
    
};

} // namespace internal
} // namespace LLDLEP

#endif // CONFIGOUTLOGGERMSG_H