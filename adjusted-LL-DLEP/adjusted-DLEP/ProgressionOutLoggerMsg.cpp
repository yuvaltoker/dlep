#include "ProgressionOutLoggerMsg.h"

using namespace LLDLEP;
using namespace LLDLEP::internal;
using std::string;
using std::cout;
using std::endl;

namespace LLDLEP
{
namespace internal
{

ProgressionOutLoggerMsg::ProgressionOutLoggerMsg(const std::string & protocol,
                                                 const std::string & status,
                                                 const std::string & msg_type,
                                                 const std::string & direction,
                                                 const std::string & modem_address,
                                                 const std::string & error,
                                                 const LLDLEP::DataItems & msg_data_items,
                                                 ProtocolConfig * protocfg) :
    OutLoggerMsg(protocol, "Progression"),
    status(status),
    msg_type(msg_type),
    direction(direction),
    modem_address(modem_address),
    error(error),
    msg_data_items(msg_data_items),
    protocfg(protocfg),
    message("")
{
}

// constructor with out data items for when an error occurs
ProgressionOutLoggerMsg::ProgressionOutLoggerMsg(const std::string & protocol,
                                                 const std::string & status,
                                                 const std::string & msg_type,
                                                 const std::string & direction,
                                                 const std::string & modem_address,
                                                 const std::string & error) :
    OutLoggerMsg(protocol, "Progression"),
    status(status),
    msg_type(msg_type),
    direction(direction),
    modem_address(modem_address),
    error(error),
    msg_data_items(),
    message("")
{
}

ProgressionOutLoggerMsg::~ProgressionOutLoggerMsg()
{
    
}

std::string
ProgressionOutLoggerMsg::get_message()
{
    build_message();
    return message;
}

void 
ProgressionOutLoggerMsg::build_message()
{
    message += "{";
    add_timeStamp_to_message(message);
    add_protocol_to_message(message);
    add_stage_to_message(message);
    add_status_to_message();
    add_msg_type_to_message();
    add_direction_to_message();
    add_modem_address_to_message();
    
    if(protocfg != nullptr)
    {
        add_data_items_to_message();
    } 
    if(error != "")
    {
        add_error_to_message();     
    } 
    message += "}";
}

void
ProgressionOutLoggerMsg::add_status_to_message()
{
    message += "\"Status\":\"" + status + "\",";
}

void
ProgressionOutLoggerMsg::add_msg_type_to_message()
{
    message += "\"MessageType\":\"" + msg_type + "\",";
}

void 
ProgressionOutLoggerMsg::add_direction_to_message()
{
    message += "\"Direction\":\"" + direction + "\",";
}

void 
ProgressionOutLoggerMsg::add_modem_address_to_message()
{
    message += "\"ModemAddress\":\"" + modem_address + "\",";
}

void
ProgressionOutLoggerMsg::add_error_to_message()
{
    message += "\"Error\":\"" + error + "\",";
}

void 
ProgressionOutLoggerMsg::add_data_item_to_message(const DataItem & di)
{
    const LLDLEP::DataItemInfo di_info = protocfg->get_data_item_info(di.name());
    if (di_info.sub_data_items.size() > 0)
        message += di.to_jason(&di_info);
    else
        message += di.to_jason();
}

void 
ProgressionOutLoggerMsg::add_data_items_to_message()
{
    message += "\"DataItems\":[";
    for (const DataItem & di : msg_data_items)
    {
        message += "{";
        add_data_item_to_message(di);
        message += "},";
    }
    if(message.back() == ',')
        message.resize(message.size() - 1); // removing the last ','. the last object should not have ','.
    message += "]";
}

} // namespace internal
} // namespace LL-DLEP


