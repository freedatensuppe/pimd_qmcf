#include "cell.hpp"                 // for simulationBox
#include "engine.hpp"               // for Engine
#include "exceptions.hpp"           // for RstFileException, customException
#include "molecule.hpp"             // for Molecule
#include "rstFileSection.hpp"       // for RstFileSection, AtomSection, readI...
#include "simulationBox.hpp"        // for SimulationBox
#include "testRstFileSection.hpp"   // for TestAtomSection
#include "throwWithMessage.hpp"     // for ASSERT_THROW_MSG
#include "vector3d.hpp"             // for linearAlgebra

#include "gmock/gmock.h"   // for ElementsAre, MakePredicateFormatte...
#include "gtest/gtest.h"   // for Message, TestPartResult, Assertion...
#include <algorithm>       // for max
#include <fstream>         // for ifstream, std
#include <gtest/gtest.h>   // for TestInfo (ptr only), EXPECT_EQ
#include <stddef.h>        // for size_t
#include <string>          // for string, stod, allocator, basic_string
#include <vector>          // for vector

using namespace std;
using namespace testing;
using namespace readInput;
using namespace simulationBox;
using namespace customException;
using namespace linearAlgebra;

/**
 * @brief tests the keyword function
 *
 */
TEST_F(TestAtomSection, keyword) { EXPECT_EQ(_section->keyword(), ""); }

/**
 * @brief tests the isHeader function
 *
 */
TEST_F(TestAtomSection, isHeader) { EXPECT_FALSE(_section->isHeader()); }

/**
 * @brief tests the numberOfArguments function
 *
 */
TEST_F(TestAtomSection, numberOfArguments)
{
    _section->_lineNumber = 7;
    for (size_t i = 0; i < 25; ++i)
        if (i != 21)
        {
            auto line = vector<string>(i);
            ASSERT_THROW_MSG(
                _section->process(line, _engine), RstFileException, "Error in line 7: Atom section must have 21 elements");
        }
}

/**
 * @brief tests if moltype is not found in process function
 *
 */
TEST_F(TestAtomSection, moltypeNotFound)
{
    auto line = vector<string>(21);
    line[2]   = "1";
    ASSERT_THROW_MSG(_section->process(line, _engine), RstFileException, "Molecule type 1 not found");
}

TEST_F(TestAtomSection, notEnoughElementsInLine)
{
    _section->_lineNumber = 7;
    auto line             = vector<string>(21);
    line[2]               = "1";
    for (size_t i = 3; i < 21; ++i)
        line[i] = "1.0";

    string filename = "data/atomSection/testNotEnoughAtomsInMolecule.rst";

    auto molecule = Molecule(1);
    molecule.setNumberOfAtoms(3);
    _engine.getSimulationBox().getMoleculeTypes().push_back(molecule);

    ifstream fp(filename);
    _section->_fp = &fp;

    ASSERT_THROW(_section->process(line, _engine), RstFileException);

    line[2] = "1";

    string   filename2 = "data/atomSection/testNotEnoughAtomsInMolecule2.rst";
    ifstream fp2(filename2);
    _section->_fp = &fp2;

    ASSERT_THROW(_section->process(line, _engine), RstFileException);
}

TEST_F(TestAtomSection, numberOfArgumentsWithinMolecule)
{
    auto line = vector<string>(21);
    line[2]   = "1";
    for (int i = 3; i < 21; ++i)
        line[i] = "1.0";

    string filename = "data/atomSection/testNumberOfArgumentsWithinMolecule.rst";

    auto molecule = Molecule(1);
    molecule.setNumberOfAtoms(3);
    _engine.getSimulationBox().getMoleculeTypes().push_back(molecule);

    ifstream fp(filename);
    _section->_fp = &fp;

    ASSERT_THROW(_section->process(line, _engine), RstFileException);
}

TEST_F(TestAtomSection, testProcess)
{
    auto line = vector<string>(21);
    line[2]   = "1";
    for (int i = 3; i < 21; ++i)
        line[i] = "1.0";

    string filename = "data/atomSection/testProcess.rst";

    auto molecule = Molecule(1);
    molecule.setNumberOfAtoms(3);
    _engine.getSimulationBox().getMoleculeTypes().push_back(molecule);

    auto molecule2 = Molecule(2);
    molecule2.setNumberOfAtoms(4);
    _engine.getSimulationBox().getMoleculeTypes().push_back(molecule2);

    ifstream fp(filename);
    _section->_fp = &fp;

    _section->process(line, _engine);

    line    = vector<string>(21);
    line[2] = "2";
    for (int i = 3; i < 21; ++i)
        line[i] = "1.0";

    _section->process(line, _engine);

    line    = vector<string>(21);
    line[2] = "1";
    for (int i = 3; i < 21; ++i)
        line[i] = "1.0";

    _section->process(line, _engine);

    EXPECT_EQ(_engine.getSimulationBox().getMolecules().size(), 3);

    EXPECT_EQ(_engine.getSimulationBox().getMolecules()[0].getMoltype(), 1);
    EXPECT_EQ(_engine.getSimulationBox().getMolecules()[0].getNumberOfAtoms(), 3);

    EXPECT_EQ(_engine.getSimulationBox().getMolecules()[1].getMoltype(), 2);
    EXPECT_EQ(_engine.getSimulationBox().getMolecules()[1].getNumberOfAtoms(), 4);

    EXPECT_EQ(_engine.getSimulationBox().getMolecules()[2].getMoltype(), 1);
    EXPECT_EQ(_engine.getSimulationBox().getMolecules()[2].getNumberOfAtoms(), 3);
}

TEST_F(TestAtomSection, testProcessAtomLine)
{
    Molecule molecule(1);

    auto line = vector<string>(21);
    line[0]   = "Ar";
    for (int i = 3; i < 21; ++i)
        line[i] = to_string(i + i / 10.0);

    static_cast<AtomSection *>(_section)->processAtomLine(line, molecule);

    ASSERT_THAT(molecule.getAtomPosition(0), ElementsAre(stod(line[3]), stod(line[4]), stod(line[5])));
    ASSERT_THAT(molecule.getAtomVelocity(0), ElementsAre(stod(line[6]), stod(line[7]), stod(line[8])));
    ASSERT_THAT(molecule.getAtomForce(0), ElementsAre(stod(line[9]), stod(line[10]), stod(line[11])));

    ASSERT_EQ(molecule.getAtomTypeName(0), line[0]);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return ::RUN_ALL_TESTS();
}