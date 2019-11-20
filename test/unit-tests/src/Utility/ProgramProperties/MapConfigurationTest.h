/** MapConfigurationTest.h */

#ifndef _DBBENCHMARK_MAPCONFIGURATIONTEST_H_
#define _DBBENCHMARK_MAPCONFIGURATIONTEST_H_

#include <gtest/gtest.h>

#include "Core/Utility/ProgramConfigurations/MapConfiguration.h"

using namespace dbbenchmark::utility::programconfigurations;

namespace test {
namespace mapconfigurationtest {

class MapConfigurationTest : public ::testing::Test {
public:
	MapConfigurationTest() {
        mapConfXML = std::make_shared<MapConfiguration>();
	}
	~MapConfigurationTest() override { }
    std::shared_ptr<MapConfiguration> mapConfXML;
};

TEST_F(MapConfigurationTest, hasProperty) {
	this->mapConfXML->setString("MapConfTest", "DONE");
	this->mapConfXML->setBool("MapConfTest.InProgress", true);
	EXPECT_TRUE(this->mapConfXML->hasProperty("MapConfTest.InProgress"));
	EXPECT_TRUE(this->mapConfXML->hasProperty("MapConfTest"));
	EXPECT_FALSE(this->mapConfXML->hasProperty("Test"));
	EXPECT_TRUE(this->mapConfXML->has("MapConfTest.InProgress"));
	EXPECT_FALSE(this->mapConfXML->has("Test"));
	EXPECT_TRUE(this->mapConfXML->hasOption("MapConfTest.InProgress"));
	EXPECT_FALSE(this->mapConfXML->hasOption("Test"));
}

TEST_F(MapConfigurationTest, getString) {
	std::string valueTemp = "DONE";
	this->mapConfXML->setString("MapConfTest", valueTemp);
	EXPECT_EQ(this->mapConfXML->getString("MapConfTest"), valueTemp);
	EXPECT_EQ(this->mapConfXML->getString("MapConfTest", valueTemp), valueTemp);
}

TEST_F(MapConfigurationTest, getStringWithDefault) {
	std::string valueTemp = "DONE";
	EXPECT_EQ(this->mapConfXML->getString("MapConfTest", valueTemp), valueTemp);
}

TEST_F(MapConfigurationTest, getStringNotFound) {
	ASSERT_ANY_THROW(
		try {
			this->mapConfXML->getString("MapConfTest");
		} catch( const std::exception& e ) {
			// and this tests that it has the correct message
			EXPECT_STREQ( "Not found", e.what() );
			throw;
		}
	);
}

TEST_F(MapConfigurationTest, getInt) {
	int valueTemp = 1;
	this->mapConfXML->setInt("MapConfTest", valueTemp);
	EXPECT_EQ(this->mapConfXML->getInt("MapConfTest"), valueTemp);
}

TEST_F(MapConfigurationTest, getIntExpand) {
	std::string valueTemp = "TEST";
	this->mapConfXML->setString("MapConfTest", valueTemp);
	valueTemp = "CONF:${MapConfTest}";
	this->mapConfXML->setString("MapConfTestExpand", valueTemp);
	EXPECT_EQ(this->mapConfXML->getString("MapConfTestExpand"), "CONF:TEST");
}

TEST_F(MapConfigurationTest, getIntWithDefault) {
	int valueTemp = 1;
	EXPECT_EQ(this->mapConfXML->getInt("MapConfTest", valueTemp), valueTemp);
}

TEST_F(MapConfigurationTest, getIntNotFound) {
	ASSERT_ANY_THROW(
		try {
			this->mapConfXML->getInt("MapConfTest");
		} catch( const std::exception& e ) {
			// and this tests that it has the correct message
			EXPECT_STREQ( "Not found", e.what() );
			throw;
		}
	);
}

TEST_F(MapConfigurationTest, getIntSyntax) {
	std::string valueTemp = "TEST";
	this->mapConfXML->setString("MapConfTest", valueTemp);
	ASSERT_ANY_THROW(
		try {
			this->mapConfXML->getInt("MapConfTest");
		} catch( const std::exception& e ) {
			// and this tests that it has the correct message
			EXPECT_STREQ( "Attempted to access the value of an uninitialized optional object.",
				e.what() );
			throw;
		}
	);
}

TEST_F(MapConfigurationTest, getUInt) {
	unsigned int valueTemp = 1;
	this->mapConfXML->setUInt("MapConfTest", valueTemp);
	EXPECT_EQ(this->mapConfXML->getUInt("MapConfTest"), valueTemp);
	EXPECT_EQ(this->mapConfXML->getUInt("MapConfTest", valueTemp), valueTemp);
}

TEST_F(MapConfigurationTest, getUIntWithDefault) {
	unsigned int valueTemp = 1;
	EXPECT_EQ(this->mapConfXML->getUInt("MapConfTest", valueTemp), valueTemp);
}

TEST_F(MapConfigurationTest, getUIntNotFound) {
	ASSERT_ANY_THROW(
		try {
			this->mapConfXML->getUInt("MapConfTest");
		} catch( const std::exception& e ) {
			// and this tests that it has the correct message
			EXPECT_STREQ( "Not found", e.what() );
			throw;
		}
	);
}

TEST_F(MapConfigurationTest, getUIntSyntax) {
	std::string valueTemp = "TEST";
	this->mapConfXML->setString("MapConfTest", valueTemp);
	ASSERT_ANY_THROW(
		try {
			this->mapConfXML->getUInt("MapConfTest");
		} catch( const std::exception& e ) {
			// and this tests that it has the correct message
			EXPECT_STREQ( "Attempted to access the value of an uninitialized optional object.",
				e.what() );
			throw;
		}
	);
}

TEST_F(MapConfigurationTest, getDouble) {
	double valueTemp = 1.0;
	this->mapConfXML->setDouble("MapConfTest", valueTemp);
	EXPECT_EQ(this->mapConfXML->getDouble("MapConfTest"), valueTemp);
	EXPECT_EQ(this->mapConfXML->getDouble("MapConfTest", valueTemp), valueTemp);
}

TEST_F(MapConfigurationTest, getDoubleWithDefault) {
	double valueTemp = 1.0;
	EXPECT_EQ(this->mapConfXML->getDouble("MapConfTest", valueTemp), valueTemp);
}

TEST_F(MapConfigurationTest, getDoubleNotFound) {
	ASSERT_ANY_THROW(
		try {
			this->mapConfXML->getDouble("MapConfTest");
		} catch( const std::exception& e ) {
			// and this tests that it has the correct message
			EXPECT_STREQ( "Not found", e.what() );
			throw;
		}
	);
}

TEST_F(MapConfigurationTest, getDoubleSyntax) {
	std::string valueTemp = "TEST";
	this->mapConfXML->setString("MapConfTest", valueTemp);
	ASSERT_ANY_THROW(
		try {
			this->mapConfXML->getDouble("MapConfTest");
		} catch( const std::exception& e ) {
			// and this tests that it has the correct message
			EXPECT_STREQ( "Attempted to access the value of an uninitialized optional object.",
				e.what() );
			throw;
		}
	);
}

TEST_F(MapConfigurationTest, getBool) {
	bool valueTemp = true;
	this->mapConfXML->setBool("MapConfTest", valueTemp);
	EXPECT_EQ(this->mapConfXML->getBool("MapConfTest"), valueTemp);
	EXPECT_EQ(this->mapConfXML->getBool("MapConfTest", valueTemp), valueTemp);

	std::string boolWithString = "TRUE";
	this->mapConfXML->setString("MapConfTest", boolWithString);
	EXPECT_EQ(this->mapConfXML->getBool("MapConfTest"), valueTemp);

	boolWithString = "true";
	this->mapConfXML->setString("MapConfTest", boolWithString);
	EXPECT_EQ(this->mapConfXML->getBool("MapConfTest"), valueTemp);

	boolWithString = "TruE";
	this->mapConfXML->setString("MapConfTest", boolWithString);
	EXPECT_EQ(this->mapConfXML->getBool("MapConfTest"), valueTemp);

	boolWithString = "1";
	this->mapConfXML->setString("MapConfTest", boolWithString);
	EXPECT_NE(this->mapConfXML->getBool("MapConfTest"), valueTemp);
}

TEST_F(MapConfigurationTest, getBoolWithDefault) {
	bool valueTemp = true;
	EXPECT_EQ(this->mapConfXML->getBool("MapConfTest", valueTemp), valueTemp);
}

TEST_F(MapConfigurationTest, getBoolNotFound) {
	ASSERT_ANY_THROW(
		try {
			this->mapConfXML->getBool("MapConfTest");
		} catch( const std::exception& e ) {
			// and this tests that it has the correct message
			EXPECT_STREQ( "Not found", e.what() );
			throw;
		}
	);
}

TEST_F(MapConfigurationTest, getBoolSyntax) {
	std::string valueTemp = "TEST";
	this->mapConfXML->setString("MapConfTest", valueTemp);
	EXPECT_EQ(this->mapConfXML->getBool("MapConfTest"), false);
}

TEST_F(MapConfigurationTest, getLogLevel) {
	LEVELS valueTemp = INFO;
	this->mapConfXML->setLogLevel("MapConfTest", valueTemp);
	EXPECT_EQ(this->mapConfXML->getLogLevel("MapConfTest"), valueTemp);
	EXPECT_EQ(this->mapConfXML->getLogLevel("MapConfTest", valueTemp), valueTemp);

	this->mapConfXML->setLogLevel("MapConfTest", "INFO");
	EXPECT_EQ(this->mapConfXML->getLogLevel("MapConfTest"), valueTemp);

	valueTemp = DEBUG;
	this->mapConfXML->setLogLevel("MapConfTest", valueTemp);
	EXPECT_EQ(this->mapConfXML->getLogLevel("MapConfTest"), valueTemp);

	valueTemp = WARNING;
	this->mapConfXML->setLogLevel("MapConfTest", valueTemp);
	EXPECT_EQ(this->mapConfXML->getLogLevel("MapConfTest"), valueTemp);

	valueTemp = ERROR;
	this->mapConfXML->setLogLevel("MapConfTest", valueTemp);
	EXPECT_EQ(this->mapConfXML->getLogLevel("MapConfTest"), valueTemp);
}

TEST_F(MapConfigurationTest, getLogLevelWithDefault) {
	LEVELS valueTemp = INFO;
	EXPECT_EQ(this->mapConfXML->getLogLevel("MapConfTest", valueTemp), valueTemp);
}

TEST_F(MapConfigurationTest, getLogLevelNotFound) {
	ASSERT_ANY_THROW(
		try {
			this->mapConfXML->getLogLevel("MapConfTest");
		} catch( const std::exception& e ) {
			// and this tests that it has the correct message
			EXPECT_STREQ( "Not found", e.what() );
			throw;
		}
	);
}

TEST_F(MapConfigurationTest, getLogLevelSyntax) {
	std::string valueTemp = "TEST";
	this->mapConfXML->setString("MapConfTest", valueTemp);
	ASSERT_ANY_THROW(
		try {
			this->mapConfXML->getLogLevel("MapConfTest");
		} catch( const std::exception& e ) {
			// and this tests that it has the correct message
			EXPECT_STREQ( "Syntax error",
				e.what() );
			throw;
		}
	);
}

TEST_F(MapConfigurationTest, getIP) {
	std::string valueTemp = "192.168.10.10";
	this->mapConfXML->setIp("MapConfTest", valueTemp);
	EXPECT_EQ(this->mapConfXML->getString("MapConfTest"), valueTemp);
}

TEST_F(MapConfigurationTest, getIPSyntax) {
	std::string valueTemp = "TEST";
	ASSERT_ANY_THROW(
		try {
			this->mapConfXML->setIp("MapConfTest", valueTemp);
			this->mapConfXML->getString("MapConfTest");
		} catch( const std::exception& e ) {
			// and this tests that it has the correct message
			EXPECT_STREQ( "Syntax error",
				e.what() );
			throw;
		}
	);
}

TEST_F(MapConfigurationTest, getKeys) {
	std::map<std::string, std::string> tempMap;
	AbstractConfiguration::Keys keysTemp;
	std::string valueTemp = "TEST";
	std::string keyWithSub = "MapConfTest.Subkey";
	this->mapConfXML->setString(keyWithSub, valueTemp);
	this->mapConfXML->keys(keysTemp);
	for(auto const& it : keysTemp) {
		AbstractConfiguration::Keys tempSubKeys;
		this->mapConfXML->keys(it, tempSubKeys);
		for(auto const& subit : tempSubKeys) {
			tempMap[it + "." + subit] = this->mapConfXML->getString(it + "." + subit);
		}
	}
	EXPECT_EQ(this->mapConfXML->getString(keyWithSub), tempMap[keyWithSub]);
}

TEST_F(MapConfigurationTest, expand) {
	std::string valueTemp = "TEST";
	this->mapConfXML->setString("MapConfTest", valueTemp);
	std::string testString = "VALUE of the MapConfTest is: ${MapConfTest}";
	std::string result = this->mapConfXML->expand(testString);
	EXPECT_EQ(result, "VALUE of the MapConfTest is: TEST");
}

TEST_F(MapConfigurationTest, remove) {
	std::string valueTemp = "TEST";
	std::string keyTemp = "MapConfTest";
	this->mapConfXML->setString(keyTemp, valueTemp);
	EXPECT_EQ(this->mapConfXML->getString(keyTemp), valueTemp);
	this->mapConfXML->remove(keyTemp);
	EXPECT_FALSE(this->mapConfXML->has(keyTemp));
}

TEST_F(MapConfigurationTest, copyTo) {
	std::string valueTemp = "TEST";
	std::string keyTemp = "MapConfTest";
	this->mapConfXML->setString(keyTemp, valueTemp);
	EXPECT_EQ(this->mapConfXML->getString(keyTemp), valueTemp);
	MapConfiguration mapConfCli;
	this->mapConfXML->copyTo(mapConfCli);
	EXPECT_TRUE(mapConfCli.has(keyTemp));
}

TEST_F(MapConfigurationTest, clear) {
	std::string valueTemp = "TEST";
	std::string keyTemp = "MapConfTest";
	this->mapConfXML->setString(keyTemp, valueTemp);
	EXPECT_EQ(this->mapConfXML->getString(keyTemp), valueTemp);
	this->mapConfXML->clear();
	EXPECT_FALSE(this->mapConfXML->has(keyTemp));
}


} // namespace mapconfigurationtest
} // namespace test

#endif // _DBBENCHMARK_MAPCONFIGURATIONTEST_H_