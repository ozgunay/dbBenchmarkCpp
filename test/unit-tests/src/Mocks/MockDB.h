/** DBMock.h
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

#ifndef _NRF_MOCKDB_H_
#define _NRF_MOCKDB_H_

#include <nlohmann/json.hpp>
#include "gmock/gmock.h"

#include "Utility/DB/DB.h"

using namespace Utility::DB;

namespace testing {

class MockDB : public DB {
public:
	MOCK_METHOD0(init, void());
	MOCK_METHOD0(cleanup, void());
	MOCK_METHOD3(read, Status(const std::string& table, const std::string& value, nlohmann::json& j));
	MOCK_METHOD2(readAll, Status(const std::string& table, std::vector<nlohmann::json>& result));
	MOCK_METHOD4(scan, Status(const std::string& table,const std::multimap <std::string, nlohmann::json>& queryList, int& recordcount, std::vector<nlohmann::json>& result));
	MOCK_METHOD4(update, Status(const std::string &table, const std::string &key, const std::string &val,
									const std::vector<Cinar::NRFNFManagementService::Model::PatchItem>& patchItem));
	MOCK_METHOD3(insert, Status(const std::string &table, const std::string &key,
									nlohmann::json j));
	MOCK_METHOD3(Delete, Status(const std::string &table, const std::string &key, const std::string &val));
	MOCK_METHOD1(connect, void(boost::asio::deadline_timer& timer));
};

} // namespace testing

#endif // _NRF_MOCKDB_H_