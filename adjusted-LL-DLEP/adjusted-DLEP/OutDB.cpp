#include "OutDB.h"
#include "json.hpp"

using namespace LLDLEP;
using namespace LLDLEP::internal;

using std::string;

namespace LLDLEP
{
namespace internal
{
    OutDB::OutDB() :
    OutWriter()
{

}

OutDB::~OutDB()
{
}  

bool
OutDB::send_out(const std::string & message)
{
    std::cout << message << std::endl;
    message_handler(message);
    std::cout << "after message handler" << std::endl;
    return true;

}

void
OutDB::message_handler(const std::string & message)
{
    json::JSON dlep_msg_json = json::JSON::Load(message);
    if(dlep_msg_json.at("Stage").ToString() == "Progression" && 
        dlep_msg_json.at("MessageType").ToString() == "Peer_Offer")
    {
        insert_device_to_db(dlep_msg_json);
    }
    insert_dlep_message_to_db(message);
}

void
OutDB::insert_dlep_message_to_db(const std::string & message)
{
    mHandler.AddDLEPMessageByJsonString(message);
}

void
OutDB::insert_device_to_db(const json::JSON &dlep_msg_json)
{
    std::string device = make_device_json_string(dlep_msg_json);
    std::cout << device << std::endl;
    mHandler.AddDeviceByJsonString(device);
}

std::string
OutDB::make_device_json_string(const json::JSON &dlep_msg_json)
{
    return std::string("{\n") +
	                    "    \"Ip\": \"" + dlep_msg_json.at("ModemAddress").ToString() +"\",\n" +
	                    "    \"NetworkType\": \"networktype\",\n" +
	                    "    \"RadioType\": \"radiotype\",\n" +
	                    "    \"SerialNumber\": \"serialnumber\",\n" +
	                    "    \"Key\": \"key\"\n" +
                        "}"; 
}

} // namespace internal
} // namespace LL-DLEP