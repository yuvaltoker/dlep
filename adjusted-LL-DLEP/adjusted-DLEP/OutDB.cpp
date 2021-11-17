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
OutDB::print_devices_base_info()
{
    std::cout << "devices _id and Ip: " << std::endl;
    for (auto it = OutDB::devices_base_info.begin(); it != OutDB::devices_base_info.end(); ++it)
        std::cout << (*it).first << " : " << (*it).second << std::endl;
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
    std::string oid_string = mHandler.AddDeviceByJsonString(device);
    device_base_info info = { oid_string, dlep_msg_json.at("ModemAddress").ToString() };
    auto it = OutDB::devices_base_info.insert(OutDB::devices_base_info.begin(), info);
}

std::string
OutDB::make_device_json_string(const json::JSON &dlep_msg_json)
{
    //std::string items = dlep_msg_json.at("DataItems").at("\"Name\" : \"Peer_Type\"").at("Value").ToString();
    //std::string items = dlep_msg_json.at("DataItems").at("Peer_Type").at("Value").ToString();
    std::string items = dlep_msg_json.at("DataItems").at("Peer_Type").ToString();
    std::cout << items << std::endl;
    std::cout << "im here" << std::endl;
    /*for( auto &j : items.ObjectRange() )
        std::cout << "Object[ " << j.first << " ] = " << j.second << "\n";*/
    return std::string("{\n") +
	                    "    \"Ip\": \"" + dlep_msg_json.at("ModemAddress").ToString() +"\",\n" +
	                    "    \"NetworkType\": \"networktype\",\n" +
	                    "    \"RadioType\": \"radiotype\",\n" +
	                    "    \"SerialNumber\": \"serialnumber\",\n" +
	                    "    \"Key\": \"key\"\n" +
                        "}"; 
}

std::vector<device_base_info> OutDB::devices_base_info;
} // namespace internal
} // namespace LL-DLEP