#include "exceptions.hpp"           // for RstFileException, customException
#include "rstFileSection.hpp"       // for RstFileSection, readInput
#include "testRstFileSection.hpp"   // for TestNoseHooverSection

#include "gtest/gtest.h"   // for AssertionResult, Message, TestPart...
#include <gtest/gtest.h>   // for TestInfo (ptr only), TEST_F, InitG...
#include <iosfwd>          // for std
#include <string>          // for string, allocator
#include <vector>          // for vector

using namespace std;
using namespace testing;
using namespace readInput;
using namespace customException;

TEST_F(TestNoseHooverSection, testKeyword) { EXPECT_EQ(_section->keyword(), "chi"); }

TEST_F(TestNoseHooverSection, testIsHeader) { EXPECT_TRUE(_section->isHeader()); }

TEST_F(TestNoseHooverSection, testNumberOfArguments) { GTEST_SKIP(); }

TEST_F(TestNoseHooverSection, testProcess)
{
    auto line = vector<string>(0);
    ASSERT_THROW(_section->process(line, _engine), RstFileException);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
