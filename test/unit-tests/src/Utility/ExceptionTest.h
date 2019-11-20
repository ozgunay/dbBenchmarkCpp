/** ExceptionTest.h
 *
 * Bu dosya CINAR projesinin bir parcasidir ve "CINAR Projesi Sozlesmesi"
 * hukumlerine tabidir. Bu dosyanin sozlesme hukumlerine aykiri kullanilmasi
 * ve herhangi bir ortama yetkisiz kopyalanmasi yasaktir.
 *
 * T.C. Cumhurbaskanligi Savunma Sanayii Baskanligi (SSB)
 *
 *
 * This file is a part of CINAR project and subjects to the conditions and
 * restrictions defined in "CINAR Project Agreement". Unauthorized use and
 * copying of this file via any medium are strictly prohibited.
 *
 * Presidency of The Republic of Turkey
 * Presidency of Defence Industries
 */

#ifndef _NRF_EXCEPTIONTEST_H_
#define _NRF_EXCEPTIONTEST_H_


#include <gtest/gtest.h>

#include "Utility/Exception.h"

using namespace Utility;

namespace testing {
namespace Exceptiontest {

TEST(ExceptionTest, defineExceptionWithCode) {
    ASSERT_ANY_THROW(
		try {
            Exception dataExp = NotImplementedException("Hello World!", 1);
            Exception expThrown = dataExp;
			expThrown.rethrow();
		} catch( const Exception& e ) {
			// and this tests that it has the correct message
			EXPECT_STREQ( "Exception", e.what() );
            EXPECT_EQ( "Hello World!", e.message() );
			throw;
		}
	);
}

TEST(ExceptionTest, defineExceptionWithArg) {
    ASSERT_ANY_THROW(
		try {
			throw CircularReferenceException("This is message to the World",
            "Extra message" , 1);
		} catch( const Exception& e ) {
			// and this tests that it has the correct message
			EXPECT_STREQ("Circular reference", e.what());
            EXPECT_EQ("This is message to the World: Extra message",e.message());
            EXPECT_EQ(1, e.code());
            EXPECT_EQ("Circular reference: This is message to the World: Extra message",
                e.displayText());
            EXPECT_STREQ("N7Utility26CircularReferenceExceptionE", e.className());
			throw;
		}
	);

}

TEST(ExceptionTest, assignmentOperator) {
    ASSERT_ANY_THROW(
		try {
            Exception dataExp = NotImplementedException("Hello World!", 1);
            Exception expThrown = CircularReferenceException("This is message to the World",
            "Extra message" , 1);
            EXPECT_STREQ("N7Utility9ExceptionE", expThrown.className());
			expThrown = dataExp;
			throw NotImplementedException("Data and not implemented exceptions occured."
                , dataExp);
		} catch( const std::exception& e ) {
			// and this tests that it has the correct message
			EXPECT_STREQ( "Not implemented", e.what() );
			throw;
		}
	);
}

TEST(ExceptionTest, nestedException) {
    ASSERT_ANY_THROW(
		try {
            Exception dataExp = PathSyntaxException(1);
			throw NotImplementedException("Data and not implemented exceptions occured."
                , dataExp);
		} catch( const std::exception& e ) {
			// and this tests that it has the correct message
			EXPECT_STREQ( "Not implemented", e.what() );
			throw;
		}
	);
}

} // namespace Exceptiontest
} // namespace Testing

#endif // _NRF_EXCEPTIONTEST_H_