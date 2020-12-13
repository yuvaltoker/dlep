#ifndef OUTLOGGER_H
#define OUTLOGGER_H

namespace LLDLEP
{
namespace internal
{

class OutLogger
{
public:
    OutLogger(
        const std::string & msg_description,
        const LLDLEP::DataItems & data_items,
        bool sending,
        bool receiving);

    bool send_out(const std::string & host,
                  const std::string & port);

private:
    std::string & message;
    std::string & msg_description;
    LLDLEP::DataItems & data_items;
    bool sending;
    bool receiving;

    void add_header();
    void add_data_item();
    void add_data_items();
    void build_message();
}



} // namespace internal
} // namespace LLDLEP

#endif // OUTLOGGER_H