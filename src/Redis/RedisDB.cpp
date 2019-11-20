// RedisDB.cpp

#include "RedisDB.h"

namespace dbbenchmark{
namespace redisdb {

using namespace dbbenchmark::utility::programconfigurations;

void RedisDB::init() {
    // PORT
    auto port = m_localConf->getString(PORT_PROPERTY, DEFAULT_PORT);
    // IP
    auto hostIP = m_localConf->getString(HOST_PROPERTY, "192.168.33.10");
    bool clusterEnabled = m_localConf->getBool(CLUSTER_PROPERTY, false);
    // Create FULL Redis Address
    auto redisAddr = hostIP + ":" + port;
    // CONN_TIMEOUT
    int connTimeout = m_localConf->getUInt(CONN_TIMEOUT);
    // RW_TIMEOUT
    int rwTimeout = m_localConf->getUInt(RW_TIMEOUT);
    // MAX_CONNS
    int maxConns = m_localConf->getUInt(MAX_CONNS);

    if (clusterEnabled) {
        // Now create Redis Connection
        redisClientCluster->set(redisAddr.c_str(), maxConns, connTimeout, rwTimeout);
        redisClient = new acl::redis_client(redisAddr.c_str(), connTimeout, rwTimeout);
        redisConnection = std::make_unique<acl::redis_connection>(redisClient);

        cmdHash = std::make_unique<acl::redis_hash>();
        cmdHash->set_cluster(redisClientCluster, maxConns);
    } 
    else {
        redisClient = new acl::redis_client(redisAddr.c_str(), connTimeout, rwTimeout);
        redisConnection = std::make_unique<acl::redis_connection>(redisClient);
        cmdHash = std::make_unique<acl::redis_hash>();
        cmdHash->set_client(redisClient);
    }
    
    // Set password if any given
    std::string password =  m_localConf->getString(PASSWORD_PROPERTY, "");
    if (!password.empty()) {
        redisConnection->auth(password.c_str());
    }
}

void RedisDB::cleanup() {
    redisConnection->clear();
    if (redisConnection->quit() == false) {
        LOG(WARNING) << redisConnection->result_error();
        throw(std::runtime_error("Error in cleaning redisDB"));
    }
}

RedisDB::~RedisDB() {
    delete redisClient;
    delete redisClientCluster;
}

Status RedisDB::read(const std::string &table, const std::string &key, 
                        const std::vector<std::string>& fields, std::vector<stringPair> &result) {
    
    if (fields.empty()) {
        std::map<acl::string, acl::string> resultTemp;
        cmdHash->hgetall(key.c_str(), resultTemp);
        std::map<acl::string, acl::string>::iterator it;
        for(it = resultTemp.begin(); it != resultTemp.end(); ++it){
            std::string temp_first = static_cast<std::string>(it->first);
            std::string temp_second = static_cast<std::string>(it->second);

            stringPair stringPairTemp = std::make_pair(temp_first, temp_second);
            result.push_back(stringPairTemp);
        }
    } 
    else {
        std::vector<acl::string> resultTemp;
        std::vector<acl::string> fieldsTemp;

        for(auto it = fields.begin(); it != fields.end(); ++it) {
            acl::string tmpStr(*it->data());
            fieldsTemp.push_back(tmpStr);
        }
        bool isSuccessed = cmdHash->hmget(key.c_str(), fieldsTemp, &resultTemp);
        if(!isSuccessed){
            return Status::OK;
        }
    }
    return result.empty() ? Status::ERROR : Status::OK;  
}

Status RedisDB::scan(const std::string &table, const std::string &startkey, int recordcount, 
                        std::vector<std::string> &fields, std::vector<std::vector<stringPair>> &result) {
    // To-DO
    // Set<String> keys = cmdHash->result_status(INDEX_KEY, hash(startkey),
    // Double.POSITIVE_INFINITY, 0, recordcount);

    // HashMap<String, ByteIterator> values;
    // for (String key : keys) {
    // values = new HashMap<String, ByteIterator>();
    // read(table, key, fields, values);
    // result.add(values);
    // }
return Status::FORBIDDEN;
}

Status RedisDB::update(const std::string &table, const std::string &key, 
                        std::vector<stringPair> values) {
    std::vector<stringPair>::iterator it;
    std::vector<acl::string> namesTmp;
    std::vector<acl::string> valuesTmp;
    for(it = values.begin(); it != values.end(); ++it){
        namesTmp.push_back(it->first.c_str());
        valuesTmp.push_back(it->second.c_str());
    }
    if (cmdHash->hmset(key.c_str(), namesTmp, valuesTmp)) {
        return Status::OK;
    }
    return Status::ERROR;
}

Status RedisDB::insert(const std::string &table, const std::string &key, 
                        std::vector<stringPair> &values) {
    std::vector<stringPair>::iterator it;
    std::vector<acl::string> namesTmp;
    std::vector<acl::string> valuesTmp;
    for(it = values.begin(); it != values.end(); ++it){
        namesTmp.push_back(it->first.c_str());
        valuesTmp.push_back(it->second.c_str());
    }
    if (cmdHash->hmset(key.c_str(), namesTmp, valuesTmp)) {
        std::vector<std::pair<const char*, double>> tmpVector;
        tmpVector.push_back(std::make_pair(INDEX_KEY.c_str(), hash(key)));
        return Status::OK;
    }
    return Status::ERROR;
}

Status RedisDB::Delete(const std::string &table, const std::string &key) {
    return cmdHash->hdel_fields(key.c_str(), table.c_str()) == 0 ? Status::ERROR
        : Status::OK;
}

} // namespace redisdb
} // namespace dbtester