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
                           const std::string & msg_type,
                           const std::string & direction,
                           const std::string & modem_address,
                           const DataItems & msg_data_items) :
    message(""),
    protocol(protocol),
    msg_type(msg_type),
    direction(direction),
    modem_address(modem_address),
    msg_data_items(msg_data_items)
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
    add_modem_address_to_message();
    message += "\n    ";
    message += "\"Status\":\"S\",";
    message += "\n    ";
    //message += "\"Error\":\"none\",";
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
OutLoggerMsg::add_direction_to_message()
{
    message += "\"Direction\":\"" + direction + "\",";
}

void 
OutLoggerMsg::add_modem_address_to_message()
{
    message += "\"Modem Address\":\"" + modem_address + "\",";
}

void 
OutLoggerMsg::add_data_item_to_message(const DataItem & di)
{
    message += "\"Name\":\"" + di.name() + "\",\"Value\":\"" + di.value_to_string() + "\"";
}

void 
OutLoggerMsg::add_data_items_to_message()
{
    message += "\"Data Items\":[";
    for (const DataItem & di : msg_data_items)
    {
        message += "\n        {";
        add_data_item_to_message(di);
        message += "},";
    }
    if(message.back() == ',')
        message.resize(message.size() - 1); // removing the last ','. the last object should not have ','.
    message += "\n    ]";
}

} // namespace internal
} // namespace LL-DLEP


