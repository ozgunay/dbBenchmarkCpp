// PropertyExporter.h

#ifndef _DBBENCHMARK_PROPERTYEXPORTER_H_
#define _DBBENCHMARK_PROPERTYEXPORTER_H_

#include "Core/Utility/ProgramConfigurations/LayeredConfiguration.h"
#include "Core/Utility/FileSystemUtility.h"

namespace dbbenchmark {
namespace utility {
namespace programconfigurations {
/**
*   \brief Interface for export global properties
*   \details Export global properties object.(XML file, CLI, ...)
*   \author Ozgun AY
*   \version 1.0
*   \date 07/01/2019
*   \bug None so far
*/
class PropertyExporter {
public:
    /** Each implementaion will flush properties to proper place(to XML file or CLI)
     *  This function will throw an exception if not successfull.
     * @param globalProperties properties object to be flushed.*/
    virtual void flushProperties() = 0;
};

} // namespace programconfigurations
} // namespace utility
} // namespace dbbenchmark

#endif // _DBBENCHMARK_PROPERTYEXPORTER_H_
