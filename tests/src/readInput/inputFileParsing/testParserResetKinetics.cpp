#include "exceptions.hpp"                     // for InputFileException
#include "inputFileParser.hpp"                // for readInput
#include "inputFileParserResetKinetics.hpp"   // for InputFileParserResetKinetics
#include "resetKineticsSettings.hpp"          // for ResetKineticsSettings
#include "testInputFileReader.hpp"            // for TestInputFileReader
#include "throwWithMessage.hpp"               // for EXPECT_THROW_MSG

#include "gtest/gtest.h"   // for Message, TestPartResult
#include <gtest/gtest.h>   // for TestInfo (ptr only), EXPECT_EQ
#include <string>          // for string, allocator, basic_string
#include <vector>          // for vector

using namespace readInput;

/**
 * @brief tests parsing the "nscale" command
 *
 * @details if the nscale is negative it throws inputFileException
 */
TEST_F(TestInputFileReader, testParseNScale)
{
    InputFileParserResetKinetics parser(*_engine);
    std::vector<std::string>     lineElements = {"nscale", "=", "3"};
    parser.parseNScale(lineElements, 0);
    EXPECT_EQ(settings::ResetKineticsSettings::getNScale(), 3);

    lineElements = {"nscale", "=", "-1"};
    EXPECT_THROW_MSG(parser.parseNScale(lineElements, 0), customException::InputFileException, "Nscale must be positive");
}

/**
 * @brief tests parsing the "fscale" command
 *
 * @details if the fscale is negative it throws inputFileException
 */
TEST_F(TestInputFileReader, testParseFScale)
{
    InputFileParserResetKinetics parser(*_engine);
    std::vector<std::string>     lineElements = {"fscale", "=", "3"};
    parser.parseFScale(lineElements, 0);
    EXPECT_EQ(settings::ResetKineticsSettings::getFScale(), 3);

    lineElements = {"fscale", "=", "-1"};
    EXPECT_THROW_MSG(parser.parseFScale(lineElements, 0), customException::InputFileException, "Fscale must be positive");
}

/**
 * @brief tests parsing the "nreset" command
 *
 * @details if the nreset is negative it throws inputFileException
 */
TEST_F(TestInputFileReader, testParseNReset)
{
    InputFileParserResetKinetics parser(*_engine);
    std::vector<std::string>     lineElements = {"nreset", "=", "3"};
    parser.parseNReset(lineElements, 0);
    EXPECT_EQ(settings::ResetKineticsSettings::getNReset(), 3);

    lineElements = {"nreset", "=", "-1"};
    EXPECT_THROW_MSG(parser.parseNReset(lineElements, 0), customException::InputFileException, "Nreset must be positive");
}

/**
 * @brief tests parsing the "freset" command
 *
 * @details if the freset is negative it throws inputFileException
 */
TEST_F(TestInputFileReader, testParseFReset)
{
    InputFileParserResetKinetics parser(*_engine);
    std::vector<std::string>     lineElements = {"freset", "=", "3"};
    parser.parseFReset(lineElements, 0);
    EXPECT_EQ(settings::ResetKineticsSettings::getFReset(), 3);

    lineElements = {"freset", "=", "-1"};
    EXPECT_THROW_MSG(parser.parseFReset(lineElements, 0), customException::InputFileException, "Freset must be positive");
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return ::RUN_ALL_TESTS();
}