// CassandraDB.h

#ifndef _DBBENCHMARK_CASSANDRADB_H_
#define _DBBENCHMARK_CASSANDRADB_H_

#include "Core/DB.h"

#include <memory>
#include <cassandra.h>

#include "Core/Utility/ProgramConfigurations/LayeredConfiguration.h"

namespace dbbenchmark {
namespace cassandradb {
/**
*   \brief CassandraDB client wrapper for Datastax C++ library.
*   \details  Database accessing layer. Each thread in the client will be given its own instance of whatever DB 
*     class is to be used in the test. This class should be constructed using a no-argument 
*     constructor, so we can load it dynamically. Any argument-based initialization 
*     should be done by init().
*   \author Özgün AY
*   \version 1.0
*   \date 27/11/2018
*   \bug None so far
*/
const std::string SETTINGS_TAG = "DBSettings.";
const std::string YCSB_KEY = SETTINGS_TAG + "y_id";
const std::string KEYSPACE_PROPERTY = SETTINGS_TAG + "keyspace";
const std::string KEYSPACE_PROPERTY_DEFAULT = "ycsb";
const std::string USERNAME_PROPERTY = SETTINGS_TAG + "username";
const std::string PASSWORD_PROPERTY = SETTINGS_TAG + "password";

const std::string HOSTS_PROPERTY = SETTINGS_TAG + "host";
const std::string PORT_PROPERTY = SETTINGS_TAG + "port";
const int PORT_PROPERTY_DEFAULT = 9042;
const unsigned int MAX_NUMBER_OF_CONNECTION = 2;

const std::string READ_CONSISTENCY_LEVEL_PROPERTY = SETTINGS_TAG + "readconsistencylevel";
const std::string READ_CONSISTENCY_LEVEL_PROPERTY_DEFAULT = "ONE";
const std::string WRITE_CONSISTENCY_LEVEL_PROPERTY = SETTINGS_TAG + "writeconsistencylevel";
const std::string WRITE_CONSISTENCY_LEVEL_PROPERTY_DEFAULT = "ONE";

const std::string MAX_CONNECTIONS_PROPERTY = SETTINGS_TAG + "max_conns";
const std::string CORE_CONNECTIONS_PROPERTY = SETTINGS_TAG + "coreconnections";
const std::string CONNECT_TIMEOUT_MILLIS_PROPERTY =	SETTINGS_TAG + "connecttimeoutmillis";
const std::string READ_TIMEOUT_MILLIS_PROPERTY = SETTINGS_TAG + "readtimeoutmillis";

const std::string TRACING_PROPERTY = SETTINGS_TAG + "tracing";
const std::string TRACING_PROPERTY_DEFAULT = SETTINGS_TAG + "false";

const std::string MYKEYTYPE = "text";

/**
*   \brief CassandraDB client wrapper for Datastax C++ library.
*   \details  Database accessing layer. Each thread in the client will be given its own instance of whatever DB 
*     class is to be used in the test. This class should be constructed using a no-argument
*     constructor, so we can load it dynamically. Any argument-based initialization 
*     should be done by init().
*   \author Deniz GAZIOGLU
*   \version 1.0
*   \date 10/12/2018
*   \bug None so far
*/
class CassandraDB : public DB {
public:
	CassandraDB() {
		m_cluster = nullptr;
		m_session = nullptr;
		m_keyspace = "";
		m_host = "";
	};
	~CassandraDB();
	
	void init() override;
	void cleanup() override;
	Status read(const std::string &table, const std::string &key,
					const std::vector<std::string>& fields, std::vector<stringPair> &result) override;
	Status scan(const std::string &table, const std::string &startkey, int recordcount,
					std::vector<std::string>& fields, std::vector<std::vector<stringPair>> &result) override;
	Status update(const std::string &table, const std::string &key, 
					std::vector<stringPair> values) override;
	Status insert(const std::string &table, const std::string &key, 
					std::vector<stringPair> &values) override;
	Status Delete(const std::string &table, const std::string &key) override;

	CassError ExecuteQuery(CassStatement*);
	void setKeyspace(std::string &in_keyspace);
	bool doesTableExist(const std::string &in_keyspace, const std::string &in_table);
	bool doesColumnExist(const std::string &in_keyspace, const std::string &in_table, const std::string &in_column);
	bool CreateTablewPrimaryKey(const std::string &in_keyspace, const std::string &in_table);
	bool CreateColumn(const std::string &in_keyspace, const std::string &in_table, const std::string &in_column);

protected:

private:
	utility::programconfigurations::LayeredConfiguration* m_localConf;
	std::string m_host;
	std::string m_keyspace;
	CassCluster *m_cluster;
	CassSession *m_session;
};

} // namespace dbbenchmark
} // namespace cassandradb

#endif // _DBBENCHMARK_CASSANDRADB_H_
