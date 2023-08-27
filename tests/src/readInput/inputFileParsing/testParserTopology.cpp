#include "engine.hpp"                    // for Engine
#include "exceptions.hpp"                // for InputFileException
#include "inputFileParser.hpp"           // for readInput
#include "inputFileParserTopology.hpp"   // for InputFileParserTopology
#include "settings.hpp"                  // for Settings
#include "testInputFileReader.hpp"       // for TestInputFileReader
#include "throwWithMessage.hpp"          // for EXPECT_THROW_MSG

#include "gtest/gtest.h"   // for Message, TestPartResult, testing
#include <gtest/gtest.h>   // for InitGoogleTest, RUN_ALL_TESTS
#include <iosfwd>          // for std
#include <string>          // for string, allocator, basic_string
#include <vector>          // for vector

using namespace std;
using namespace readInput;
using namespace ::testing;

/**
 * @brief tests parsing the "topology_file" command
 *
 * @details if the filename is empty  or does not exist it throws inputFileException
 *
 */
TEST_F(TestInputFileReader, testParseTopologyFilename)
{
    InputFileParserTopology parser(_engine);
    vector<string>          lineElements = {"topology_file", "=", ""};
    EXPECT_THROW_MSG(
        parser.parseTopologyFilename(lineElements, 0), customException::InputFileException, "Topology filename cannot be empty");

    lineElements = {"topology_file", "=", "topology.txt"};
    EXPECT_THROW_MSG(parser.parseTopologyFilename(lineElements, 0),
                     customException::InputFileException,
                     "Cannot open topology file - filename = topology.txt");

    lineElements = {"topology_file", "=", "data/topologyReader/topology.top"};
    parser.parseTopologyFilename(lineElements, 0);
    EXPECT_EQ(_engine.getSettings().getTopologyFilename(), "data/topologyReader/topology.top");
}

int main(int argc, char **argv)
{
    InitGoogleTest(&argc, argv);
    return ::RUN_ALL_TESTS();
}