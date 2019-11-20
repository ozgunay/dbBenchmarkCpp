// ZipfianGeneratorTest.h

#ifndef _DBBENCHMARK_ZIPFIANGENERATORTEST_H_
#define _DBBENCHMARK_ZIPFIANGENERATORTEST_H_

#include <gtest/gtest.h>
#include "Core/Generators/ZipfianGenerator.h"

using namespace dbbenchmark::generators;

namespace test {
namespace zipfiangeneratortest {

::testing::AssertionResult IsBetweenInclusive(int val, int a, int b) {
	if((val >= a) && (val <= b))
		return ::testing::AssertionSuccess();
	else
		return ::testing::AssertionFailure()
			<< val << " is outside the range " << a << " to " << b;
}

class ZipfianGeneratorTest  : public ::testing::TestWithParam<std::pair<uint64_t,uint64_t>> {
	virtual void SetUp() { return; }
	virtual void TearDown() { return; }
};

TEST_P(ZipfianGeneratorTest, NextValue) {
	ZipfianGenerator generator(std::get<0>(GetParam()), std::get<1>(GetParam()));
	auto value = generator.Next();
	EXPECT_TRUE(IsBetweenInclusive(value, std::get<0>(GetParam()), std::get<1>(GetParam())));
}

TEST_P(ZipfianGeneratorTest, LastValue) {
	ZipfianGenerator generator(std::get<0>(GetParam()), std::get<1>(GetParam()));
	for(int i = 0; i < 10; i++) {
		auto valueTmp = generator.Next();
		EXPECT_TRUE(IsBetweenInclusive(valueTmp, std::get<0>(GetParam()), std::get<1>(GetParam())));
	}
	auto value = generator.Last();
	EXPECT_TRUE(IsBetweenInclusive(value, std::get<0>(GetParam()), std::get<1>(GetParam())));
}

TEST_F(ZipfianGeneratorTest, CreatewithNumberofItems) {
	ZipfianGenerator generator(123);
	auto value = generator.Next();
	EXPECT_TRUE(IsBetweenInclusive(value, 0, 123));
}

std::pair<uint64_t,uint64_t> FormulaTable[] = {
	std::make_pair(static_cast<uint64_t>(0), static_cast<uint64_t>(2)),
	std::make_pair(static_cast<uint64_t>(10), static_cast<uint64_t>(50)),
	std::make_pair(static_cast<uint64_t>(100), static_cast<uint64_t>(200))
};

INSTANTIATE_TEST_CASE_P(InstantiationName, ZipfianGeneratorTest,::testing::ValuesIn(FormulaTable));

} // namespace zipfiangeneratortest
} // namespace test

#endif // _DBBENCHMARK_ZIPFIANGENERATORTEST_H_