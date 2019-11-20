// FileImporter.h

#ifndef _DBBENCHMARK_FILEIMPORTER_H_
#define _DBBENCHMARK_FILEIMPORTER_H_

#include "PropertyImporter.h"

#include <boost/filesystem.hpp>

#include "Core/Utility/FileSystemUtility.h"
#include "Core/Utility/Exception.h"

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
class FileImporter : public PropertyImporter {
public:
    static const std::string WORKLOADS_DIRECTORY;
    explicit FileImporter(std::shared_ptr<MapConfiguration> mapConfigIn);
    void readProperties() override;
    bool setFileDirectory(const std::string &fileDirectoryIn);
    void loadWorkloadProperties();
private:
    std::string m_fileDirectory;        // member file directory
    std::shared_ptr<MapConfiguration> m_fileConfig;
    std::string m_workloadName;   //name of the workload selected
};

} // namespace programconfigurations
} // namespace utility
} // namespace dbbenchmark

#endif // _DBBENCHMARK_FILEIMPORTER_H_