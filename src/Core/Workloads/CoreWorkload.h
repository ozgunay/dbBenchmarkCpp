// CoreWorkload.h

#ifndef _DBBENCHMARK_COREWORKLOAD_H_
#define _DBBENCHMARK_COREWORKLOAD_H_

#include <vector>
#include <string>

#include "Core/Utility/ProgramConfigurations/LayeredConfiguration.h"
#include "Core/Generators/DiscreteGenerator.h"
#include "Core/Generators/CounterGenerator.h"
#include "Core/Generators/Generator.h"
#include "Core/Utility/Exception.h"
#include "Core/Utility/Utils.h"
#include "Core/DB.h"

namespace dbbenchmark {
namespace workloads {

enum Operation {
  INSERT,
  READ,
  UPDATE,
  SCAN,
  READMODIFYWRITE
};
/**
*   \brief A trivial integer generator that always returns the same value.
*   \details The core benchmark scenario. Represents a set of clients doing simple CRUD operations. The
      * relative proportion of different kinds of operations, and other properties of the workload,
      * are controlled by parameters specified at runtime.
      * <p>
      * Properties to control the client:
      * <UL>
      * <LI><b>fieldcount</b>: the number of fields in a record (default: 10)
      * <LI><b>fieldlength</b>: the size of each field (default: 100)
      * <LI><b>minfieldlength</b>: the minimum size of each field (default: 1)
      * <LI><b>readallfields</b>: should reads read all fields (true) or just one (false) (default: true)
      * <LI><b>writeallfields</b>: should updates and read/modify/writes update all fields (true) or just
      * one (false) (default: false)
      * <LI><b>readproportion</b>: what proportion of operations should be reads (default: 0.95)
      * <LI><b>updateproportion</b>: what proportion of operations should be updates (default: 0.05)
      * <LI><b>insertproportion</b>: what proportion of operations should be inserts (default: 0)
      * <LI><b>scanproportion</b>: what proportion of operations should be scans (default: 0)
      * <LI><b>readmodifywriteproportion</b>: what proportion of operations should be read a record,
      * modify it, write it back (default: 0)
      * <LI><b>requestdistribution</b>: what distribution should be used to select the records to operate
      * on - uniform, zipfian, hotspot, sequential, exponential or latest (default: uniform)
      * <LI><b>minscanlength</b>: for scans, what is the minimum number of records to scan (default: 1)
      * <LI><b>maxscanlength</b>: for scans, what is the maximum number of records to scan (default: 1000)
      * <LI><b>scanlengthdistribution</b>: for scans, what distribution should be used to choose the
      * number of records to scan, for each scan, between 1 and maxscanlength (default: uniform)
      * <LI><b>insertstart</b>: for parallel loads and runs, defines the starting record for this
      * YCSB instance (default: 0)
      * <LI><b>insertcount</b>: for parallel loads and runs, defines the number of records for this
      * YCSB instance (default: recordcount)
      * <LI><b>zeropadding</b>: for generating a record sequence compatible with string sort order by
      * 0 padding the record number. Controls the number of 0s to use for padding. (default: 1)
      * For example for row 5, with zeropadding=1 you get 'user5' key and with zeropading=8 you get
      * 'user00000005' key. In order to see its impact, zeropadding needs to be bigger than number of
      * digits in the record number.
      * <LI><b>insertorder</b>: should records be inserted in order by key ("ordered"), or in hashed
      * order ("hashed") (default: hashed)
      * <LI><b>fieldnameprefix</b>: what should be a prefix for field names, the shorter may decrease the
      * required storage size (default: "field")
      * </ul>
*   \author Ozgun AY
*   \version 1.0
*   \date 21/11/2018
*   \bug None so far
*/
class CoreWorkload {
public:
  static const std::string WORKLOAD_KEY;
  /// 
  /// The name of the database table to run queries against.
  ///
  static const std::string TABLENAME_PROPERTY;
  static const std::string TABLENAME_DEFAULT;
  
  /// 
  /// The name of the property for the number of fields in a record.
  ///
  static const std::string FIELD_COUNT_PROPERTY;
  static const int FIELD_COUNT_DEFAULT;
  
  /// 
  /// The name of the property for the field length distribution.
  /// Options are "uniform", "zipfian" (favoring short records), and "constant".
  ///
  static const std::string FIELD_LENGTH_DISTRIBUTION_PROPERTY;
  static const std::string FIELD_LENGTH_DISTRIBUTION_DEFAULT;
  
  /// 
  /// The name of the property for the length of a field in bytes.
  ///
  static const std::string FIELD_LENGTH_PROPERTY;
  static const int FIELD_LENGTH_DEFAULT;
  
  /// 
  /// The name of the property for deciding whether to read one field (false)
  /// or all fields (true) of a record.
  ///
  static const std::string READ_ALL_FIELDS_PROPERTY;
  static const bool READ_ALL_FIELDS_DEFAULT;

  /// 
  /// The name of the property for deciding whether to write one field (false)
  /// or all fields (true) of a record.
  ///
  static const std::string WRITE_ALL_FIELDS_PROPERTY;
  static const bool WRITE_ALL_FIELDS_DEFAULT;
  
  /// 
  /// The name of the property for the proportion of read transactions.
  ///
  static const std::string READ_PROPORTION_PROPERTY;
  static const double READ_PROPORTION_DEFAULT;
  
  /// 
  /// The name of the property for the proportion of update transactions.
  ///
  static const std::string UPDATE_PROPORTION_PROPERTY;
  static const double UPDATE_PROPORTION_DEFAULT;
  
  /// 
  /// The name of the property for the proportion of insert transactions.
  ///
  static const std::string INSERT_PROPORTION_PROPERTY;
  static const double INSERT_PROPORTION_DEFAULT;
  
  /// 
  /// The name of the property for the proportion of scan transactions.
  ///
  static const std::string SCAN_PROPORTION_PROPERTY;
  static const double SCAN_PROPORTION_DEFAULT;
  
  ///
  /// The name of the property for the proportion of
  /// read-modify-write transactions.
  ///
  static const std::string READMODIFYWRITE_PROPORTION_PROPERTY;
  static const double READMODIFYWRITE_PROPORTION_DEFAULT;
  
  /// 
  /// The name of the property for the the distribution of request keys.
  /// Options are "uniform", "zipfian" and "latest".
  ///
  static const std::string REQUEST_DISTRIBUTION_PROPERTY;
  static const std::string REQUEST_DISTRIBUTION_DEFAULT;
  
  /// 
  /// The name of the property for the max scan length (number of records).
  ///
  static const std::string MAX_SCAN_LENGTH_PROPERTY;
  static const int MAX_SCAN_LENGTH_DEFAULT;
  
  /// 
  /// The name of the property for the scan length distribution.
  /// Options are "uniform" and "zipfian" (favoring short scans).
  ///
  static const std::string SCAN_LENGTH_DISTRIBUTION_PROPERTY;
  static const std::string SCAN_LENGTH_DISTRIBUTION_DEFAULT;

  /// 
  /// The name of the property for the order to insert records.
  /// Options are "ordered" or "hashed".
  ///
  static const std::string INSERT_ORDER_PROPERTY;
  static const std::string INSERT_ORDER_DEFAULT;

  static const std::string INSERT_START_PROPERTY;
  static const int INSERT_START_DEFAULT;
  
  static const std::string RECORD_COUNT_PROPERTY;
  static const std::string OPERATION_COUNT_PROPERTY;

  ///
  /// Initialize the scenario.
  /// Called once, in the main client thread, before any operations are started.
  ///
  virtual void Init();
  
  virtual void BuildValues(std::vector<stringPair> &values);
  virtual void BuildUpdate(std::vector<stringPair> &update);
  
  virtual std::string NextTable() { return m_tableName; }
  virtual std::string NextSequenceKey(); /// Used for loading data
  virtual std::string NextTransactionKey(); /// Used for transactions
  virtual Operation NextOperation() { return m_opChooser.Next(); }
  virtual std::string NextFieldName();
  virtual std::size_t NextScanLength() { return m_scanLenChooser->Next(); }
  
  bool read_all_fields() const { return m_readAllFields; }
  bool write_all_fields() const { return m_writeAllFields; }

  CoreWorkload() :
      m_fieldCount(0), m_readAllFields(false), m_writeAllFields(false),
      m_fieldLenGenerator(NULL), m_keyGenerator(NULL), m_keyChooser(NULL),
      m_fieldChooser(NULL), m_scanLenChooser(NULL), m_insertKeySequence(3),
      m_orderedInserts(true), m_recordCount(0) {
        m_localConf = &(utility::programconfigurations::LayeredConfiguration::Instance());
  }
  
  virtual ~CoreWorkload() {
    if (m_fieldLenGenerator) delete m_fieldLenGenerator;
    if (m_keyGenerator) delete m_keyGenerator;
    if (m_keyChooser) delete m_keyChooser;
    if (m_fieldChooser) delete m_fieldChooser;
    if (m_scanLenChooser) delete m_scanLenChooser;
  }
  
protected:
  dbbenchmark::generators::Generator<uint64_t> *GetFieldLenGenerator();
  std::string BuildKeyName(uint64_t key_num);

  std::string m_tableName;
  int m_fieldCount;
  bool m_readAllFields;
  bool m_writeAllFields;
  dbbenchmark::generators::Generator<uint64_t> *m_fieldLenGenerator;
  dbbenchmark::generators::Generator<uint64_t> *m_keyGenerator;
  dbbenchmark::generators::DiscreteGenerator<Operation> m_opChooser;
  dbbenchmark::generators::Generator<uint64_t> *m_keyChooser;
  dbbenchmark::generators::Generator<uint64_t> *m_fieldChooser;
  dbbenchmark::generators::Generator<uint64_t> *m_scanLenChooser;
  dbbenchmark::generators::CounterGenerator m_insertKeySequence;
  bool m_orderedInserts;
  std::size_t m_recordCount;

private:
  utility::programconfigurations::LayeredConfiguration* m_localConf;
};

inline std::string CoreWorkload::NextSequenceKey() {
  uint64_t key_num = m_keyGenerator->Next();
  return BuildKeyName(key_num);
}

inline std::string CoreWorkload::NextTransactionKey() {
  uint64_t key_num;
  do {
    key_num = m_keyChooser->Next();
  } while (key_num > m_insertKeySequence.Last());
  return BuildKeyName(key_num);
}

inline std::string CoreWorkload::BuildKeyName(uint64_t key_num) {
  if (!m_orderedInserts) {
    key_num = utility::Hash(key_num);
  }
  return std::string("user").append(std::to_string(key_num));
}

inline std::string CoreWorkload::NextFieldName() {
  return std::string("field").append(std::to_string(m_fieldChooser->Next()));
}

} // namespace workloads
} // namespace dbbenchmark

#endif // _DBBENCHMARK_COREWORKLOAD_H_
