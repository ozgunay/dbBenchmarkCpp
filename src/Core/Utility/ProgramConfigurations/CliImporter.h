// CliImporter.h

#ifndef _DBBENCHMARK_CLIIMPORTER_H_
#define _DBBENCHMARK_CLIIMPORTER_H_

#include <boost/program_options.hpp>

#include "PropertyImporter.h"

namespace dbbenchmark {
namespace utility {
namespace programconfigurations {
/**
*   \brief Export global properties to a XML file
*   \details Export global properties object to a XML file full
*               path must be given first
*   \author Ozgun AY
*   \version 1.0
*   \date 07/01/2019
*   \bug None so far
*/
class CliImporter : public PropertyImporter {
public:
    /** Constructor.
    * @param generalProperties Shared pointer referance to set the signals
    * */
    explicit CliImporter(std::shared_ptr<MapConfiguration> mapConfIn);
    /** Read properties from CLI. This class is using boost::program_options.
    * @param generalProperties Shared pointer referance to set the signals
    * */
    void readProperties() override;
    /** Set ac and av arguments
    * @param ac Shared pointer referance to set the signals
    * @param av Shared pointer referance to set the signals
    * */
    void setCliParameters(const int ac, const char **av);
    /** Set ac and av arguments
    * @param ac Number of string pointed by argv
    * @param av Argument vector
    * */
    void overrideProperties();
protected:
    int m_ac; // Argument count
    const char **m_av; // Argument vector.
    boost::program_options::variables_map m_vm; // Map that stored readed CLI arguments
    std::shared_ptr<MapConfiguration> m_cliConfig;
};

} // namespace programconfigurations
} // namespace utility
} // namespace dbbenchmark

#endif // _DBBENCHMARK_CLIIMPORTER_H_
