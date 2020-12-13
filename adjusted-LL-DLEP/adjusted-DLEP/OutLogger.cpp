#include "OutLogger.h"

using namespace LLDLEP;
using namespace LLDLEP::internal;

OutLogger::OutLogger(const std::string & msg_description,
                     const LLDLEP::DataItems & data_items,
                     bool sending,
                     bool receiving) :
    message(""),
    msg_description(msg_description),
    data_items(data_items),
    sending(sending),
    receiving(receiving)
{
}

bool 
OutLogger::send_out(const std::string & host, const std::string & port)
{
    build_message();
    
}

void
OutLogger::add_header()
{
    message += msg_description;
}

void 
OutLogger::add_data_item()
{

}

void 
OutLogger::add_data_items()
{
    for (const DataItem & di : data_items)
    {
        add_data_item(di);
    }
}

void 
OutLogger::build_message()
{
    add_header();
    add_data_items();
}