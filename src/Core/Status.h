// Status.h

#ifndef _DBBENCHMARK_STATUS_H_
#define _DBBENCHMARK_STATUS_H_

#include <string>

namespace dbbenchmark {
namespace utility {
/**
*   \brief Result of the operations.
*   \details  Database accessing layer. Each thread in the client will be given its own instance of whatever DB 
*     class is to be used in the test. This class should be constructed using a no-argument 
*     constructor, so we can load it dynamically. Any argument-based initialization 
*     should be done by init().
*   \author Ozgun AY
*   \version 1.0
*   \date 21/11/2018
*   \bug None so far
*/
class Status {
public:
    static Status OK;
    static Status ERROR;
    static Status NOT_FOUND;
    static Status NOT_IMPLEMENTED;
    static Status UNEXPECTED_STATE;
    static Status BAD_REQUEST;
    static Status FORBIDDEN;
    static Status SERVICE_UNAVAILABLE;
    static Status BATCHED_OK;
    
public:
    /** Constructor.
     * @param name A short name for the status.
     * @param description A description of the status. */
    Status(const std::string &name = "", const std::string &description = "");
    /** 
     * @return Name of status.*/
    std::string getName() const { return this->name; };
    /**
     * 
     * @return Description of status.*/
    std::string getDescription() const { return this->description; };
    /**
     * Convert status to a string
     * @return Converted string.
     */
    std::string toString() const;
    /**
     * Calculate HashCode with std::hash.
     * @return Calculated hashcode.
     */
    int hashCode() const;
    /**
     * Compare status with other.
     * @param statusToCompare Status to compare.
     */
    bool isEqual(const Status& statusToCompare) const;
    /**
     * If status object is OK.
     * @return true if OK.
     */
    bool isOk() const;
protected:

private:
    std::string name;   // TODO_purpose_and_units.
    std::string description;   // TODO_purpose_and_units.
    /**
     * Is {@code this} a passing state for the operation: {@link Status#OK} or {@link Status#BATCHED_OK}.
     * @return true if the operation is successful, false otherwise
     */
    bool isOk() {
        return this->isEqual(Status::OK) || this->isEqual(Status::BATCHED_OK);
    }
};

inline bool operator==(const Status& lhs, const Status& rhs) { return lhs.isEqual(rhs); };

} // namespace utility
} // namespace dbbenchmark

#endif // _DBBENCHMARK_STATUS_H_