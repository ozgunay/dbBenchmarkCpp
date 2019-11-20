//CassandraDB.cpp

#include "CassandraDB.h"

using namespace dbbenchmark::utility;
using namespace dbbenchmark::utility::programconfigurations;

namespace dbbenchmark {
namespace cassandradb {

CassandraDB::~CassandraDB() {
	// Close the session
	CassFuture *close_future = cass_session_close(m_session);
	cass_future_wait(close_future);
	cass_future_free(close_future);
	// Free cluster and session
	if(m_cluster != nullptr) cass_cluster_free(m_cluster);
	if(m_session != nullptr) cass_session_free(m_session);
	m_cluster =nullptr;
	m_session = nullptr;
}

void CassandraDB::init() {
	m_localConf = &(LayeredConfiguration::Instance());
	// PORT
	int port = m_localConf->getInt(PORT_PROPERTY, PORT_PROPERTY_DEFAULT);
	// IP
	std::string hostIP = m_localConf->getString(HOSTS_PROPERTY, "192.168.33.10");
	// USERNAME
	std::string username = m_localConf->getString(USERNAME_PROPERTY);
	// PASSWORD
	std::string password = m_localConf->getString(PASSWORD_PROPERTY);
	std::string keyspace = m_localConf->getString(KEYSPACE_PROPERTY, KEYSPACE_PROPERTY_DEFAULT);
	unsigned int maxconn = m_localConf->getUInt(MAX_CONNECTIONS_PROPERTY, MAX_NUMBER_OF_CONNECTION);

	std::string queryStr;
	CassError rc;

	this->setKeyspace(keyspace);

	// Setup and connect to cluster
	m_cluster = cass_cluster_new();
	m_session = cass_session_new();

	// Configure Cluster
	cass_cluster_set_contact_points(m_cluster, hostIP.c_str());
	cass_cluster_set_port(m_cluster, port );
	cass_cluster_set_credentials(m_cluster, username.c_str(), password.c_str());
	cass_cluster_set_max_connections_per_host ( m_cluster, maxconn );

	// Session <-> Cluster
	CassFuture* connect_future
		= cass_session_connect(m_session, m_cluster);

	if (cass_future_error_code(connect_future) != CASS_OK) {
		const char *message;
		size_t message_length;
		cass_future_error_message(connect_future, &message, &message_length);
		return;
	}
	cass_future_free(connect_future);

	//Create Keyspace with 1 node
	queryStr = "CREATE KEYSPACE " + m_keyspace + " WITH REPLICATION = {'class' : 'SimpleStrategy','replication_factor':1}; ";
	CassStatement* statement1 = cass_statement_new(queryStr.c_str(), 0);
	if((rc=ExecuteQuery(statement1)) != CASS_OK)
		LOG(INFO) << "Query result for creating keyspace: " << m_keyspace << " "
			<< cass_error_desc(rc) << std::endl;

	//Use Keyspace
	queryStr = "USE " + m_keyspace + ";";
	CassStatement* statement2 = cass_statement_new(queryStr.c_str(), 0);
	if((rc=ExecuteQuery(statement2)) != CASS_OK)
		LOG(INFO) << "Query result for use: " << m_keyspace<< " " 
			<< cass_error_desc(rc) << std::endl;
	//std::cout<<"init() EXECUTED!!!"<<std::endl;

}

void CassandraDB::cleanup() {
	return;
}

Status CassandraDB::read(const std::string &table, const std::string &key,
						const std::vector<std::string>& fields, std::vector<stringPair> &result) {

	if((m_session == nullptr) || (m_cluster == nullptr))
		return Status::FORBIDDEN;

	if(!doesTableExist(m_keyspace, table))
		return Status::FORBIDDEN;

	std::map<std::string, std::string> columnAndValueMap;
	std::vector<std::string> columnNamesVec;

	// Get all columns except Primary Key
	if (fields.empty()){

		//Read column names and put them in column - value map
		std::string queryStr = "SELECT column_name FROM system_schema.columns WHERE keyspace_name = '" 
			+ m_keyspace + "' AND table_name = '" + table +"';";

		CassStatement* statement = cass_statement_new(queryStr.c_str(), 0);
		CassFuture* result_future = cass_session_execute(m_session, statement);
		cass_statement_free(statement);
		CassError rc = cass_future_error_code(result_future);
		if(rc != CASS_OK){
			return Status::FORBIDDEN;
		}

		if (cass_future_error_code(result_future) == CASS_OK) {
			// Retrieve result set and get the first row
			const CassResult* cassResult = cass_future_get_result(result_future);
			CassIterator* iterator = cass_iterator_from_result(cassResult);

			while (cass_iterator_next(iterator)) {
				const CassRow* row = cass_iterator_get_row(iterator);
				const CassValue* value_table = cass_row_get_column_by_name(row, "column_name");
				const char* value;
				size_t value_length;
				cass_value_get_string(value_table, &value, &value_length);
				columnNamesVec.push_back(value);
			}
			cass_iterator_free(iterator);
			cass_result_free(cassResult);
		} else {
			// Handle error
			const char* message;
			size_t message_length;
			cass_future_error_message(result_future, &message, &message_length);
			return Status::FORBIDDEN;
		}
	}
	else // get columns that are given in the "fields"
		columnNamesVec = fields;

	//When column names exist
	//read values associated with columns
	std::string queryStr = "SELECT * FROM " + m_keyspace + "." + table + " WHERE " + YCSB_KEY + " = '"+ key + "';";
	CassStatement* statement = cass_statement_new(queryStr.c_str(), 0);
	
	CassFuture* result_future = cass_session_execute(m_session, statement);
	cass_statement_free(statement);
	CassError rc = cass_future_error_code(result_future);
	if (rc != CASS_OK){
		return Status::FORBIDDEN;
	}

	if (cass_future_error_code(result_future) == CASS_OK) {
		// Retrieve result set and get the first row
		const CassResult* cassResult = cass_future_get_result(result_future);
		const CassRow* row = cass_result_first_row(cassResult);

		for (unsigned int i=0; i< columnNamesVec.size(); i++) {
			const CassValue* value_table = cass_row_get_column_by_name(row, (columnNamesVec[i]).c_str());
			const char* value;
			size_t value_length;
			cass_value_get_string(value_table, &value, &value_length);
			if(columnNamesVec[i]!=YCSB_KEY)
				columnAndValueMap.insert(std::pair<std::string,std::string>(columnNamesVec[i],value));
		}
		cass_result_free(cassResult);
	} else {
		// Handle error
		const char* message;
		size_t message_length;
		cass_future_error_message(result_future, &message, &message_length);
		return Status::FORBIDDEN;
	}

	return Status::OK;
}

Status CassandraDB::scan(const std::string &table, const std::string &startkey, int recordcount,
						std::vector<std::string>& fields, std::vector<std::vector<stringPair>> &result) {

	if((m_session == nullptr) || (m_cluster == nullptr))
		return Status::FORBIDDEN;

	if(!doesTableExist(m_keyspace,table)) {
			return Status::FORBIDDEN;
	}

	return Status::FORBIDDEN;
}

Status CassandraDB::update(const std::string &table, const std::string &key,
						std::vector<stringPair> values) {
	if((m_session == nullptr) || (m_cluster == nullptr))
		return Status::FORBIDDEN;

	if(!doesTableExist(m_keyspace,table)) {
		return Status::FORBIDDEN;
	}

	CassError rc;

	std::string queryStr1 = "UPDATE " + m_keyspace + "." + table + " SET ";
	std::string queryStr2 = "";
	std::string queryStr3 = " WHERE " + YCSB_KEY + " = '"+ key + "';";

	std::vector<stringPair>::iterator it;
	for(it = values.begin(); it != values.end(); ++it) {
		if(it == values.begin())
			queryStr2 = it->first + " = '" + it->second + "'";
		else
			queryStr2 += ", " + it->first + " = '" + it->second + "'";
	}

	std::string queryStr = queryStr1 + queryStr2 + queryStr3;
	CassStatement* statement = cass_statement_new(queryStr.c_str(), 0);
	if((rc=ExecuteQuery(statement)) != CASS_OK)
		return Status::FORBIDDEN;

	return Status::OK;
}

Status CassandraDB::insert(const std::string &table, const std::string &key,
						std::vector<stringPair> &values) {

	if((m_session == nullptr) || (m_cluster == nullptr))
		return Status::FORBIDDEN;

	if(!doesTableExist(m_keyspace, table)) {
		//Create table in 'm_keyspace' to store key and associated values
		CreateTablewPrimaryKey(m_keyspace, table);
	}

	CassError rc;
	std::string queryStr1 = "INSERT INTO " + m_keyspace + "." + table + " (" + YCSB_KEY ;
	std::string queryStr2 = "";
	std::string queryStr3 = "?";

	std::vector<stringPair>::iterator it;

	for(it = values.begin(); it != values.end(); ++it){
		if(!doesColumnExist(m_keyspace,table,it->first)) {
			CreateColumn(m_keyspace, table, it->first);
		}
		queryStr2 = queryStr2 + "," + it->first;
		queryStr3 += ",?";
	}

	std::string queryStr = queryStr1 + queryStr2 + ") VALUES (" + queryStr3 + ");";

	/* Prepare the statement on the Cassandra cluster */
	CassFuture* prepare_future = cass_session_prepare(m_session, queryStr.c_str());

	/* Wait for the statement to prepare and get the result */
	rc = cass_future_error_code(prepare_future);

	if (rc != CASS_OK) {
		/* Handle error */
		cass_future_free(prepare_future);
		return Status::FORBIDDEN;
	}

	/* Get the prepared object from the future */
	const CassPrepared* prepared = cass_future_get_prepared(prepare_future);

	/* The future can be freed immediately after getting the prepared object */
	cass_future_free(prepare_future);

	/* The prepared object can now be used to create statements that can be executed */
	CassStatement* statement = cass_prepared_bind(prepared);
	/* Bind Primary Key */
	cass_statement_bind_string_by_name(statement, YCSB_KEY.c_str(), key.c_str());

	/* Bind variables by name this time (this can only be done with prepared statements)*/
	for(it = values.begin(); it != values.end(); ++it) {
		cass_statement_bind_string_by_name(statement, it->first.c_str(), it->second.c_str());
	}

	/* Execute statement (same a the non-prepared code) */
	if((rc=ExecuteQuery(statement)) != CASS_OK) {
		/* The prepared object must be freed */
		cass_prepared_free(prepared);
		return Status::ERROR;
	}

	/* The prepared object must be freed */
	cass_prepared_free(prepared);
	return Status::OK;
}

Status CassandraDB::Delete(const std::string &table, const std::string &key) {
	if((m_session == nullptr) || (m_cluster == nullptr))
		return Status::FORBIDDEN;
	CassError rc;
	std::string queryStr = "DELETE FROM " + m_keyspace + "." + table + " WHERE " + YCSB_KEY + " = '"+ key + "';";
	CassStatement* statement = cass_statement_new(queryStr.c_str(), 0);
	if((rc=ExecuteQuery(statement)) != CASS_OK)
		return Status::FORBIDDEN;
	return Status::OK;
}

CassError CassandraDB::ExecuteQuery(CassStatement* statement) {
	CassFuture* query_future = cass_session_execute(m_session, statement);
	cass_statement_free(statement);
	CassError rc = cass_future_error_code(query_future);
	cass_future_free(query_future);
	return rc;
}

void CassandraDB::setKeyspace(std::string &in_keyspace){
	m_keyspace=in_keyspace;
}

bool CassandraDB::doesTableExist(const std::string &in_keyspace, const std::string &in_table) {
	const CassSchemaMeta* schema_meta = cass_session_get_schema_meta(m_session);
	const CassKeyspaceMeta* keyspace_meta = cass_schema_meta_keyspace_by_name(schema_meta, in_keyspace.c_str());
	if (keyspace_meta != NULL) {
		const CassTableMeta* table_meta = cass_keyspace_meta_table_by_name(keyspace_meta, in_table.c_str());
		if (table_meta != NULL) {
			cass_schema_meta_free(schema_meta);
			return true;
		}
	}
	cass_schema_meta_free(schema_meta);
	return false;
}

bool CassandraDB::doesColumnExist(const std::string &in_keyspace, const std::string &in_table, const std::string &in_column) {
	const CassSchemaMeta* schema_meta = cass_session_get_schema_meta(m_session);
	const CassKeyspaceMeta* keyspace_meta = cass_schema_meta_keyspace_by_name(schema_meta, in_keyspace.c_str());

	if (keyspace_meta != NULL) {
		const CassTableMeta* table_meta = cass_keyspace_meta_table_by_name(keyspace_meta, in_table.c_str());
		if (table_meta != NULL) {
			const CassColumnMeta* column_meta = cass_table_meta_column_by_name(table_meta, in_column.c_str());
			if (column_meta != NULL) {
				cass_schema_meta_free(schema_meta);
				return true;
			}
		}
	}
	cass_schema_meta_free(schema_meta);
	return false;
}

bool CassandraDB::CreateTablewPrimaryKey(const std::string &in_keyspace, const std::string &in_table) {
	CassError rc;
	std::string queryStr = "CREATE TABLE " + in_keyspace + "." + in_table + " (" + YCSB_KEY + " " + MYKEYTYPE + " PRIMARY KEY);" ;
	CassStatement* statement = cass_statement_new(queryStr.c_str(), 0);
	if((rc=ExecuteQuery(statement)) != CASS_OK) {
		return false;
	}
	return true;
}

bool CassandraDB::CreateColumn(const std::string &in_keyspace, const std::string &in_table, const std::string &in_column) {
	CassError rc;
	std::string queryStr = "ALTER TABLE " + in_keyspace + "." + in_table + " ADD " + in_column + " text;" ;
	CassStatement* statement = cass_statement_new(queryStr.c_str(), 0);
	if((rc=ExecuteQuery(statement)) != CASS_OK) {
		return false;
	}
	return true;
}

} // namespace cassandradb
} // namespace dbbenchmark
