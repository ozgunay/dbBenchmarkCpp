// CounterGeneratorTest.h

#ifndef _DBBENCHMARK_COUNTERGENERATORTEST_H_
#define _DBBENCHMARK_COUNTERGENERATORTEST_H_

#include <gtest/gtest.h>
#include "Core/Generators/CounterGenerator.h"

using namespace dbbenchmark::generators;

namespace test {
namespace countergeneratortest {

class CounterGeneratorTest  : public ::testing::TestWithParam<uint64_t> {
	virtual void SetUp() { return; }
	virtual void TearDown() { return; }
};

TEST_P(CounterGeneratorTest, NextValue) {
	CounterGenerator generator(GetParam());
	EXPECT_EQ(GetParam(), generator.Next());
	ASSERT_EQ(GetParam()+1, generator.Next());
}

TEST_P(CounterGeneratorTest, LastValue) {
	CounterGenerator generator(GetParam());
	for(int i = 0; i < 2; i++)
		generator.Next();
	ASSERT_EQ(GetParam() + 1, generator.Last());
}

TEST_P(CounterGeneratorTest, Set) {
	CounterGenerator generator(GetParam());
	generator.Set(GetParam());
	EXPECT_EQ(GetParam(), generator.Next());
}

INSTANTIATE_TEST_CASE_P(InstantiationName, CounterGeneratorTest, ::testing::Values(static_cast<uint64_t>(0), static_cast<uint64_t>(2),
						static_cast<uint64_t>(12345)));

} // namespace countergeneratortest
} // namespace test

#endif // _DBBENCHMARK_COUNTERGENERATORTEST_H_