/** LogTestHelper.h
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

#ifndef _NRF_LOGTESTHELPER_H_
#define _NRF_LOGTESTHELPER_H_

#include <memory>
#include <fstream>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <cerrno>
#include <unistd.h>
#include <map>

namespace testing {
namespace Logtesthelper {

std::string ReadContent(const std::string filename);
std::string ExtractContent(const std::map<long, std::string>& content);
bool Exists(const std::string content, const std::string expected);
bool DoesFileEntityExist(const std::string& pathToFile);

/// Ref: StackOverflow + http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
std::string ReadContent(const std::string filename) {
   std::ifstream readIn(filename.c_str(), std::ios::in | std::ios::binary);
   if (readIn) {
      std::shared_ptr<void> raii(nullptr, [&](void*) {
         readIn.close();
      });

      std::string contents;
      readIn.seekg(0, std::ios::end);
      contents.resize(readIn.tellg());
      readIn.seekg(0, std::ios::beg);
      readIn.read(&contents[0], contents.size());
      return contents;
   }
   throw errno;
}

std::string ExtractContent(const std::map<long, std::string>& content) {
   std::string extracted = "\n ";
   for (const auto& pair : content) {
      std::string file = pair.second;
      extracted += file + ", \n";
   }
   extracted += "\n";
   return extracted;
}

bool Exists(const std::string content, const std::string expected) {
   auto found = content.find(expected);
   return found != std::string::npos;
}

bool DoesFileEntityExist(const std::string& pathToFile) {
   int check = access(pathToFile.c_str(), F_OK);
   bool found = (0 == check);
   if (!found) {
      std::cerr << pathToFile << " was not found: " << std::strerror(errno) << std::endl;
   }
   return found;
}

} // namespace Logtesthelper
} // namesapce Testing

#endif // _NRF_LOGTESTHELPER_H_