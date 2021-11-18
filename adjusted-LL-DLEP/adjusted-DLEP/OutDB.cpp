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
    //std::cout << "im here first" << std::endl;
    //std::cout << dlep_msg_json.at("DataItems") << std::endl;
    //std::string items = dlep_msg_json.at("DataItems").at("\"Name\" : \"Peer_Type\"").at("Value").ToString();
    auto items = dlep_msg_json.at("DataItems");
   
    /*for( int j =0; j < items.length();j++)
    {
        if(items.at(j))
            std::cout << items.at(j) << "\n";
    }*/
    std::string peer_type;
    for( auto &j : items.ArrayRange() )
    {
        if(j.at("Name").ToString() == "Peer_Type")
        {
            peer_type = j.at("Value").ToString();
        }  
    }
    

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