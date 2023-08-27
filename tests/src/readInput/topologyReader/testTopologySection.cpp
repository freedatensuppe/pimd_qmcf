#include "testTopologySection.hpp"

#include "bondConstraint.hpp"    // for BondConstraint
#include "constraints.hpp"       // for Constraints
#include "exceptions.hpp"        // for TopologyException
#include "topologySection.hpp"   // for ShakeSection, topology

#include "gtest/gtest.h"   // for Message, TestPartResult
#include <fstream>         // for operator<<, ofstream, basic_ostream
#include <vector>          // for vector

using namespace ::testing;
using namespace readInput::topology;

/**
 * @brief tests full process function
 *
 */
TEST_F(TestTopologySection, processShakeSection)
{
    ShakeSection shakeSection;

    std::ofstream _outputStream(_topologyFilename.c_str());

    _outputStream << "shake\n";
    _outputStream << "1 2 1.0 0\n";
    _outputStream << "         \n";
    _outputStream << "2 3 1.2 0\n";
    _outputStream << "end" << std::endl;

    _outputStream.close();

    auto          lineElements = std::vector{std::string("")};
    std::ifstream fp(_topologyFilename.c_str());
    getline(fp, lineElements[0]);

    shakeSection.setFp(&fp);
    shakeSection.setLineNumber(1);

    EXPECT_NO_THROW(shakeSection.process(lineElements, *_engine));

    EXPECT_EQ(_engine->getConstraints().getBondConstraints().size(), 2);

    EXPECT_EQ(_engine->getConstraints().getBondConstraints()[0].getMolecule1(), &(_engine->getSimulationBox().getMolecules()[0]));
    EXPECT_EQ(_engine->getConstraints().getBondConstraints()[0].getMolecule2(), &(_engine->getSimulationBox().getMolecules()[1]));
    EXPECT_EQ(_engine->getConstraints().getBondConstraints()[0].getAtomIndex1(), 0);
    EXPECT_EQ(_engine->getConstraints().getBondConstraints()[0].getAtomIndex2(), 0);
    EXPECT_EQ(_engine->getConstraints().getBondConstraints()[0].getTargetBondLength(), 1.0);

    EXPECT_EQ(_engine->getConstraints().getBondConstraints()[1].getMolecule1(), &(_engine->getSimulationBox().getMolecules()[1]));
    EXPECT_EQ(_engine->getConstraints().getBondConstraints()[1].getMolecule2(), &(_engine->getSimulationBox().getMolecules()[1]));
    EXPECT_EQ(_engine->getConstraints().getBondConstraints()[1].getAtomIndex1(), 0);
    EXPECT_EQ(_engine->getConstraints().getBondConstraints()[1].getAtomIndex2(), 1);
    EXPECT_EQ(_engine->getConstraints().getBondConstraints()[1].getTargetBondLength(), 1.2);

    EXPECT_EQ(shakeSection.getLineNumber(), 5);
}

/**
 * @brief tests if incorrect number of elements is correctly handled
 *
 */
TEST_F(TestTopologySection, processShakeSection_incorrectNumberOfElements)
{
    ShakeSection shakeSection;

    std::ofstream _outputStream(_topologyFilename.c_str());

    _outputStream << "shake\n";
    _outputStream << "1 2 1.0\n";
    _outputStream << "end" << std::endl;

    _outputStream.close();

    auto          lineElements = std::vector{std::string("")};
    std::ifstream fp(_topologyFilename.c_str());
    getline(fp, lineElements[0]);
    shakeSection.setFp(&fp);

    EXPECT_THROW(shakeSection.process(lineElements, *_engine), customException::TopologyException);
}

/**
 * @brief tests if same atom given twice is correctly handled
 *
 */
TEST_F(TestTopologySection, processShakeSection_sameAtomTwice)
{
    ShakeSection shakeSection;

    std::ofstream _outputStream(_topologyFilename.c_str());

    _outputStream << "shake\n";
    _outputStream << "1 1 1.0 0\n";
    _outputStream << "end" << std::endl;

    _outputStream.close();

    auto          lineElements = std::vector{std::string("")};
    std::ifstream fp(_topologyFilename.c_str());
    getline(fp, lineElements[0]);
    shakeSection.setFp(&fp);

    EXPECT_THROW(shakeSection.process(lineElements, *_engine), customException::TopologyException);
}

/**
 * @brief tests if missing end statement is correctly handled
 *
 */
TEST_F(TestTopologySection, processShakeSection_missingEnd)
{
    ShakeSection shakeSection;

    std::ofstream _outputStream(_topologyFilename.c_str());

    _outputStream << "shake\n";
    _outputStream << "1 2 1.0 0\n";
    _outputStream << "         \n";
    _outputStream << "2 3 1.2 0\n";
    _outputStream << "" << std::endl;

    _outputStream.close();

    auto          lineElements = std::vector{std::string("")};
    std::ifstream fp(_topologyFilename.c_str());
    getline(fp, lineElements[0]);
    shakeSection.setFp(&fp);

    EXPECT_THROW(shakeSection.process(lineElements, *_engine), customException::TopologyException);
}

int main(int argc, char **argv)
{
    InitGoogleTest(&argc, argv);
    return ::RUN_ALL_TESTS();
}