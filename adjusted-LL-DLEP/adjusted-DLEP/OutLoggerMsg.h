#ifndef OUTLOGGERMSG_H
#define OUTLOGGERMSG_H

#include <iostream>
#include <ctime>
#include <chrono>
#include "DataItem.h"

namespace LLDLEP
{
namespace internal
{

class OutLoggerMsg
{
public:
    OutLoggerMsg(const std::string & protocol,      // DLEP/SNMP 
                 const std::string & stage);        // config/progression

    virtual ~OutLoggerMsg();

    virtual std::string get_message() = 0;


private:
    std::string protocol;
    std::string stage;
    
protected:
    virtual void build_message() = 0;
    void add_protocol_to_message(std::string & message);
    void add_stage_to_message(std::string & message);
    void add_timeStamp_to_message(std::string & message);
    
};

} // namespace internal
} // namespace LLDLEP

#endif // OUTLOGGERMSG_H
