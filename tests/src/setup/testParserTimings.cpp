#include "exceptions.hpp"
#include "testInputFileReader.hpp"

using namespace std;
using namespace setup;
using namespace ::testing;

TEST_F(TestInputFileReader, testParseTimestep)
{
    vector<string> lineElements = {"timestep", "=", "1"};
    _inputFileReader->parseTimestep(lineElements);
    EXPECT_EQ(_engine.getTimings().getTimestep(), 1.0);
}

TEST_F(TestInputFileReader, testParseNumberOfSteps)
{
    vector<string> lineElements = {"nsteps", "=", "1000"};
    _inputFileReader->parseNumberOfSteps(lineElements);
    EXPECT_EQ(_engine.getTimings().getNumberOfSteps(), 1000);
}

int main(int argc, char **argv)
{
    InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}