#ifndef PARAMETER_H
#define PARAMETER_H

#include <iostream>

namespace LLDLEP
{
namespace internal
{

namespace ConfigStrings
{
    const std::string Local_Type = "local-type";
    const std::string Discovery_Enable = "discovery-enable";
    const std::string Discovery_Port = "discovery-port";
    const std::string Discovery_Iface = "discovery-iface";
    const std::string Discovery_Mcast_Address = "discovery-mcast-address";
    const std::string Discovery_Interval = "discovery-interval";
    const std::string Heartbeat_Interval = "heartbeat-interval";
    const std::string Heartbeat_Threshold = "heartbeat-threshold";
    const std::string Session_Port = "session-port";
    const std::string Peer_Type = "peer-type";
    const std::string Log_File = "log-file";
}

class Parameter
{
public:
    Parameter(const std::string & config_field,
              const std::string & value);

    ~Parameter();

    std::string getConfigField() const;
    std::string getValue() const;

private:
    std::string config_field;
    std::string value; 

};

} // internal
} // LLDLEP


#endif // PARAMETER_H