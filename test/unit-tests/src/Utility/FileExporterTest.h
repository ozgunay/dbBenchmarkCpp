/** FileExporterTest.h
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

#ifndef _NRF_FILEEXPORTERTEST_H_
#define _NRF_FILEEXPORTERTEST_H_

#include <gtest/gtest.h>

#include "Utility/ProgramProperties/FileExporter.h"

using namespace Utility::Filesystem;
using namespace Utility::ProgramProperties;

namespace testing {
namespace Fileexportertest {

class FileExporterTest : public ::testing::Test {
public:
	FileExporterTest() {
		localConf = &(LayeredConfiguration::Instance());
        std::shared_ptr<MapConfiguration> mapConfXML( new MapConfiguration());
		localConf->add(mapConfXML, "XML Configuration", 1);
		mapConfXML->setString("GeneralSettings.LogLevel", "INFO");
		mapConfXML->setBool("NFSettings.TLS_insecure", false);
		mapConfXML->setString("DBSettings.db_type", "mongo");
		mapConfXML->setUInt("HearbeatSettings.HeartbeatInterval", 3);
		writer = std::make_unique<FileExporter>();
	}
	~FileExporterTest() override { }
	std::unique_ptr<FileExporter> writer;
	LayeredConfiguration* localConf;
};
TEST_F(FileExporterTest, SaveFile) {
	ASSERT_NO_THROW(
		try {
			this->writer->setFileDirectory("/tmp/settingsutest.xml");
			this->writer->flushProperties();
		}
		catch( const std::exception& e ) {
			// and this tests that it has the correct message
			EXPECT_STREQ( "dbsettings.xml: cannot open file", e.what() );
			throw;
		}
	);
}
TEST_F(FileExporterTest, SaveFileError) {
	ASSERT_ANY_THROW(
		try {
			this->writer->setFileDirectory("");
			this->writer->flushProperties();
		}
		catch( const std::exception& e ) {
			// and this tests that it has the correct message
			EXPECT_STREQ( "File extention not supported", e.what() );
			throw;
		}
	);
}
// TEST(FileExporterTest, setFileDirectoryInvalid) {
//      FileExporter writer;
//     EXPECT_FALSE(writer.setFileDirectory("???"));
// }

} // namespace Fileexportertest
} // namespace Testing

#endif // _NRF_FILEEXPORTERTEST_H_