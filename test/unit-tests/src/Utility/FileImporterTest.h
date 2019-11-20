/** FileImporterTest.h
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

#ifndef _NRF_FILEIMPORTERTEST_H_
#define _NRF_FILEIMPORTERTEST_H_

#include <gtest/gtest.h>

#include "Utility/ProgramProperties/FileImporter.h"

using namespace Utility::ProgramProperties;

namespace testing {
namespace Fileimportertest {

class FileImporterTest : public ::testing::Test {
public:
	FileImporterTest() {
		localConf = &(LayeredConfiguration::Instance());
        std::shared_ptr<MapConfiguration> mapConfXML( new MapConfiguration());
		localConf->add(mapConfXML, "XML Configuration", 1);
		reader = std::make_unique<FileImporter>(mapConfXML);
	}
	~FileImporterTest() override { }
	std::unique_ptr<FileImporter> reader;
	LayeredConfiguration* localConf;
};
TEST_F(FileImporterTest, LoadFile) {
	ASSERT_NO_THROW(
		try {
			this->reader->setFileDirectory("settings.json");
			this->reader->readProperties();
		}
		catch( const std::exception& e ) {
			EXPECT_STREQ( "<unspecified file>: cannot open file", e.what() );
			throw;
		}
	);
}
TEST_F(FileImporterTest, LoadFileError) {
	ASSERT_ANY_THROW(
		try {
			this->reader->setFileDirectory("");
			this->reader->readProperties();
		}
		catch( const std::exception& e ) {
			EXPECT_STREQ( "File extention not supported", e.what() );
			throw;
		}
	);
}
TEST_F(FileImporterTest, setFileDirectoryInvalid) {
	EXPECT_FALSE(this->reader->setFileDirectory("???"));
}

} // namespace Fileimportertest
} // namespace Testing

#endif // _NRF_FILEIMPORTERTEST_H_