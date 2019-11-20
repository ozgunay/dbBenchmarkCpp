// DiscreteGeneratorTest.h

#ifndef _DBBENCHMARK_DISCRETEGENERATORTEST_H_
#define _DBBENCHMARK_DISCRETEGENERATORTEST_H_

#include <gtest/gtest.h>
#include <memory>

#include "Core/Generators/DiscreteGenerator.h"

using namespace dbbenchmark::generators;

namespace test {
namespace discretegeneratortest {

::testing::AssertionResult IsBetweenInclusive(int val, int a, int b) {
	if((val >= a) && (val <= b))
		return ::testing::AssertionSuccess();
	else
		return ::testing::AssertionFailure()
			<< val << " is outside the range " << a << " to " << b;
}

template <typename Value>
class DiscreteGeneratorTest : public ::testing::Test {
public:
	DiscreteGeneratorTest() {
		generator = std::make_unique<DiscreteGenerator<Value>>();
	}
	~DiscreteGeneratorTest() override { }
	std::unique_ptr<DiscreteGenerator<Value>> generator;
};

typedef ::testing::Types<float, double, u_int64_t /*, and few more*/> TestTypes;
TYPED_TEST_CASE(DiscreteGeneratorTest, TestTypes);

TYPED_TEST(DiscreteGeneratorTest, NextValue) {
	this->generator->AddValue(1, 0.2);
	this->generator->AddValue(2, 0.4);
	this->generator->AddValue(3, 0.4);	
	auto value = this->generator->Next();
	EXPECT_TRUE(IsBetweenInclusive(value, 1, 3));
	value = this->generator->Next();
	EXPECT_TRUE(IsBetweenInclusive(value, 1, 3));
	value = this->generator->Next();
	EXPECT_TRUE(IsBetweenInclusive(value, 1, 3));
}

TYPED_TEST(DiscreteGeneratorTest, NextConstValue) {
	this->generator->AddValue(11, 1.0);
	auto value = this->generator->Next();
	EXPECT_EQ(11, value);
}

TYPED_TEST(DiscreteGeneratorTest, LastValue) {
	auto value = this->generator->Last();
	EXPECT_EQ(11, value);
}

} // namespace discretegeneratortest
} // namespace test

#endif // _DBBENCHMARK_DISCRETEGENERATORTEST_H_