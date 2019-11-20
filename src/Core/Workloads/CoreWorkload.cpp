// CoreWorkload.cpp

#include <string>

#include "CoreWorkload.h"

#include "Core/Generators/UniformGenerator.h"
#include "Core/Generators/ZipfianGenerator.h"
#include "Core/Generators/ScrambledZipfianGenerator.h"
#include "Core/Generators/SkewedLatestGenerator.h"
#include "Core/Generators/ConstGenerator.h"

using std::string;

using namespace dbbenchmark::generators;
using namespace dbbenchmark::utility;
using namespace dbbenchmark::utility::programconfigurations;

namespace dbbenchmark {
namespace workloads {

const string CoreWorkload::WORKLOAD_KEY = "WorkloadSettings.";
const string CoreWorkload::TABLENAME_PROPERTY = 
    WORKLOAD_KEY + "table";
const string CoreWorkload::TABLENAME_DEFAULT = "usertable";

const string CoreWorkload::FIELD_COUNT_PROPERTY = 
    WORKLOAD_KEY + "fieldcount";
const int CoreWorkload::FIELD_COUNT_DEFAULT = 10;

const string CoreWorkload::FIELD_LENGTH_DISTRIBUTION_PROPERTY =
    WORKLOAD_KEY + "field_len_dist";
const string CoreWorkload::FIELD_LENGTH_DISTRIBUTION_DEFAULT = 
    "constant";

const string CoreWorkload::FIELD_LENGTH_PROPERTY = 
    WORKLOAD_KEY + "fieldlength";
const int CoreWorkload::FIELD_LENGTH_DEFAULT = 100;

const string CoreWorkload::READ_ALL_FIELDS_PROPERTY = 
    WORKLOAD_KEY + "readallfields";
const bool CoreWorkload::READ_ALL_FIELDS_DEFAULT = true;

const string CoreWorkload::WRITE_ALL_FIELDS_PROPERTY = 
    WORKLOAD_KEY + "writeallfields";
const bool CoreWorkload::WRITE_ALL_FIELDS_DEFAULT = false;

const string CoreWorkload::READ_PROPORTION_PROPERTY = 
    WORKLOAD_KEY + "readproportion";
const double CoreWorkload::READ_PROPORTION_DEFAULT = 0.95;

const string CoreWorkload::UPDATE_PROPORTION_PROPERTY = 
    WORKLOAD_KEY + "updateproportion";
const double CoreWorkload::UPDATE_PROPORTION_DEFAULT = 0.05;

const string CoreWorkload::INSERT_PROPORTION_PROPERTY = 
    WORKLOAD_KEY + "insertproportion";
const double CoreWorkload::INSERT_PROPORTION_DEFAULT = 0.0;

const string CoreWorkload::SCAN_PROPORTION_PROPERTY = 
    WORKLOAD_KEY + "scanproportion";
const double CoreWorkload::SCAN_PROPORTION_DEFAULT = 0.0;

const string CoreWorkload::READMODIFYWRITE_PROPORTION_PROPERTY =
    WORKLOAD_KEY + "readmodifywriteproportion";
const double CoreWorkload::READMODIFYWRITE_PROPORTION_DEFAULT = 0.0;

const string CoreWorkload::REQUEST_DISTRIBUTION_PROPERTY =
    WORKLOAD_KEY + "requestdistribution";
const string CoreWorkload::REQUEST_DISTRIBUTION_DEFAULT = "uniform";

const string CoreWorkload::MAX_SCAN_LENGTH_PROPERTY = 
    WORKLOAD_KEY + "maxscanlength";
const int CoreWorkload::MAX_SCAN_LENGTH_DEFAULT = 1000;

const string CoreWorkload::SCAN_LENGTH_DISTRIBUTION_PROPERTY =
    WORKLOAD_KEY + "scanlengthdistribution";
const string CoreWorkload::SCAN_LENGTH_DISTRIBUTION_DEFAULT = 
    "uniform";

const string CoreWorkload::INSERT_ORDER_PROPERTY = 
    WORKLOAD_KEY + "insertorder";
const string CoreWorkload::INSERT_ORDER_DEFAULT = 
    "hashed";

const string CoreWorkload::INSERT_START_PROPERTY = 
    WORKLOAD_KEY + "insertstart";
const int CoreWorkload::INSERT_START_DEFAULT = 0;

const string CoreWorkload::RECORD_COUNT_PROPERTY = 
    WORKLOAD_KEY + "recordcount";
const string CoreWorkload::OPERATION_COUNT_PROPERTY = 
    WORKLOAD_KEY + "operationcount";

void CoreWorkload::Init() {
  m_tableName = m_localConf->getString(TABLENAME_PROPERTY,TABLENAME_DEFAULT);
  
  m_fieldCount = m_localConf->getInt(FIELD_COUNT_PROPERTY,
                                        FIELD_COUNT_DEFAULT);
  m_fieldLenGenerator = GetFieldLenGenerator();
  
  double read_proportion = m_localConf->getDouble(READ_PROPORTION_PROPERTY,
                                                  READ_PROPORTION_DEFAULT);
  double update_proportion = m_localConf->getDouble(UPDATE_PROPORTION_PROPERTY,
                                                    UPDATE_PROPORTION_DEFAULT);
  double insert_proportion = m_localConf->getDouble(INSERT_PROPORTION_PROPERTY,
                                                    INSERT_PROPORTION_DEFAULT);
  double scan_proportion = m_localConf->getDouble(SCAN_PROPORTION_PROPERTY,
                                                  SCAN_PROPORTION_DEFAULT);
  double readmodifywrite_proportion = m_localConf->getDouble(
      READMODIFYWRITE_PROPORTION_PROPERTY, READMODIFYWRITE_PROPORTION_DEFAULT);
  
  try {
    m_recordCount = m_localConf->getInt(RECORD_COUNT_PROPERTY);
  }
  catch(const std::exception& e) {
    LOG(INFO) << "[recordcount property must be specified]" << std::endl;
    throw;
  }
  std::string request_dist = m_localConf->getString(REQUEST_DISTRIBUTION_PROPERTY,
                                          REQUEST_DISTRIBUTION_DEFAULT);
  int max_scan_len = m_localConf->getInt(MAX_SCAN_LENGTH_PROPERTY,
                                            MAX_SCAN_LENGTH_DEFAULT);
  std::string scan_len_dist = m_localConf->getString(SCAN_LENGTH_DISTRIBUTION_PROPERTY,
                                            SCAN_LENGTH_DISTRIBUTION_DEFAULT);
  int insert_start = m_localConf->getInt(INSERT_START_PROPERTY,
                                            INSERT_START_DEFAULT);
  
  m_readAllFields = m_localConf->getBool(READ_ALL_FIELDS_PROPERTY,
                                                    READ_ALL_FIELDS_DEFAULT);
  m_writeAllFields = m_localConf->getBool(WRITE_ALL_FIELDS_PROPERTY,
                                                    WRITE_ALL_FIELDS_DEFAULT);
  
  if (m_localConf->getString(INSERT_ORDER_PROPERTY, INSERT_ORDER_DEFAULT) == "hashed") {
    m_orderedInserts = false;
  } else {
    m_orderedInserts = true;
  }
  
  m_keyGenerator = new CounterGenerator(insert_start);
  
  if (read_proportion > 0) {
    m_opChooser.AddValue(READ, read_proportion);
  }
  if (update_proportion > 0) {
    m_opChooser.AddValue(UPDATE, update_proportion);
  }
  if (insert_proportion > 0) {
    m_opChooser.AddValue(INSERT, insert_proportion);
  }
  if (scan_proportion > 0) {
    m_opChooser.AddValue(SCAN, scan_proportion);
  }
  if (readmodifywrite_proportion > 0) {
    m_opChooser.AddValue(READMODIFYWRITE, readmodifywrite_proportion);
  }
  
  m_insertKeySequence.Set(m_recordCount);
  
  if (request_dist == "uniform") {
    m_keyChooser = new UniformGenerator(0, m_recordCount - 1);
    
  } else if (request_dist == "zipfian") {
    // If the number of keys changes, we don't want to change popular keys.
    // So we construct the scrambled zipfian generator with a keyspace
    // that is larger than what exists at the beginning of the test.
    // If the generator picks a key that is not inserted yet, we just ignore it
    // and pick another key.
    int op_count = m_localConf->getInt(OPERATION_COUNT_PROPERTY);
    int new_keys = (int)(op_count * insert_proportion * 2); // a fudge factor
    m_keyChooser = new ScrambledZipfianGenerator(m_recordCount + new_keys);
    
  } else if (request_dist == "latest") {
    m_keyChooser = new SkewedLatestGenerator(m_insertKeySequence);
    
  } else {
    throw InvalidArgumentException("Unknown request distribution: " + request_dist);
  }
  
  m_fieldChooser = new UniformGenerator(0, m_fieldCount - 1);
  
  if (scan_len_dist == "uniform") {
    m_scanLenChooser = new UniformGenerator(1, max_scan_len);
  } else if (scan_len_dist == "zipfian") {
    m_scanLenChooser = new ZipfianGenerator(1, max_scan_len);
  } else {
    throw IllegalStateException("Distribution not allowed for scan length: " +
        scan_len_dist);
  }
}

Generator<uint64_t> *CoreWorkload::GetFieldLenGenerator() {
  string field_len_dist = m_localConf->getString(FIELD_LENGTH_DISTRIBUTION_PROPERTY,
                                        FIELD_LENGTH_DISTRIBUTION_DEFAULT);
  int field_len = m_localConf->getInt(FIELD_LENGTH_PROPERTY,
                                          FIELD_LENGTH_DEFAULT);
  if(field_len_dist == "constant") {
    return new ConstGenerator(field_len);
  } else if(field_len_dist == "uniform") {
    return new UniformGenerator(1, field_len);
  } else if(field_len_dist == "zipfian") {
    return new ZipfianGenerator(1, field_len);
  } else {
    throw IllegalStateException("Unknown field length distribution: " +
        field_len_dist);
  }
}

void CoreWorkload::BuildValues(std::vector<stringPair> &values) {
  for (int i = 0; i < m_fieldCount; ++i) {
    stringPair pair;
    pair.first.append("field").append(std::to_string(i));
    pair.second.append(m_fieldLenGenerator->Next(), utility::RandomPrintChar());
    values.push_back(pair);
  }
}

void CoreWorkload::BuildUpdate(std::vector<stringPair> &update) {
  stringPair pair;
  pair.first.append(NextFieldName());
  pair.second.append(m_fieldLenGenerator->Next(), utility::RandomPrintChar());
  update.push_back(pair);
}

} // namespace workloads
} // namespace dbbenchmark