
using namespace LLDLEP;
using namespace LLDLEP::internal;

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
    json::JSON dlep_msg_json = json::JSON::Load(message);
    message_handler(dlep_msg_json);
    
    return true;

}

void
OutDB::message_handler(const json::JSON &dlep_msg_json)
{
    if(dlep_msg_json.at("Stage").ToString() == "Progression" && 
        dlep_msg_json.at("MessageType").ToString() == "Peer_Discovery")
    {
        insert_device_to_db(dlep_msg_json);
    }
    insert_dlep_message_to_db(dlep_msg_json);
}

void
OutDB::insert_dlep_message_to_db(const json::JSON &dlep_msg_json)
{
    mongocxx::instance instance;
    learning::MongoDbHandler mHandler;

    mHandler.AddDLEPMessageByJson(dlep_msg_json);
}

void
OutDB::insert_device_to_db(const json::JSON &dlep_msg_json)
{
    mongocxx::instance instance;
    learning::MongoDbHandler mHandler;

    std::string device = make_device_json_string(dlep_msg_json);
    json::JSON device_json = json::JSON::Load(device);
    
    mHandler.AddDeviceByJson(device_json);
}

std::string
OutDB::make_device_json_string(const json::JSON &dlep_msg_json)
{
    return std::string("{") +
	                    "\"Ip\": \"" + dlep_msg_json.at("ModemAdress").ToString() +"\"," +
	                    "\"NetworkType\": \"" +  +"\"," +
	                    "\"RadioType\": \"" +  +"\"," +
	                    "\"SerialNumber\": \"" +  +"\"," +
	                    "\"Key\": \"" +  +"\"" +
                        "}"; 
}

} // namespace internal
} // namespace LL-DLEP