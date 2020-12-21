#include "OutLoggerMsg.h"

using namespace LLDLEP;
using namespace LLDLEP::internal;
using std::string;

namespace LLDLEP
{
namespace internal
{

OutLoggerMsg::OutLoggerMsg(const std::string & protocol,
                           const std::string & msg_type,
                           const DataItems & msg_data_items,
                           const std::string & direction) :
    message(""),
    protocol(protocol),
    msg_type(msg_type),
    msg_data_items(msg_data_items),
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
    message += "{\n    ";
    add_protocol_to_message();
    message += "\n    ";
    add_msg_type_to_message();
    message += "\n    ";
    add_direction_to_message();
    message += "\n    ";
    message += "\"Status\": \"S\",";
    message += "\n    ";
    //message += "\"Error\": \"none\",";
    //message += "\n    ";
    add_data_items_to_message();
    message += "\n}";
}

void
OutLoggerMsg::add_protocol_to_message()
{
    message += "\"Protocol\":\"" + protocol + "\"," ;
}

void
OutLoggerMsg::add_msg_type_to_message()
{
    message += "\"Message Type\":\"" + msg_type + "\"," ;
}

void 
OutLoggerMsg::add_data_item_to_message(const DataItem & di)
{
    message += "\"Name\": \"" + di.name() + "\", \"Value\": \"" + di.value_to_string() + "\"";
}

void 
OutLoggerMsg::add_data_items_to_message()
{
    message += "\"Data Items\": ["; 
    for (const DataItem & di : msg_data_items)
    {
        message += "\n        {";
        add_data_item_to_message(di);
        message += "},";
    }
    message.resize(message.size() - 1); // removing the last ','. the last object should not have ','.
    message += "\n    ]";
}

void 
OutLoggerMsg::add_direction_to_message()
{
    message += "\"Direction\":\"" + direction + "\",";
}

} // namespace internal
} // namespace LL-DLEP


