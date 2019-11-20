// UniformGeneratorTest.h

#ifndef _DBBENCHMARK_UNIFORMGENERATORTEST_H_
#define _DBBENCHMARK_UNIFORMGENERATORTEST_H_

#include <gtest/gtest.h>
#include "Core/Generators/UniformGenerator.h"

using namespace dbbenchmark::generators;

namespace test {
namespace uniformgeneratortest {

::testing::AssertionResult IsBetweenInclusive(int val, int a, int b) {
	if((val >= a) && (val <= b))
		return ::testing::AssertionSuccess();
	else
		return ::testing::AssertionFailure()
			<< val << " is outside the range " << a << " to " << b;
}

class UniformGeneratorTest  : public ::testing::TestWithParam<std::pair<uint64_t,uint64_t>> {
	virtual void SetUp() { return; }
	virtual void TearDown() { return; }
};

TEST_P(UniformGeneratorTest, NextValue) {
	UniformGenerator generator(std::get<0>(GetParam()), std::get<1>(GetParam()));
	auto value = generator.Next();
	EXPECT_TRUE(IsBetweenInclusive(value, std::get<0>(GetParam()), std::get<1>(GetParam())));
}

TEST_P(UniformGeneratorTest, LastValue) {
	UniformGenerator generator(std::get<0>(GetParam()), std::get<1>(GetParam()));
	for(int i = 0; i < 10; i++) {
		auto valueTmp = generator.Next();
		EXPECT_TRUE(IsBetweenInclusive(valueTmp, std::get<0>(GetParam()), std::get<1>(GetParam())));
	}
	auto value = generator.Last();
	EXPECT_TRUE(IsBetweenInclusive(value, std::get<0>(GetParam()), std::get<1>(GetParam())));
}

std::pair<uint64_t,uint64_t> FormulaTable[] = {
	std::make_pair(static_cast<uint64_t>(0), static_cast<uint64_t>(2)),
	std::make_pair(static_cast<uint64_t>(10), static_cast<uint64_t>(50)),
};

INSTANTIATE_TEST_CASE_P(InstantiationName, UniformGeneratorTest,::testing::ValuesIn(FormulaTable));

} // namespace uniformgeneratortest
} // namespace test

#endif // _DBBENCHMARK_UNIFORMGENERATORTEST_H_