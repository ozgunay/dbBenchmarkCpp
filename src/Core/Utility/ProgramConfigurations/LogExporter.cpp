// LogExporter.cpp

#include <boost/date_time.hpp>

#include "LogExporter.h"

namespace dbbenchmark {
namespace utility {
namespace programconfigurations {

LogExporter::LogExporter() {
}

void LogExporter::flushProperties() {
    boost::posix_time::ptime timeLocal = boost::posix_time::second_clock::local_time();
    boost::gregorian::date dateObj = timeLocal.date();

    std::map<std::string, std::string> tempPropertiesMap;
    LayeredConfiguration& local = LayeredConfiguration::Instance();
    std::string base;
    auto confMap = local.getAllConfigurationsAsMap();
    LOG(INFO) << "Properties";
    LOG(INFO) << dateObj;
    for(auto const& it : confMap) {
        LOG(INFO) << it.first << " = " << it.second;
    }
}

} // namespace programconfigurations
} // namespace utility
} // namespace dbbenchmark