/** LogExporterTest.h
 *
 * Bu dosya CINAR projesinin bir parcasidir ve "CINAR Projesi Sozlesmesi"
 * hukumlerine tabidir. Bu dosyanin sozlesme hukumlerine aykiri kullanilmasi
 * ve herhangi bir ortama yetkisiz kopyalanmasi yasaktir.
 *
 * T.C. Cumhurbaskanligi Savunma Sanayii Baskanligi (SSB)
 *
 * This file is a part of CINAR project and subjects to the conditions and
 * restrictions defined in "CINAR Project Agreement". Unauthorized use and
 * copying of this file via any medium are strictly prohibited.
 *
 * Presidency of The Republic of Turkey
 * Presidency of Defence Industries
 */

#ifndef _NRF_LOGEXPORTERTEST_H_
#define _NRF_LOGEXPORTERTEST_H_

#include <gtest/gtest.h>

#include "Utility/ProgramProperties/LogExporter.h"
#include "LogTestHelper.h"

using namespace Cinar::Logger;
using namespace testing::Logtesthelper;
using namespace Utility::ProgramProperties;
using namespace Utility::ProgramProperties::Configurations;

namespace testing {
namespace Logexportertest {

class LogExporterTest : public ::testing::Test {
public:
	LogExporterTest() {
		localConf = &(LayeredConfiguration::Instance());
		std::shared_ptr<MapConfiguration> mapConfCli( new MapConfiguration());
		localConf->add(mapConfCli, "CLI Configuration", 1);
		mapConfCli->setString("GeneralSettings.LogLevel", "INFO");
		mapConfCli->setBool("NFSettings.TLS_insecure", false);
		mapConfCli->setString("DBSettings.db_type", "mongo");
		mapConfCli->setUInt("GeneralSettings.HeartbeatSettings.HeartbeatInterval", 3);
		writer = std::make_unique<LogExporter>();
	}
	~LogExporterTest() override { }
	std::unique_ptr<LogExporter> writer;
	LayeredConfiguration* localConf;
	std::string fileName = "flushPropertiesTest";
	std::string directory = "/tmp/";
};

TEST_F(LogExporterTest, flushProperties) {
	std::string filenameTemp;
	Initializer& filterSinkPtr = Initializer::instance(this->fileName, this->directory);
	filenameTemp = filterSinkPtr.getLogFileName();
	this->writer->flushProperties();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	auto name = std::string{filenameTemp};
	auto content = ReadContent(name);
	EXPECT_TRUE(Exists(content, "GeneralSettings.LogLevel")) << content;
	EXPECT_TRUE(Exists(content, "HearbeatSettings.HeartbeatInterval")) << content;
	EXPECT_TRUE(Exists(content, "NFSettings.TLS_insecure")) << content;
	EXPECT_TRUE(Exists(content, "DBSettings.db_type")) << content;
}

} // namespace Logexportertest
} // namespace Testing

#endif // _NRF_LOGEXPORTERTEST_H_