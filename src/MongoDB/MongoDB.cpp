// MongoDB.cpp

#include "MongoDB.h"

namespace dbbenchmark {
namespace mongodb {

mongocxx::instance instance{};

void MongoDB::init() {
    // PORT
    auto port = m_localConf->getString(PORT_PROPERTY, DEFAULT_PORT);
    // IP
    std::string hostIP = m_localConf->getString(HOST_PROPERTY, "localhost");
    // cluster enabled
    // bool clusterEnabled = m_localConf->getBool(CLUSTER_PROPERTY, false);
    // Create FULL Redis Address
    std::string redisAddr = hostIP + ":" + port;
    // // CONN_TIMEOUT
    // int connTimeout = m_localConf->getUInt(CONN_TIMEOUT, 0);
    // // RW_TIMEOUT
    // int rwTimeout = m_localConf->getUInt(RW_TIMEOUT, 0);
    // // MAX_CONNS
    // int maxConns = m_localConf->getUInt(MAX_CONNS, 0);
    //USERNAME
    std::string username = m_localConf->getString(USERNAME_PROPERTY, "");
    //PASSWORD
    std::string password = m_localConf->getString(PASSWORD_PROPERTY, "");

    //"mongodb://myTester:xyz123@127.0.0.1:27017/?authSource=my_database"
    std::string mongoDBAddr;
    mongoDBAddr.append("mongodb://");
    mongoDBAddr.append(username);
    mongoDBAddr.append(":");
    mongoDBAddr.append(password);
    mongoDBAddr.append("@");
    mongoDBAddr.append(hostIP);
    mongoDBAddr.append(":");
    mongoDBAddr.append(port);
    mongoDBAddr.append("/?authSource=mydb");

    this->uri = new mongocxx::uri(mongoDBAddr);
    this->client = new mongocxx::client(*uri);
    this->db = new mongocxx::database((*client)["test"]);
}

void MongoDB::cleanup() {
}

MongoDB::~MongoDB() {
    delete uri;
    delete client;
    delete db;
}

Status MongoDB::read(const std::string &table, const std::string &key, 
                        const std::vector<std::string> &fields, std::vector<stringPair> &result) {
    if (fields.empty()) {
        auto cursor = (*db)[table].find(make_document(kvp("_id", key)));
        for (auto &&doc : cursor) {
            for (bsoncxx::document::element ele : doc) {
                // element is non owning view of a key-value pair within a document.
                // we can use the key() method to get a string_view of the key.
                std::string field_key{ele.key()};
                bsoncxx::types::value ele_val{ele.get_value()};
                std::string temp_first = field_key;
                std::string temp_second = ele_val.get_utf8().value.to_string();
                stringPair stringPairTemp = std::make_pair(temp_first, temp_second);
                result.push_back(stringPairTemp);
            }
        }
    }
    else {
        auto document = bsoncxx::builder::basic::document{};
        for(auto it = fields.begin(); it != fields.end(); ++it) {
            document.append(kvp(*it, 1));  // string literal value will be converted to b_utf8 automatically
        }
        auto cursor = (*db)[table].find(make_document(kvp("_id", key)),
                                    mongocxx::options::find{}.projection(document.view()));
        for (auto &&doc : cursor) {
            nlohmann::json j = bsoncxx::to_json(doc);
            
            for (bsoncxx::document::element ele : doc) {
                // element is non owning view of a key-value pair within a document.
                // we can use the key() method to get a string_view of the key.
                std::string field_key{ele.key()};
                bsoncxx::types::value ele_val{ele.get_value()};
                std::string temp_first = field_key;
                std::string temp_second = ele_val.get_utf8().value.to_string();
                stringPair stringPairTemp = std::make_pair(temp_first, temp_second);
                result.push_back(stringPairTemp);
            }
        }
    }
    return result.empty() ? Status::ERROR : Status::OK;  
}

Status MongoDB::scan(const std::string &table, const std::string &startkey, int recordcount, 
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

Status MongoDB::update(const std::string &table, const std::string &key, 
                        std::vector<stringPair> values) {
    auto document = bsoncxx::builder::basic::document{};

    std::vector<stringPair>::iterator it;
    std::string tempkey;
    std::string tempvalue;

    for(it = values.begin(); it != values.end(); ++it){
        tempkey = it->first;
        tempvalue = it->second;
        document.append(kvp("$set", make_document(kvp(tempkey, tempvalue))));
    }

    bsoncxx::stdx::optional<mongocxx::result::update> result = (*db)[table].update_one(
                        make_document(kvp("_id", key)),
                        document.view());
    if(result) {
        return Status::OK;
    }
    else {
        return Status::ERROR;
    }

}
Status MongoDB::insert(const std::string &table, const std::string &key, 
                        std::vector<stringPair> &values) {
    auto document = bsoncxx::builder::basic::document{};
    std::vector<stringPair>::iterator it;
    document.append(kvp("_id", key));
    std::string tempKey, tempValue;

    for(it = values.begin(); it != values.end(); ++it){
        tempKey = it->first;
        tempValue = it->second;
        document.append(kvp(tempKey, tempValue));
    }

    auto result = (*db)[table].insert_one(document.view());
    if(result) {
        return Status::OK;
    }
    else {
        return Status::ERROR;
    }
}

Status MongoDB::Delete(const std::string &table, const std::string &key) {
    
    bsoncxx::stdx::optional<mongocxx::result::delete_result> result = 
                                        (*db)[table].delete_one(make_document(kvp("_id", key)));
    if(result) {
        return Status::OK;
    }
    else {
        return Status::ERROR;
    }
}

} // namespace mongodb
} // namespace dbtester