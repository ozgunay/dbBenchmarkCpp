/** FileSystemUtilityTest.h
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

#ifndef _NRF_FILESYSTEMUTILITYTEST_H_
#define _NRF_FILESYSTEMUTILITYTEST_H_

#include <memory>
#include <boost/filesystem.hpp>
#include <gtest/gtest.h>

#include "Utility/FileSystemUtility.h"

using namespace Utility::Filesystem;

namespace testing {
namespace Filesystemutilitytest {

TEST(FileSystemUtiltyTest, CheckIfDirectoryValid) {
   const std::string denemedirectory = "/tmp/nrfutest";
   boost::filesystem::create_directories(denemedirectory);
   EXPECT_TRUE(checkIfDirectory(denemedirectory));
}
TEST(FileSystemUtiltyTest, CheckIfFileValid) {
   const std::string denemefile = "/tmp/nrfutest/deneme";
   boost::filesystem::ofstream ostream(denemefile);
   ostream << "THIS FILE WILL BE DELETED!";
   ostream.close();
   EXPECT_TRUE(checkIfFile(denemefile));
}
TEST(FileSystemUtiltyTest, CheckIfFileInvalid) {
   const std::string denemefileinvalid = "deneme";
   EXPECT_FALSE(checkIfFile(denemefileinvalid));
}
TEST(FileSystemUtiltyTest, CheckIfDirectoryInvalid) {
   const std::string deneme = "deneme";
   EXPECT_FALSE(checkIfDirectory(deneme));
}
TEST(FileSystemUtiltyTest, CheckIfFilePermissonDenied) {
   const std::string denemefileinvalid = "/proc/1/fd/1";
   EXPECT_FALSE(checkIfFile(denemefileinvalid));
}
TEST(FileSystemUtiltyTest, CheckIfDirectoryPermissonDenied) {
   const std::string deneme = "/proc/1/fd/1";
   EXPECT_FALSE(checkIfDirectory(deneme));
}
TEST(FileSystemUtiltyTest, CheckIfFileException) {
   ASSERT_ANY_THROW(
      const std::string deneme = '\0';
      checkIfFile(deneme);
   );
}
TEST(FileSystemUtiltyTest, CheckIfDirectoryException) {
   ASSERT_ANY_THROW(
      const std::string deneme = '\0';
      checkIfDirectory(deneme);
   );
}

} // namespace Propertiestest
} // namespace Testing
#endif // _NRF_FILESYSTEMUTILITYTEST_H_