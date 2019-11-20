// MongoDB.h

#ifndef _DBBENCHMARK_MONGODB_H_
#define _DBBENCHMARK_MONGODB_H_

#include <memory>
#include <iterator> // for iterators 
#include <vector> // for vectors 

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>

#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/types/value.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <nlohmann/json.hpp>

#include "Core/DB.h"


using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace dbbenchmark {
namespace mongodb {
/**
*   \brief MongoDB client wrapper for mongocxx library.
*   \details  Database accessing layer. Each thread in the client will be given its own instance of whatever DB 
*     class is to be used in the test. This class should be constructed using a no-argument 
*     constructor, so we can load it dynamically. Any argument-based initialization 
*     should be done by init().
*   \author Yücel ALBAR
*   \version 1.0
*   \date 27/11/2018
*   \bug None so far
*/
const std::string SETTINGS_TAG = "DBSettings.";
const std::string HOST_PROPERTY = SETTINGS_TAG + "host";
const std::string PORT_PROPERTY = SETTINGS_TAG + "port";
const std::string DEFAULT_PORT = SETTINGS_TAG + "27017";
const std::string USERNAME_PROPERTY = SETTINGS_TAG + "username";
const std::string PASSWORD_PROPERTY = SETTINGS_TAG + "password";
const std::string CLUSTER_PROPERTY = SETTINGS_TAG + "cluster";
const std::string CONN_TIMEOUT = SETTINGS_TAG + "conn_timeout";
const std::string RW_TIMEOUT = SETTINGS_TAG + "rw_timeout";
const std::string MAX_CONNS = SETTINGS_TAG + "max_conns";
const std::string INDEX_KEY = SETTINGS_TAG + "_indices";

class MongoDB : public DB {
public:
    MongoDB() {
        uri = nullptr;
        client = nullptr;
        db = nullptr;
        m_localConf = &(utility::programconfigurations::LayeredConfiguration::Instance());
    };
    void init() override;
    void cleanup() override;
    Status read(const std::string &table, const std::string &key, 
                    const std::vector<std::string> &fields, std::vector<stringPair> &result) override;
    Status scan(const std::string &table, const std::string &startkey, int recordcount, 
                    std::vector<std::string> &fields, std::vector<std::vector<stringPair>> &result) override;
    Status update(const std::string &table, const std::string &key, 
                    std::vector<stringPair> values) override;
    Status insert(const std::string &table, const std::string &key, 
                    std::vector<stringPair> &values) override;
    Status Delete(const std::string &table, const std::string &key) override;

    ~MongoDB();
protected:

private:
/*
* Calculate a hash for a key to store it in an index. The actual return value
* of this function is not interesting -- it primarily needs to be fast and
* scattered along the whole space of doubles. In a real world scenario one
* would probably use the ASCII values of the keys.
*/
    double hash(const std::string &key) { return std::hash<std::string>{}(key); };

    mongocxx::uri *uri;
    mongocxx::client *client;
    mongocxx::database *db;
};

} // namespace mongodb
} // namespace dbbenchmark

#endif // _DBBENCHMARK_MONGODB_H_