// ScrambledZipfianGeneratorTest.h

#ifndef _DBBENCHMARK_SCRAMBLEDZIPFIANGENERATORTEST_H_
#define _DBBENCHMARK_SCRAMBLEDZIPFIANGENERATORTEST_H_

#include <gtest/gtest.h>
#include "Core/Generators/ScrambledZipfianGenerator.h"

using namespace dbbenchmark::generators;

namespace testing {
namespace scrambledzipfiangeneratortest {

::testing::AssertionResult IsBetweenInclusive(int val, int a, int b) {
	if((val >= a) && (val <= b))
		return ::testing::AssertionSuccess();
	else
		return ::testing::AssertionFailure()
			<< val << " is outside the range " << a << " to " << b;
}

class ScrambledZipfianGeneratorTest  : public ::testing::TestWithParam<std::pair<uint64_t,uint64_t>> {
	virtual void SetUp() { return; }
	virtual void TearDown() { return; }
};

TEST_P(ScrambledZipfianGeneratorTest, NextValue) {
	ScrambledZipfianGenerator generator(std::get<0>(GetParam()), std::get<1>(GetParam()));
	auto value = generator.Next();
	EXPECT_TRUE(IsBetweenInclusive(value, std::get<0>(GetParam()), std::get<1>(GetParam())));
}

TEST_P(ScrambledZipfianGeneratorTest, LastValue) {
	ScrambledZipfianGenerator generator(std::get<0>(GetParam()), std::get<1>(GetParam()));
	for(int i = 0; i < 10; i++) {
		auto valueTmp = generator.Next();
		EXPECT_TRUE(IsBetweenInclusive(valueTmp, std::get<0>(GetParam()), std::get<1>(GetParam())));
	}
	auto value = generator.Last();
	EXPECT_TRUE(IsBetweenInclusive(value, std::get<0>(GetParam()), std::get<1>(GetParam())));
}

TEST_F(ScrambledZipfianGeneratorTest, CreatewithNumberofItems) {
	ScrambledZipfianGenerator generator(123);
	auto value = generator.Next();
	EXPECT_TRUE(IsBetweenInclusive(value, 0, 123));
}

std::pair<uint64_t,uint64_t> FormulaTable[] = {
	std::make_pair(static_cast<uint64_t>(0), static_cast<uint64_t>(2)),
	std::make_pair(static_cast<uint64_t>(10), static_cast<uint64_t>(50)),
	std::make_pair(static_cast<uint64_t>(100), static_cast<uint64_t>(200))
};
INSTANTIATE_TEST_CASE_P(InstantiationName, ScrambledZipfianGeneratorTest,::testing::ValuesIn(FormulaTable));

} // namespace scrambledzipfiangeneratortest
} // namespace test

#endif // _DBBENCHMARK_SCRAMBLEDZIPFIANGENERATORTEST_H_