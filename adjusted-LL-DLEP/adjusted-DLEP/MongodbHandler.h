#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include <vector>

#include "bsoncxx/builder/stream/document.hpp"
#include "bsoncxx/json.hpp"
#include "bsoncxx/oid.hpp"
#include "mongocxx/client.hpp"
#include "mongocxx/database.hpp"
#include "mongocxx/uri.hpp"
#include "json.hpp"

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::basic::kvp;

using namespace bsoncxx;


namespace LLDLEP
{
namespace internal
{


constexpr char MongoDbUri[] = "mongodb://root:example@mongodb:27017";
constexpr char DatabaseName[] = "rri";
constexpr char dlepMessageCollection[] = "DlepMessage";
constexpr char deviceCollection[] = "Devices";

class MongoDbHandler {
 public:
  MongoDbHandler()
      : uri(mongocxx::uri(MongoDbUri)),
        client(mongocxx::client(uri)),
        db(client[DatabaseName]) {

  }

  std::string AddDLEPMessageByJsonString(const std::string &json_input)
  {
    mongocxx::collection collection = db[dlepMessageCollection];
    bsoncxx::document::value doc = bsoncxx::from_json(json_input);
    
    auto retVal = collection.insert_one(doc.view());  // Part where document is uploaded to database
    bsoncxx::oid oid = retVal->inserted_id().get_oid().value; // finding inserted object's id
    return oid.to_string();
  }

  // insert dlep-message (as the json) to database
  std::string AddDLEPMessageByJson(const json::JSON &dlep_message) 
  {
    mongocxx::collection collection = db[dlepMessageCollection];
    auto builder = bsoncxx::builder::stream::document{};

    bsoncxx::document::value doc =
        builder << "Time" << dlep_message.at("Time").ToString()
                << "Protocol" << dlep_message.at("Protocol").ToString()
                << "Stage" << dlep_message.at("Stage").ToString()
                << "Status" << dlep_message.at("Status").ToString()
                << "MessageType" << dlep_message.at("MessageType").ToString()
                << "Direction" << dlep_message.at("Direction").ToString()
                << "ModemAddress" << dlep_message.at("ModemAddress").ToString()
                //<< "DataItems" << dlep_message.at("DataItems").ToString() // dataitems - find how to do it
                << bsoncxx::builder::stream::finalize;
    
    auto retVal = collection.insert_one(doc.view());  // Part where document is uploaded to database
    bsoncxx::oid oid = retVal->inserted_id().get_oid().value; // finding inserted object's id
    return oid.to_string();
  }

  std::string AddDeviceByJsonString(const std::string &json_input)
  {
    mongocxx::collection collection = db[deviceCollection];
    bsoncxx::document::value doc = bsoncxx::from_json(json_input);
    
    auto retVal = collection.insert_one(doc.view());  // Part where document is uploaded to database
    bsoncxx::oid oid = retVal->inserted_id().get_oid().value; // finding inserted object's id
    return oid.to_string();
  }

  // insert device (as the json) to database
  std::string AddDeviceByJson(const json::JSON &device) {
    mongocxx::collection collection = db[deviceCollection];
    auto builder = bsoncxx::builder::stream::document{};

    bsoncxx::document::value doc =
        builder << "Ip" << device.at("ip").ToString()
                << "NetworkType" << device.at("networkType").ToString()
                << "RadioType" << device.at("radioType").ToString()
                << "SerialNumber" << device.at("serialNumber").ToString()
                << "Key" << device.at("key").ToString()
                << bsoncxx::builder::stream::finalize;
    
    auto retVal = collection.insert_one(doc.view());  // Part where document is uploaded to database
    bsoncxx::oid oid = retVal->inserted_id().get_oid().value; // finding inserted object's id
    return oid.to_string();

  }

  /*bool RemoveCharacterFromDb(const std::string &character_id) {
      mongocxx::collection collection = db[kCollectionName];
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::oid document_id(character_id);

    bsoncxx::document::value doc =
        builder << "_id" << document_id << bsoncxx::builder::stream::finalize;
    bsoncxx::stdx::optional<mongocxx::result::delete_result> maybe_result =
        collection.delete_one(doc.view());

    if(maybe_result) {
      return maybe_result->deleted_count() == 1;
    }
    return false;
  }*/

  /*::JSON GetAllDocuments() {
    mongocxx::collection collection = db[kCollectionName];
    mongocxx::cursor cursor = collection.find({});
    json::JSON result;
    result["characters"] = json::Array();
    if (cursor.begin() != cursor.end()) {
      for (auto doc : cursor) {
        result["characters"].append(bsoncxx::to_json(doc));
      }
    }
    return result;
  }*/

  /*json::JSON GetDocumentByName(const std::string &character_name)
  {
    mongocxx::collection collection = db[kCollectionName]; // getting the collection
    auto builder = bsoncxx::builder::stream::document{};
    
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result = collection.find_one(document{} << "characterName" << character_name << finalize);
    json::JSON result;
    if(maybe_result) {
      result["characters"] = json::Array();
      result["characters"].append(bsoncxx::to_json(*maybe_result));
      //std::cout << bsoncxx::to_json(*maybe_result) << "\n";
    }
    return result;
  }*/


 private:
  mongocxx::uri uri;
  mongocxx::client client;
  mongocxx::database db;
};

} // namespace internal
} // namespace LLDLEP