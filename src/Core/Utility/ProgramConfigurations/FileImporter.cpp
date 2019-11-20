// FileImporter.cpp

#include <iostream>

#include "FileImporter.h"

using boost::convert;

namespace dbbenchmark {
namespace utility {
namespace programconfigurations {

const std::string FileImporter::WORKLOADS_DIRECTORY = "workloads/";

FileImporter::FileImporter(std::shared_ptr<MapConfiguration> mapConfigIn) :
    m_fileConfig(mapConfigIn) {
}

void FileImporter::readProperties() {
    using boost::property_tree::ptree;
    ptree pt;
    try {
        std::string extension = boost::filesystem::extension(m_fileDirectory);
        if(extension == ".xml") {
            read_xml(m_fileDirectory, pt);
        } else if(extension == ".json") {
            read_json(m_fileDirectory, pt);
        } else {
            throw UnsupportedFileExtention(extension);
        }
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        throw;
    }
    try {
        BOOST_FOREACH( ptree::value_type const& v, pt.get_child("DBSettings") ) {
            if(v.first == "db_ip") {
                m_fileConfig->setIp("DBSettings." + v.first, v.second.data());
            } else if(v.first == "port") {
                m_fileConfig->setUInt("DBSettings." + v.first, convert<unsigned int>(v.second.data()).value());
            } else if(v.first == "cluster") {
                m_fileConfig->setUInt("DBSettings." + v.first, convert<unsigned int>(v.second.data()).value());
            } else if(v.first == "conn_timeout") {
                m_fileConfig->setUInt("DBSettings." + v.first, convert<unsigned int>(v.second.data()).value());
            } else if(v.first == "rw_timeout") {
                m_fileConfig->setUInt("DBSettings." + v.first, convert<unsigned int>(v.second.data()).value());
            } else if(v.first == "max_conns") {
                m_fileConfig->setUInt("DBSettings." + v.first, convert<unsigned int>(v.second.data()).value());
            } else {
                m_fileConfig->setString("DBSettings." + v.first, v.second.data());
            }
        }
    } catch( const std::exception& e ) {
        LOG(WARNING) << e.what() << "Error in reading configuration file."
            "Default value used.";
    }
    m_workloadName = m_fileConfig->getString("workloadname", "workloada"); //Default is a
    try {
        loadWorkloadProperties();
    } catch(const std::exception& e) {
            std::cerr << e.what() << '\n';
        throw;
    }
}

void FileImporter::loadWorkloadProperties() {
    using boost::property_tree::ptree;
    ptree ptWorkload;
    std::string workloadFullDirectory = WORKLOADS_DIRECTORY + m_workloadName + ".xml";

    try {
        read_xml(workloadFullDirectory, ptWorkload);
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        throw;
    }
    BOOST_FOREACH( ptree::value_type const& v, ptWorkload.get_child("WorkloadSettings") ) {
        if(v.first == "recordcount") {
            m_fileConfig->setUInt("WorkloadSettings." + v.first, convert<unsigned int>(v.second.data()).value());
        } else if(v.first == "operationcount") {
            m_fileConfig->setUInt("WorkloadSettings." + v.first, convert<unsigned int>(v.second.data()).value());
        } else if(v.first == "readallfields") {
            m_fileConfig->setBool("WorkloadSettings." + v.first, convert<bool>(v.second.data()).value());
        } else if(v.first == "readproportion") {
            m_fileConfig->setDouble("WorkloadSettings." + v.first, convert<double>(v.second.data()).value());
        } else if(v.first == "updateproportion") {
            m_fileConfig->setDouble("WorkloadSettings." + v.first, convert<double>(v.second.data()).value());
        } else if(v.first == "scanproportion") {
            m_fileConfig->setDouble("WorkloadSettings." + v.first, convert<double>(v.second.data()).value());
        } else if(v.first == "insertproportion") {
            m_fileConfig->setDouble("WorkloadSettings." + v.first, convert<double>(v.second.data()).value());
        } else {
            m_fileConfig->setString("WorkloadSettings." + v.first, v.second.data());
        }
    }
}

bool FileImporter::setFileDirectory(const std::string &fileDirectoryIn) {
    if(checkIfFile(fileDirectoryIn)) {
        m_fileDirectory = fileDirectoryIn;
        m_fileConfig->setString("GeneralSettings.ConfigurationPath", fileDirectoryIn);
        return true;
    }
    else
        return false;
}

} // namespace programconfigurations
} // namespace utility
} // namespace dbbenchmark