#include <gtest/gtest.h>

#include "Generators/ConstGeneratorTest.h"
#include "Generators/CounterGeneratorTest.h"
#include "Generators/DiscreteGeneratorTest.h"
#include "Generators/ScrambledZipfianGeneratorTest.h"
#include "Generators/ScrambledZipfianGeneratorTest.h"
#include "Generators/UniformGeneratorTest.h"
#include "CoreWorkloadTest.h"

using namespace testing;

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}