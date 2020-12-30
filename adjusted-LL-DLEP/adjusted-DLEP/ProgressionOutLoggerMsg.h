#ifndef PROGRESSIONOUTLOGGERMSG_H
#define PROGRESSIONOUTLOGGERMSG_H

#include <iostream>
#include "DataItem.h"
#include "OutLoggerMsg.h"

namespace LLDLEP
{
namespace internal
{

class ProgressionOutLoggerMsg : public OutLoggerMsg
{
public:
    ProgressionOutLoggerMsg(const std::string & protocol,
                            const std::string & status,
                            const std::string & progress_type,
                            const std::string & direction,
                            const std::string & modem_address,
                            const std::string & error,
                            const LLDLEP::DataItems & msg_data_items);

    ~ProgressionOutLoggerMsg();

    std::string get_message();


private:
    std::string message;
    std::string status;
    std::string progress_type;
    std::string direction;
    std::string modem_address;
    std::string error;
    LLDLEP::DataItems msg_data_items;
    
    void build_message();
    void add_status_to_message();
    void add_progress_type_to_message();
    void add_direction_to_message();
    void add_modem_address_to_message();
    void add_error_to_message();
    void add_data_item_to_message(const LLDLEP::DataItem & di);
    void add_data_items_to_message();
    
};

} // namespace internal
} // namespace LLDLEP

#endif // PROGRESSIONOUTLOGGERMSG_H
