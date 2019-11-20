// Client.h

#ifndef _DBBENCHMARK_CLIENT_H_
#define _DBBENCHMARK_CLIENT_H_

#include <string>

#include "DB.h"
#include "Workloads/CoreWorkload.h"
#include "Core/Utility/Utils.h"

using namespace dbbenchmark::utility;
using namespace dbbenchmark::workloads;

namespace dbbenchmark {
class Client {

  public:
    Client(std::shared_ptr<DB> db, std::shared_ptr<CoreWorkload> wl) : db(db), workload(wl) { }
    
    virtual bool DoInsert();
    virtual bool DoTransaction();
    
    virtual ~Client() { }
    
  protected:
    
    virtual Status TransactionRead();
    virtual Status TransactionReadModifyWrite();
    virtual Status TransactionScan();
    virtual Status TransactionUpdate();
    virtual Status TransactionInsert();
    
    std::shared_ptr<DB> db;
    std::shared_ptr<CoreWorkload> workload;
};

inline bool Client::DoInsert() {
  std::string key = this->workload->NextSequenceKey();
  std::vector<stringPair> pairs;
  this->workload->BuildValues(pairs);
  return (this->db->insert(this->workload->NextTable(), key, pairs) == Status::OK);
}

inline bool Client::DoTransaction() {
  Status tmpStatus;
  switch (this->workload->NextOperation()) {
    case READ:
      tmpStatus = TransactionRead();
      break;
    case UPDATE:
      tmpStatus = TransactionUpdate();
      break;
    case INSERT:
      tmpStatus = TransactionInsert();
      break;
    case SCAN:
      tmpStatus = TransactionScan();
      break;
    case READMODIFYWRITE:
      tmpStatus = TransactionReadModifyWrite();
      break;
    default:
      throw NotImplementedException("Operation request is not recognized!");
  }
  return (tmpStatus == Status::OK);
}

inline Status Client::TransactionRead() {
  const std::string &table = this->workload->NextTable();
  const std::string &key = this->workload->NextTransactionKey();
  std::vector<stringPair> result;
  std::vector<std::string> fields;
  if (!this->workload->read_all_fields()) {
    fields.push_back("field" + this->workload->NextFieldName());
    return this->db->read(table, key, fields, result);
  } else {
    return this->db->read(table, key, fields, result);
  }
}

inline Status Client::TransactionReadModifyWrite() {
  const std::string &table = this->workload->NextTable();
  const std::string &key = this->workload->NextTransactionKey();
  std::vector<stringPair> result;
  std::vector<std::string> fields;
  if (!this->workload->read_all_fields()) {
    
    fields.push_back("field" + this->workload->NextFieldName());
    this->db->read(table, key, fields, result);
  } else {
    this->db->read(table, key, fields, result);
  }

  std::vector<stringPair> values;
  if (this->workload->write_all_fields()) {
    this->workload->BuildValues(values);
  } else {
    this->workload->BuildUpdate(values);
  }
  return this->db->update(table, key, values);
}

inline Status Client::TransactionScan() {
  const std::string &table = this->workload->NextTable();
  const std::string &key = this->workload->NextTransactionKey();
  int len = this->workload->NextScanLength();
  std::vector<std::vector<stringPair>> result;
  std::vector<std::string> fields;
  if (!this->workload->read_all_fields()) {
    fields.push_back("field" + this->workload->NextFieldName());
    return this->db->scan(table, key, len, fields, result);
  } else {
    return this->db->scan(table, key, len, fields, result);
  }
}

inline Status Client::TransactionUpdate() {
  const std::string &table = this->workload->NextTable();
  const std::string &key = this->workload->NextTransactionKey();
  std::vector<stringPair> values;
  if (this->workload->write_all_fields()) {
    this->workload->BuildValues(values);
  } else {
    this->workload->BuildUpdate(values);
  }
  return this->db->update(table, key, values);
}

inline Status Client::TransactionInsert() {
  const std::string &table = this->workload->NextTable();
  const std::string &key = this->workload->NextSequenceKey();
  std::vector<stringPair> values;
  this->workload->BuildValues(values);
  return this->db->insert(table, key, values);
}

} // namespace dbtester

#endif // _DBBENCHMARK_CLIENT_H_
