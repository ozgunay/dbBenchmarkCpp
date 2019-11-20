// CliImporter.cpp

#include <iostream>

#include "CliImporter.h"

using boost::convert;

namespace dbbenchmark {
namespace utility {
namespace programconfigurations {

CliImporter::CliImporter(std::shared_ptr<MapConfiguration> mapConfIn) :
    m_cliConfig(mapConfIn){
    this->m_ac = 0;
    this->m_av = nullptr;
}
void CliImporter::readProperties() {
    boost::program_options::options_description generalSettings("GeneralSettings");
    generalSettings.add_options()
        ("help, h", "produce help message")
        ("confpath, f", boost::program_options::value<std::string>(),"configuration file path")
        ("loglevel, loglvl", boost::program_options::value<std::string>(),"logging severity level")
        ("threads, t", boost::program_options::value<std::string>(),"execute using n threads(default: 1)")
        ("dbname, db", boost::program_options::value<std::string>(),
            "specify the name of the DB to use (default: basic)");

    boost::program_options::options_description visible("Allowed options");
    visible.add(generalSettings);

    boost::program_options::command_line_parser cliParser{m_ac, m_av};
    cliParser.options(generalSettings).allow_unregistered().style(
                                boost::program_options::command_line_style::default_style |
                                boost::program_options::command_line_style::allow_slash_for_short);
    boost::program_options::parsed_options parsedOptions = cliParser.run();
    boost::program_options::store(parsedOptions, this->m_vm);
    boost::program_options::notify(this->m_vm);

    if (this->m_vm.count("help")) {
        std::cout << visible << "\n";
        throw HelpCalledException("Program stop executing.");
    }

    overrideProperties();
}
void CliImporter::setCliParameters(const int ac, const char **av) {
    m_ac = ac;
    m_av = av;
}
void CliImporter::overrideProperties() {
    if (m_vm.count("confpath")) {
        std::string confPathString = m_vm["confpath"].as<std::string>();
        try {
            checkIfFile(confPathString);
            std::cout << "Configuration file path set to: "
                << confPathString << "\n";
            m_cliConfig->setString("GeneralSettings.configurationpath", confPathString);
        } catch(const std::exception& e) {
            LOG(WARNING) << e.what() << "Configuration path is invalid."
                "Default value used.";
        }
    }
    if(m_vm.count("loglevel")) {
        std::string logLevelString = m_vm["loglevel"].as<std::string>();
        try {
            std::cout << "Logging level is set to: "
                << logLevelString << "\n";
            m_cliConfig->setLogLevel("GeneralSettings.loglevel", logLevelString);
        } catch(const std::exception& e) {
            LOG(WARNING) << e.what() << "Log level is invalid."
                "Default value used.";
        }
    }
    if (m_vm.count("threads")) {
        std::string tlsSecureString = m_vm["threads"].as<std::string>();
        try {
            std::cout << "Number of threads is set to: "
                << tlsSecureString << "\n";
            m_cliConfig->setBool("GeneralSettings.numberofthreads",
                convert<unsigned int>(tlsSecureString).value());
        } catch(const std::exception& e) {
            LOG(WARNING) << e.what() << "Number of threads is invalid."
                "Default value used.";
        }
    }
    if(m_vm.count("dbname")) {
        std::string logLevelString = m_vm["loglevel"].as<std::string>();
        try {
            std::cout << "DB Name is set to: "
                << logLevelString << "\n";
            m_cliConfig->setLogLevel("DBSettings.dbname", logLevelString);
        } catch(const std::exception& e) {
            LOG(WARNING) << e.what() << "DB Name is invalid."
                "Default value used.";
        }
    }

}

} // namespace programconfigurations
} // namespace utility
} // namespace dbbenchmark