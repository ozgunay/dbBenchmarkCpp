// BasicDB.h

#ifndef _DBBENCHMARK_BASICDB_H_
#define _DBBENCHMARK_BASICDB_H_

#include "DB.h"

#include <iostream>
#include <string>
#include <mutex>

namespace dbbenchmark {
namespace basicdb {

/**
*   \brief Simple Database example
*   \details  This class is for demonstration purposes only. This class simply includes
*     all the functionality of a DB class has to have. 
*   \author Ozgun AY
*   \version 1.0
*   \date 21/11/2018
*   \bug None so far
*/
class BasicDB : public DB {
public:
  void init() override{
    std::lock_guard<std::mutex> lock(mutex_);
    LOG(INFO) << "A new thread begins working.";
  }
  void cleanup() {};

  Status read(const std::string &table, const std::string &key, 
              const std::vector<std::string> &fields, std::vector<stringPair> &result) override {
    std::lock_guard<std::mutex> lock(mutex_);
    LOG(INFO) << "READ " << table << ' ' << key;
    if (!fields.empty()) {
      LOG(INFO) << " [ ";
      for (auto f : fields) {
        LOG(INFO) << f << ' ';
      }
      LOG(INFO) << ']';
    } else {
      LOG(INFO) << " < all fields >";
    }
    return Status::OK;
  }

  Status scan(const std::string &table, const std::string &startkey, int recordcount, 
              std::vector<std::string> &fields, std::vector<std::vector<stringPair>> &result) override {
    std::lock_guard<std::mutex> lock(mutex_);
    LOG(INFO) << "SCAN " << table << ' ' << startkey << " " << recordcount;
    if (!fields.empty()) {
      LOG(INFO) << " [ ";
      for (auto f : fields) {
        LOG(INFO) << f << ' ';
      }
      LOG(INFO) << ']';
    } else {
      LOG(INFO) << " < all fields >";
    }
    return Status::OK;
  }

  Status update(const std::string &table, const std::string &key, 
                std::vector<stringPair> values) override {
    std::lock_guard<std::mutex> lock(mutex_);
    LOG(INFO) << "UPDATE " << table << ' ' << key << " [ ";
    for (auto v : values) {
      LOG(INFO) << v.first << '=' << v.second << ' ';
    }
    LOG(INFO) << ']';
    return Status::OK;
  }

  Status insert(const std::string &table, const std::string &key, 
                std::vector<stringPair> &values) override {
    std::lock_guard<std::mutex> lock(mutex_);
    LOG(INFO) << "INSERT " << table << ' ' << key << " [ ";
    for (auto v : values) {
      LOG(INFO) << v.first << '=' << v.second << ' ';
    }
    LOG(INFO) << ']';
    return Status::OK;
  }

  Status Delete(const std::string &table, const std::string &key) override {
    std::lock_guard<std::mutex> lock(mutex_);
    LOG(INFO) << "DELETE " << table << ' ' << key;
    return Status::OK; 
  }

private:
  std::mutex mutex_;
};

} // namespace basicdb
} // namespace dbtester 

#endif // _DBBENCHMARK_BASICDB_H_