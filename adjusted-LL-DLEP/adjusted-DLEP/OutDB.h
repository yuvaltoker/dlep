#ifndef OUTDB_H
#define OUTDB_H

#include "mongocxx/instance.hpp"
#include "MongodbHandler.h"
#include "OutLoggerEnviromentVariables.h"
#include "OutWriter.h"

#include <iostream>
#include <vector>
#include <sstream>

namespace LLDLEP
{
namespace internal
{

typedef std::pair< std::string, std::string > device_base_info;

class OutDB : public OutWriter
{
public:
    OutDB();

    ~OutDB();

    bool send_out(const std::string & mesage);

    static void print_devices_base_info();

private:

    std::string get_id_by_ip(const std::string & ip);

    void message_handler(const std::string & message);

    void insert_dlep_message_to_db(const std::string & message);

    void insert_device_to_db(const json::JSON &dlep_msg_json);

    std::string make_device_json_string(const json::JSON &dlep_msg_json);

    mongocxx::instance instance;
    MongoDbHandler mHandler;
    static std::vector<device_base_info> devices_base_info;
    
};

} // namespace internal
} // namespace LLDLEP

#endif // OUTDB_H