// RedisDB.h

#ifndef _DBBENCHMARK_REDISDB_H_
#define _DBBENCHMARK_REDISDB_H_

#include "Core/DB.h"

#include <memory>
#include <acl_cpp/lib_acl.hpp>

namespace dbbenchmark {
namespace redisdb {

/**
*   \brief RedisDB client wrapper for acl Redis library.
*   \details  Database accessing layer. Each thread in the client will be given its own instance of whatever DB 
*     class is to be used in the test. This class should be constructed using a no-argument 
*     constructor, so we can load it dynamically. Any argument-based initialization 
*     should be done by init().
*   \author Ozgun AY
*   \version 1.0
*   \date 21/11/2018
*   \bug None so far
*/
const std::string SETTINGS_TAG = "DBSettings.";
const std::string HOST_PROPERTY = SETTINGS_TAG + "host";
const std::string PORT_PROPERTY = SETTINGS_TAG + "port";
const std::string DEFAULT_PORT = SETTINGS_TAG + "6379";
const std::string PASSWORD_PROPERTY = SETTINGS_TAG + "password";
const std::string CLUSTER_PROPERTY = SETTINGS_TAG + "cluster";
const std::string CONN_TIMEOUT = SETTINGS_TAG + "conn_timeout";
const std::string RW_TIMEOUT = SETTINGS_TAG + "rw_timeout";
const std::string MAX_CONNS = SETTINGS_TAG + "max_conns";
const std::string INDEX_KEY = SETTINGS_TAG + "_indices";

class RedisDB : public DB {
public:
    RedisDB() {
        redisClient = nullptr;
        redisClientCluster = nullptr;
        m_localConf = &(utility::programconfigurations::LayeredConfiguration::Instance());
    };

    void init() override;
    void cleanup() override;
    Status read(const std::string &table, const std::string &key, 
                    const std::vector<std::string>& fields, std::vector<stringPair> &result) override;
    Status scan(const std::string &table, const std::string &startkey, int recordcount, 
                    std::vector<std::string> &fields, std::vector<std::vector<stringPair>> &result) override;
    Status update(const std::string &table, const std::string &key, 
                    std::vector<stringPair> values) override;
    Status insert(const std::string &table, const std::string &key, 
                    std::vector<stringPair> &values) override;
    Status Delete(const std::string &table, const std::string &key) override;

    ~RedisDB();

protected:

private:
    /*
    * Calculate a hash for a key to store it in an index. The actual return value
    * of this function is not interesting -- it primarily needs to be fast and
    * scattered along the whole space of doubles. In a real world scenario one
    * would probably use the ASCII values of the keys.
    */
    double hash(const std::string &key) { return std::hash<std::string>{}(key); };
    acl::redis_client *redisClient; // acl::redis library uses raw pointers so here we have to use raw pointer
    acl::redis_client_cluster *redisClientCluster; // acl::redis library uses raw pointers so here we have to use raw pointer
    std::unique_ptr<acl::redis_hash> cmdHash;
    std::unique_ptr<acl::redis_connection> redisConnection;
    //std::unique_ptr<acl::redis_zset> zSet;
};

} // namespace redisdb
} // namespace dbtester

#endif // _DBBENCHMARK_REDISDB_H_