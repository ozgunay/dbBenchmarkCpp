// ConstGeneratorTest.h

#ifndef _DBBENCHMARK_CONSTGENERATORTEST_H_
#define _DBBENCHMARK_CONSTGENERATORTEST_H_

#include <gtest/gtest.h>
#include "Core/Generators/ConstGenerator.h"

using namespace dbbenchmark::generators;

namespace test {
namespace constgeneratortest {
	
class ConstGeneratorTest  : public ::testing::TestWithParam<uint64_t> {
public:
	virtual void SetUp() { }
	virtual void TearDown() { }
};

TEST_P(ConstGeneratorTest, NextValue) {
	ConstGenerator generator(GetParam());
	EXPECT_EQ(GetParam(), generator.Next());
}
TEST_P(ConstGeneratorTest, LastValue) {
	ConstGenerator generator(GetParam());
	EXPECT_EQ(GetParam(), generator.Last());
}

INSTANTIATE_TEST_CASE_P(InstantiationValue, ConstGeneratorTest, ::testing::Values(static_cast<uint64_t>(0), static_cast<uint64_t>(2),
						static_cast<uint64_t>(12345)));
} // namespace constgeneratortest
} // namespace test

#endif // _DBBENCHMARK__CONSTGENERATORTEST_H_