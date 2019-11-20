/** LayeredConfigurationTest.h */

#ifndef _DBBENCHMARK_LAYEREDCONFIGURATIONTEST_H_
#define _DBBENCHMARK_LAYEREDCONFIGURATIONTEST_H_

#include <gtest/gtest.h>

#include "Core/Utility/ProgramConfigurations/MapConfiguration.h"
#include "Core/Utility/ProgramConfigurations/LayeredConfiguration.h"

using namespace dbbenchmark::utility::programconfigurations;

namespace test {
namespace layeredconfigurationtest {

class LayeredConfigurationTest : public ::testing::Test {
public:
	LayeredConfigurationTest() {
        layeredConf = &(LayeredConfiguration::Instance());
	}
	~LayeredConfigurationTest() override { }
    LayeredConfiguration* layeredConf;
};

TEST_F(LayeredConfigurationTest, addConfigurationWithLabel) {
	std::shared_ptr<MapConfiguration> mapConfXml( new MapConfiguration());
    this->layeredConf->add(mapConfXml, "XML Configuration");
	EXPECT_TRUE(this->layeredConf->find("XML Configuration"));
    this->layeredConf->removeConfiguration(mapConfXml);
}

TEST_F(LayeredConfigurationTest, removeRaw) {
	std::shared_ptr<MapConfiguration> mapConfXml( new MapConfiguration());
    this->layeredConf->addWriteable(mapConfXml, 1);
    std::string valueTempXML = "TESTXML";
	this->layeredConf->setString("LabeledTest", valueTempXML);
	EXPECT_EQ(this->layeredConf->getString("LabeledTest"), valueTempXML);
    this->layeredConf->remove("LabeledTest");
    EXPECT_FALSE(this->layeredConf->hasOption("LabeledTest"));
    this->layeredConf->removeConfiguration(mapConfXml);
}

TEST_F(LayeredConfigurationTest, addConfigurationToFront) {
    std::shared_ptr<MapConfiguration> mapConfXml( new MapConfiguration());
    std::shared_ptr<MapConfiguration> mapConfCli( new MapConfiguration());
    std::shared_ptr<MapConfiguration> mapConfGUI( new MapConfiguration());
    std::string valueTempXML = "TESTXML";
	mapConfXml->setString("LabeledTest", valueTempXML);
    std::string valueTempCLI = "TESTCLI";
	mapConfCli->setString("LabeledTest", valueTempCLI);
    std::string valueTempGUI = "TESTGUI";
	mapConfGUI->setString("LabeledTest", valueTempGUI);
    this->layeredConf->add(mapConfXml);
    this->layeredConf->addFront(mapConfCli);
    this->layeredConf->add(mapConfCli, 3);
	EXPECT_EQ(this->layeredConf->getString("LabeledTest"), valueTempCLI);
    this->layeredConf->removeConfiguration(mapConfXml);
    this->layeredConf->removeConfiguration(mapConfCli);
    this->layeredConf->removeConfiguration(mapConfGUI);
}

TEST_F(LayeredConfigurationTest, addWriteable) {
	std::shared_ptr<MapConfiguration> mapConfXml( new MapConfiguration());
    std::shared_ptr<MapConfiguration> mapConfCli( new MapConfiguration());
    this->layeredConf->addWriteable(mapConfXml, 2);
    this->layeredConf->add(mapConfCli, 1, true);
    std::string valueTempXML = "TESTXML";
	this->layeredConf->setString("LabeledTest", valueTempXML);
    std::string valueTempCLI = "TESTCLI";
	this->layeredConf->setString("LabeledTest", valueTempCLI);
	EXPECT_EQ(this->layeredConf->getString("LabeledTest"), valueTempCLI);
    this->layeredConf->removeConfiguration(mapConfXml);
    this->layeredConf->removeConfiguration(mapConfCli);
}

TEST_F(LayeredConfigurationTest, removeConfiguration) {
	std::shared_ptr<MapConfiguration> mapConfXml( new MapConfiguration());
    this->layeredConf->add(mapConfXml, "XML Configuration");
    this->layeredConf->removeConfiguration(mapConfXml);
	EXPECT_TRUE(this->layeredConf->find("XML Configuration"));
}

} // namespace layeredconfigurationtest
} // namespace test

#endif // _DBBENCHMARK_LAYEREDCONFIGURATIONTEST_H_