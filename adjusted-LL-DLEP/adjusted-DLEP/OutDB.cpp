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

std::string
OutDB::get_id_by_ip(const std::string & ip)
{
    for (auto it = OutDB::devices_base_info.begin(); it != OutDB::devices_base_info.end(); ++it)
        if((*it).second == ip)
            return (*it).first;
    return "ip not found";
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

    std::string device_id = get_id_by_ip(dlep_msg_json.at("ModemAddress").ToString());
    dlep_msg_json["DeviceId"] = device_id;
    insert_dlep_message_to_db(dlep_msg_json.dump());
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
    auto items = dlep_msg_json.at("DataItems");
   
    std::string peer_type = "";
    for( auto &j : items.ArrayRange() )
    {
        if(j.at("Name").ToString() == "Peer_Type")
        {
            peer_type = j.at("Value").ToString();
        }  
    }
    std::stringstream ss( peer_type );
    std::vector<std::string> vect;

    while( ss.good() )
    {
        std::string substr;
        getline( ss, substr, ';' );
        vect.push_back( substr );
    }

    return std::string("{\n") +
	                    "    \"Ip\": \"" + dlep_msg_json.at("ModemAddress").ToString() +"\",\n" +
	                    "    \"NetworkType\": \"" + vect[1] + "\",\n" +
	                    "    \"RadioType\": \"" + vect[2] + "\",\n" +
	                    "    \"SerialNumber\": \"" + vect[3] + "\",\n" +
	                    "    \"Key\": \"" + vect[4] + "\"\n" +
                        "}"; 
}

std::vector<device_base_info> OutDB::devices_base_info;
} // namespace internal
} // namespace LL-DLEP