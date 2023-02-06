#include "OutDB.h"
#include "json.hpp"

using namespace LLDLEP;
using namespace LLDLEP::internal;
using namespace boost::asio;

using std::string;

namespace LLDLEP
{
namespace internal
{
OutDB::OutDB() :
    OutWriter(),
    rmq_mdb_host("127.0.0.1"),
    rmq_mdb_port(12345)
{

}

OutDB::~OutDB()
{
}  

bool
OutDB::send_out(const std::string & message)
{
    std::cout << message << std::endl;
    // old pathway of uploading to mongo db with cpp
    // message_handler(message);
    send_update_over_socket(message);
    std::cout << "after message handler" << std::endl;
    return true;

}

//void
//OutDB::print_devices_base_info()
//{
//    std::cout << "devices _id and Ip: " << std::endl;
//    for (auto it = OutDB::devices_base_info.begin(); it != OutDB::devices_base_info.end(); ++it)
//        std::cout << (*it).first << " : " << (*it).second << std::endl;
//}
//
//std::string
//OutDB::get_id_by_ip(const std::string & ip)
//{
//    for (auto it = OutDB::devices_base_info.begin(); it != OutDB::devices_base_info.end(); ++it)
//        if((*it).second == ip)
//            return (*it).first;
//    return "ip not found";
//}
//
//void
//OutDB::message_handler(const std::string & message)
//{
//    json::JSON dlep_msg_json = json::JSON::Load(message);
//    if(dlep_msg_json.at("Stage").ToString() == "Progression" && 
//        dlep_msg_json.at("MessageType").ToString() == "Peer_Offer")
//    {
//        insert_device_to_db(dlep_msg_json);
//    }
//
//    std::string device_id = get_id_by_ip(dlep_msg_json.at("ModemAddress").ToString());
//    dlep_msg_json["DeviceId"] = device_id;
//    insert_dlep_message_to_db(dlep_msg_json.dump());
//}
//
//void
//OutDB::insert_dlep_message_to_db(const std::string & message)
//{
//    mHandler.AddDLEPMessageByJsonString(message);
//}
//
//void
//OutDB::insert_device_to_db(const json::JSON &dlep_msg_json)
//{
//    std::string device = make_device_json_string(dlep_msg_json);
//    std::cout << device << std::endl;
//    std::string oid_string = mHandler.AddDeviceByJsonString(device);
//    device_base_info info = { oid_string, dlep_msg_json.at("ModemAddress").ToString() };
//    auto it = OutDB::devices_base_info.insert(OutDB::devices_base_info.begin(), info);
//
//    OutDB::send_update_over_rabbitmq(oid_string);
//}
//
//std::string
//OutDB::make_device_json_string(const json::JSON &dlep_msg_json)
//{
//    auto items = dlep_msg_json.at("DataItems");
//   
//    std::string peer_type = "";
//    for( auto &j : items.ArrayRange() )
//    {
//        if(j.at("Name").ToString() == "Peer_Type")
//        {
//            peer_type = j.at("Value").ToString();
//        }  
//    }
//    std::stringstream ss( peer_type );
//    std::vector<std::string> vect;
//
//    while( ss.good() )
//    {
//        std::string substr;
//        getline( ss, substr, ';' );
//        vect.push_back( substr );
//    }
//
//    return std::string("{\n") +
//	                    "    \"Ip\": \"" + dlep_msg_json.at("ModemAddress").ToString() +"\",\n" +
//	                    "    \"NetworkType\": \"" + vect[1] + "\",\n" +
//	                    "    \"RadioType\": \"" + vect[2] + "\",\n" +
//	                    "    \"SerialNumber\": \"" + vect[3] + "\",\n" +
//	                    "    \"Key\": \"" + vect[4] + "\"\n" +
//                        "}"; 
//}
//
//bool 
//OutDB::send_update_over_rabbitmq(const std::string & uid)
//{
//    if(is_connected == false)
//    {
//        if(!OutDB::connect())
//        {
//            std::cout << "Could not connect to socket." << std::endl;
//            OutDB::close();
//            return false;
//        }
//    }
//    
//    boost::system::error_code error;
//    //sending
//    boost::asio::write( *(OutDB::sock), boost::asio::buffer(uid), error );
//    if( !error ) {
//       std::cout << "New device entered to device_ids: " << uid << std::endl;
//    }
//    else {
//       std::cout << "Sending message failed: " << error.message() << std::endl;
//       OutDB::close();
//       return false;
//    }
//    return true;
//}

bool
OutDB::send_update_over_socket(const std::string & message)
{
    if(is_connected == false)
    {
        if(!OutDB::connect())
        {
            std::cout << "Could not connect to socket." << std::endl;
            OutDB::close();
            return false;
        }
    }
    boost::system::error_code err;
    try
    {
        std::string new_msg = std::to_string(message.size()) + message;
        boost::asio::write(*OutDB::sock, boost::asio::buffer(new_msg), err);
        if( !err ) 
        {
            std::cout << "Sent message to rmq_mdb successfully: " << new_msg << std::endl;
        }
        else
        {
            std::cout << "Sending message failed but not exception: " << err.message() << std::endl;
        }
    }
    catch(const std::exception& e)
    {
        std::cout << "Sending message failed: " << e.what() << std::endl;
        OutDB::close();
        return false;
    }
    return true;
}

bool 
OutDB::connect()
{
    std::cout << "Connecting to out server... " << std::endl;
    try
    {
        boost::asio::io_service ios;
        // socket creation
        OutDB::sock = new boost::asio::ip::tcp::socket(ios);

        boost::asio::ip::tcp::endpoint end_point(boost::asio::ip::address::from_string(rmq_mdb_host),
            rmq_mdb_port);
        

        // asio::connect() method iterates over
        // each endpoint until successfully connects to one
        // of them. It will throw an exception if it fails
        // to connect to all the endpoints or if other
        // error occurs.
        //boost::asio::connect(*OutDB::sock, end_point);

        OutDB::sock -> connect(end_point);

    }
    catch (boost::system::system_error &e) {
        std::cout << "Error occured! Error code = " << e.code()
                  << ". Message: " << e.what();
        OutDB::close();
        OutDB::is_connected = false;
        return false;
    }
    OutDB::is_connected = true;
    return true;
}

void
OutDB::close()
{
    OutDB::sock -> close();
    OutDB::is_connected = false;
}



std::vector<device_base_info> OutDB::devices_base_info;
} // namespace internal
} // namespace LL-DLEP