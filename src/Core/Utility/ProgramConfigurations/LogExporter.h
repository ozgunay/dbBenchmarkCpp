// LogExporter.h

#ifndef _DBBENCHMARK_LOGEXPORTER_H_
#define _DBBENCHMARK_LOGEXPORTER_H_

#include "PropertyExporter.h"
#include "Core/Utility/LoggerSink.hpp"

namespace dbbenchmark {
namespace utility {
namespace programconfigurations {
/**
*   \brief Export properties to logger file and syslog.
*   \details Exporter class for export properties to log file and syslog
*           using G3Logger.
*   \author Ozgun AY
*   \version 1.0
*   \date 14/01/2019
*   \bug None so far
*/
class LogExporter : public PropertyExporter {
public:
    explicit LogExporter();
    void flushProperties() override;
};

} // namespace programconfigurations
} // namespace utility
} // namespace dbbenchmark

#endif // _DBBENCHMARK_LOGEXPORTER_H_
