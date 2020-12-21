#ifndef OUTLOGGERMSG_H
#define OUTLOGGERMSG_H

#include <iostream>
#include "DataItem.h"

namespace LLDLEP
{
namespace internal
{

class OutLoggerMsg
{
public:
    OutLoggerMsg(
        const std::string & protocol,
        const std::string & msg_type,
        const LLDLEP::DataItems & msg_data_items,
        const std::string & direction);

    std::string get_message();


private:
    std::string message;
    std::string protocol;
    std::string msg_type;
    LLDLEP::DataItems msg_data_items;
    std::string direction;

    void build_message();
    void add_protocol_to_message();
    void add_msg_type_to_message();
    void add_data_item_to_message(const LLDLEP::DataItem & di);
    void add_data_items_to_message();
    void add_direction_to_message();
};



} // namespace internal
} // namespace LLDLEP

#endif // OUTLOGGERMSG_H
