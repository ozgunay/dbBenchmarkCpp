/** CliImporterTest.h */

#ifndef _NRF_CLIIMPORTERTEST_H_
#define _NRF_CLIIMPORTERTEST_H_

#include <gtest/gtest.h>

#include "Utility/ProgramProperties/CliImporter.h"

using namespace Utility::ProgramProperties;
using namespace Utility::ProgramProperties::Configurations;

namespace testing {
namespace Cliimportertest {

class CliImporterTest  : public ::testing::Test {
public:
    CliImporterTest() {
        localConf = &(LayeredConfiguration::Instance());
        std::shared_ptr<MapConfiguration> mapConfCli( new MapConfiguration());
        localConf->add(mapConfCli, "CLI Configuration", 1);
        mapConfCli->setString("GeneralSettings.LogLevel", "NONE");
        testObject = std::make_unique<CliImporter>(mapConfCli);
    }
    ~CliImporterTest() override {
    }
    std::unique_ptr<CliImporter> testObject;
    LayeredConfiguration* localConf;
};

TEST_F(CliImporterTest, setParametersValid) {
    const char* tempArgv[4];
    tempArgv[0] = "./cnrnrftest";
    tempArgv[1] = "--confpath=al";
    tempArgv[2] = "--loglevel=INFO";
    tempArgv[3] = "--tlssecure=true";
    this->testObject->setCliParameters(4, tempArgv);
    this->testObject->readProperties();
    EXPECT_EQ(this->localConf->getString("GeneralSettings.LogLevel", "NONE"), "INFO");
    EXPECT_EQ(this->localConf->getString("GeneralSettings.ConfigurationPath", "NONE"), "al");
    EXPECT_TRUE(this->localConf->getBool("NFSettings.TLSSecure"));
}
TEST_F(CliImporterTest, setParametersInvalid) {
    const char* tempArgv[3];
    tempArgv[0] = "./cnrnrftest";
    tempArgv[1] = "--cnfpth=al";
    tempArgv[2] = "--lglvl=INFO";
    this->testObject->setCliParameters(3, tempArgv);
    this->testObject->readProperties();
    EXPECT_NE(this->localConf->getString("GeneralSettings.LogLevel"), "INFO");
}

TEST_F(CliImporterTest, setHelp) {
    const char* tempArgv[2];
    tempArgv[0] = "./cnrnrftest";
    tempArgv[1] = "--help";
    this->testObject->setCliParameters(2, tempArgv);
    ASSERT_ANY_THROW(
        try {
            this->testObject->readProperties();
            EXPECT_NE(this->localConf->getString("GeneralSettings.LogLevel"), "INFO");
        } catch( const std::exception& e ) {
			EXPECT_STREQ( "Help called.",
				e.what() );
			throw;
		}
   );
}

} // namespace Cliimportertest
} // namespace Testing

#endif // _NRF_CLIIMPORTERTEST_H_