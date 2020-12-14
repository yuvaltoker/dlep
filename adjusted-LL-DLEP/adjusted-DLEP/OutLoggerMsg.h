#ifndef OUTLOGGERMSG_H
#define OUTLOGGERMSG_H

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
        const LLDLEP::DataItems & data_items,
        const std::string & direction);

    std::string get_message();


private:
    std::string & message;
    std::string & protocol;
    std::string & msg_type;
    LLDLEP::DataItems & data_items;
    std::string & direction;

    void build_message();
    void add_protocol();
    void add_msg_type();
    void add_data_item(const LLDLEP::DataItem & di);
    void add_data_items();
    void add_direction();
}



} // namespace internal
} // namespace LLDLEP

#endif // OUTLOGGERMSG_H