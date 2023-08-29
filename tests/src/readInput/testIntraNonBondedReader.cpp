#include "testIntraNonBondedReader.hpp"

#include "exceptions.hpp"                // for IntraNonBondedException
#include "fileSettings.hpp"              // for FileSettings
#include "intraNonBondedContainer.hpp"   // for IntraNonBondedContainer
#include "throwWithMessage.hpp"          // for EXPECT_THROW_MSG

#include "gtest/gtest.h"   // for Message, TestPartResult, TEST_F
#include <vector>          // for vector

TEST_F(TestIntraNonBondedReader, findMoleculeType)
{
    EXPECT_EQ(_intraNonBondedReader->findMoleculeType("molecule1"), 0);

    EXPECT_EQ(_intraNonBondedReader->findMoleculeType("0"), 0);

    _intraNonBondedReader->setFileName("intraNonBonded.dat");
    EXPECT_THROW_MSG([[maybe_unused]] const auto dummy = _intraNonBondedReader->findMoleculeType("molecule2"),
                     customException::IntraNonBondedException,
                     "ERROR: could not find molecule type \"molecule2\" in line 1 in file \"intraNonBonded.dat\"");

    EXPECT_THROW_MSG([[maybe_unused]] const auto dummy = _intraNonBondedReader->findMoleculeType("1"),
                     customException::IntraNonBondedException,
                     "ERROR: could not find molecule type \"1\" in line 1 in file \"intraNonBonded.dat\"");
}

TEST_F(TestIntraNonBondedReader, noFileSetByUser)
{
    settings::FileSettings::unsetIsIntraNonBondedFileNameSet();
    EXPECT_THROW_MSG(_intraNonBondedReader->read(),
                     customException::IntraNonBondedException,
                     "Intra non bonded file needed for requested simulation setup");
}

TEST_F(TestIntraNonBondedReader, readingNotNeeded)
{
    _intraNonBondedReader->setFileName("");
    _engine->getIntraNonBonded().deactivate();
    EXPECT_NO_THROW(_intraNonBondedReader->read());
}

TEST_F(TestIntraNonBondedReader, referenceAtomOutOfRange)
{
    _intraNonBondedReader->setFileName("data/intraNonBondedReader/referenceAtomOutOfRange.dat");
    _intraNonBondedReader->reInitializeFp();
    EXPECT_THROW_MSG(_intraNonBondedReader->read(),
                     customException::IntraNonBondedException,
                     "ERROR: reference atom index \"4\" in line 2 in file "
                     "\"data/intraNonBondedReader/referenceAtomOutOfRange.dat\" is out of range");
}

TEST_F(TestIntraNonBondedReader, atomIndexOutOfRange)
{
    _intraNonBondedReader->setFileName("data/intraNonBondedReader/atomIndexOutOfRange.dat");
    _intraNonBondedReader->reInitializeFp();
    EXPECT_THROW_MSG(
        _intraNonBondedReader->read(),
        customException::IntraNonBondedException,
        "ERROR: atom index \"4\" in line 3 in file \"data/intraNonBondedReader/atomIndexOutOfRange.dat\" is out of range");
}

TEST_F(TestIntraNonBondedReader, missingEndKeyword)
{
    _intraNonBondedReader->setFileName("data/intraNonBondedReader/missingEndKeyword.dat");
    _intraNonBondedReader->reInitializeFp();
    EXPECT_THROW_MSG(_intraNonBondedReader->read(),
                     customException::IntraNonBondedException,
                     "ERROR: could not find \"END\" for moltype \"0\" in file "
                     "\"data/intraNonBondedReader/missingEndKeyword.dat\"");
}

TEST_F(TestIntraNonBondedReader, moltypeDefinedMultipleTimes)
{
    _intraNonBondedReader->setFileName("data/intraNonBondedReader/molTypeDefinedMultipleTimes.dat");
    _intraNonBondedReader->reInitializeFp();
    EXPECT_THROW_MSG(
        _intraNonBondedReader->read(),
        customException::IntraNonBondedException,
        "ERROR: moltype \"0\" is defined multiple times in file \"data/intraNonBondedReader/molTypeDefinedMultipleTimes.dat\"");
}

TEST_F(TestIntraNonBondedReader, readIntraNonBondedFile)
{
    settings::FileSettings::setIntraNonBondedFileName("data/intraNonBondedReader/intraNonBonded.dat");
    readInput::intraNonBonded::readIntraNonBondedFile(*_engine);

    const auto container = _engine->getIntraNonBonded().getIntraNonBondedContainers()[0];
    EXPECT_EQ(container.getMolType(), 0);
    const auto &atomIndices = container.getAtomIndices();

    EXPECT_EQ(atomIndices[0], std::vector<int>({1, -2}));
    EXPECT_EQ(atomIndices[1], std::vector<int>({}));
    EXPECT_EQ(atomIndices[2], std::vector<int>({1}));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return ::RUN_ALL_TESTS();
}