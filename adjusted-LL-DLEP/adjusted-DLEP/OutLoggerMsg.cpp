#include "OutLoggerMsg.h"

using namespace LLDLEP;
using namespace LLDLEP::internal;

OutLoggerMsg::OutLoggerMsg(const std::string & protocol,
                           const std::string & msg_type,
                           const LLDLEP::DataItems & data_items,
                           const std::string & direction) :
    message(""),
    protocol(protocol),
    msg_type(msg_type)
    data_items(data_items),
    direction(direction)
{
}

std::string
OutLoggerMsg::get_message()
{
    build_message();
    return message;
}

void 
OutLoggerMsg::build_message()
{
    message += "{";
    add_protocol();
    add_msg_type();
    add_data_items();
    add_direction();
    message += "}";
}

void
OutLoggerMsg::add_protocol()
{
    message += "\"Protocol\":\"" + protocol + "\"," 
}

void
OutLoggerMsg::add_msg_type()
{
    message += "\"Message Type\":\"" + msg_type + "\"," 
}

void 
OutLoggerMsg::add_data_item(const LLDLEP::DataItem & di)
{
    message += "\"" + di -> name() + "\":" + di -> value_to_string() + "\","
}

void 
OutLoggerMsg::add_data_items()
{
    message += "\"Data Items\":{"; 
    for (const DataItem & di : data_items)
    {
        add_data_item(di);
    }
    message.popback(); // removing the last ','. the last object should not have ','.
    message += "}";
}

void 
OutLoggerMsg::add_direction()
{
    message += "\"Direction\":\"" + direction + "\"" 
}