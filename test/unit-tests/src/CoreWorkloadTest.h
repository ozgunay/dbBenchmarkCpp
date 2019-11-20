// CoreWorkloadTest.h

#ifndef _DBBENCHMARK_COREWORKLOADTEST_H_
#define _DBBENCHMARK_COREWORKLOADTEST_H_

#include <gtest/gtest.h>

#include "Core/Workloads/CoreWorkload.h"

using namespace dbbenchmark::workloads;

namespace testing {
namespace coreworkloadtest {

class CoreWorkloadTest  : public ::testing::Test {
public:
CoreWorkloadTest() {
	coreworkload = std::make_unique<CoreWorkload>();
	try{
		this->coreworkload->Init();
	}
	catch( const std::exception& e ) {
		// and this tests that it has the correct message
		LOG(WARNING) <<"[Error in Workload Init]" << std::endl;
		throw;
	}
}
~CoreWorkloadTest() override { }
	std::unique_ptr<CoreWorkload> coreworkload;
	const int numberOfFiledsGenerated = 1;
};

TEST_F(CoreWorkloadTest, BuildValues) {
	std::vector<stringPair> values;
	this->coreworkload->BuildValues(values);
	EXPECT_EQ(numberOfFiledsGenerated, values.size());
}

TEST_F(CoreWorkloadTest, BuildUpdate) {
	std::vector<stringPair> update;
	this->coreworkload->BuildUpdate(update);
	EXPECT_EQ(1, update.size());
}

TEST_F(CoreWorkloadTest, NextTable) {
	ASSERT_EQ("usertable", this->coreworkload->NextTable());
}

TEST_F(CoreWorkloadTest, NextSequenceKey) {
	ASSERT_EQ("user0", this->coreworkload->NextSequenceKey());
}

TEST_F(CoreWorkloadTest, NextTransactionKey) {
	ASSERT_EQ("user250", this->coreworkload->NextTransactionKey());
}

TEST_F(CoreWorkloadTest, NextOperation) {
	Operation Opr = Operation::READ;
	ASSERT_EQ(Opr, this->coreworkload->NextOperation());
}

TEST_F(CoreWorkloadTest, NextFieldName) {
	ASSERT_EQ("field0", this->coreworkload->NextFieldName());
}

TEST_F(CoreWorkloadTest, NextScanLength) {
	ASSERT_EQ(1, this->coreworkload->NextScanLength());
}

} // namespace coreworkloadtest
} // namespace testing

#endif // _DBBENCHMARK_COREWORKLOADTEST_H_