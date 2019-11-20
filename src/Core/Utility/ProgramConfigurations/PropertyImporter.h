// PropertySetter.h

#ifndef _DBBENCHMARK_PROPERTYIMPORTER_H_
#define _DBBENCHMARK_PROPERTYIMPORTER_H_

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>

#include "Core/Utility/ProgramConfigurations/MapConfiguration.h"
#include "Core/Utility/FileSystemUtility.h"
#include "Core/Utility/Exception.h"

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
class PropertyImporter {
public:
    /** Each implementaion will read properties from proper place(from XML file or CLI) */
    virtual void readProperties() = 0;
};

} // namespace programconfigurations
} // namespace utility
} // namespace dbbenchmark

#endif // _DBBENCHMARK_PROPERTYIMPORTER_H_
