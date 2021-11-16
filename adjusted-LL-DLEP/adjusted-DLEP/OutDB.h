#ifndef OUTDB_H
#define OUTDB_H

#include "mongocxx/instance.hpp"
#include "MongodbHandler.h"
#include "OutLoggerEnviromentVariables.h"
#include "OutWriter.h"

#include <iostream>

namespace LLDLEP
{
namespace internal
{

class OutDB : public OutWriter
{
public:
    OutDB();

    ~OutDB();

    bool send_out(const std::string & mesage);

private:
    void message_handler(const std::string & message);

    void insert_dlep_message_to_db(const std::string & message);

    void insert_device_to_db(const json::JSON &dlep_msg_json);

    std::string make_device_json_string(const json::JSON &dlep_msg_json);
    
};

} // namespace internal
} // namespace LLDLEP

#endif // OUTDB_H