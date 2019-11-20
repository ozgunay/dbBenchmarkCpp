// DBFactory.cpp

#include <string>
#include "DBFactory.h"

// Add new DB interfaces here
#include "Redis/RedisDB.h"
#include "MongoDB/MongoDB.h"
#include "Cassandra/CassandraDB.h"
#include "BasicDB.h"

using namespace dbbenchmark::utility::programconfigurations;
 
namespace dbbenchmark {
// Add namespaces here !
using namespace redisdb;
using namespace basicdb;
using namespace mongodb;
using namespace cassandradb;

std::shared_ptr<DB> DBFactory::CreateDB() {
    m_localConf = &(LayeredConfiguration::Instance());
    if (m_localConf->getString("DBSettings.dbname") == "basic") {
        auto DBCreated = std::make_shared<BasicDB>();
        DBCreated->init();
        return DBCreated;
    } else if (m_localConf->getString("DBSettings.dbname") == "redis") {
        auto DBCreated = std::make_shared<RedisDB>();
        DBCreated->init();
        return DBCreated;
    } else if (m_localConf->getString("DBSettings.dbname") == "mongodb") {
        auto DBCreated = std::make_shared<MongoDB>();
        DBCreated->init();
        return DBCreated;
    } else if (m_localConf->getString("DBSettings.dbname") == "cassandradb") {
            auto DBCreated = std::make_shared<CassandraDB>();
            DBCreated->init();
            return DBCreated;
    } else 
        return NULL;
}

} // namespace dbtester


