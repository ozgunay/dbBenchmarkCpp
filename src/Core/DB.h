// DB.h

#ifndef _DBBENCHMARK_DB_H_
#define _DBBENCHMARK_DB_H_

#include <string>
#include <iterator>

#include "Core/Utility/ProgramConfigurations/LayeredConfiguration.h"
#include "Core/Utility/LoggerSink.hpp"
#include "Core/Utility/Utils.h"
#include "Status.h"

using namespace dbbenchmark::utility;

namespace dbbenchmark {
/**
*   \brief Interface for database objects.
*   \details  Database accessing layer. Each thread in the client will be given its own instance of whatever DB 
*     class is to be used in the test. This class should be constructed using a no-argument 
*     constructor, so we can load it dynamically. Any argument-based initialization 
*     should be done by init().
*   \author Ozgun AY
*   \version 1.0
*   \date 21/11/2018
*   \bug None so far
*/
class DB {
public:
    /** Init  DB. All the initializations done here. Must call this at least once.
    */
    virtual void init() = 0;
    /** Cleanup any state for this DB.
    */
    virtual void cleanup() = 0;
    /** Read a record from the database. Each field/value pair from the result will be stored in a HashMap.
    * @param table The name of the table
    * @param key The record key of the record to read.
    * @param fields The list of fields to read, or null for all of them
    * @param result A vector of field/value pairs for the result
    * @return The result of the operation.
    */
    virtual Status read(const std::string &table, const std::string &key, 
                        const std::vector<std::string> &fields, std::vector<stringPair> &result) = 0;
    /** Perform a range scan for a set of records in the database. Each field/value pair from the result will be stored
    * in a list.
    *
    * @param table The name of the table
    * @param startkey The record key of the first record to read.
    * @param recordcount The number of records to read
    * @param fields The list of fields to read, or null for all of them
    * @param result A Vector of results, where each HashMap is a set field/value pairs for one record
    * @return The result of the operation.
    */
    virtual Status scan(const std::string &table, const std::string &startkey, int recordcount, 
                        std::vector<std::string> &fields, std::vector<std::vector<stringPair>> &result) = 0;
    /** Update a record in the database. Any field/value pairs in the specified values will be written into the
    * record with the specified record key, overwriting any existing values with the same field name.
    * @param table The name of the table
    * @param key The record key of the record to write.
    * @param values A HashMap of field/value pairs to update in the record
    * @return The result of the operation.
    */
    virtual Status update(const std::string &table, const std::string &key, 
                        std::vector<stringPair> values) = 0;
    /** Insert a record in the database. Any field/value pairs in the specified values will be written into the
    * record with the specified record key.
    * @param table The name of the table
    * @param key The record key of the record to insert.
    * @param values A HashMap of field/value pairs to insert in the record
    * @return The result of the operation.
    */
    virtual Status insert(const std::string &table, const std::string &key, 
                        std::vector<stringPair> &values) = 0;
    /** Delete a record from the database.
    * @param table The name of the table
    * @param key The record key of the record to delete.
    * @return The result of the operation.
    */
    virtual Status Delete(const std::string &table, const std::string &startkey) = 0;

protected:
    utility::programconfigurations::LayeredConfiguration* m_localConf;   // Properties of DB.
};

} // namespace dbbenchmark

#endif // _DBBENCHMARK_DB_H_