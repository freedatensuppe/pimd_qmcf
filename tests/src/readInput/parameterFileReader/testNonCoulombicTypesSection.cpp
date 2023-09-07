#include "buckinghamPair.hpp"             // for BuckinghamPair
#include "engine.hpp"                     // for Engine
#include "exceptions.hpp"                 // for ParameterFileException
#include "forceFieldNonCoulomb.hpp"       // for ForceFieldNonCoulomb
#include "lennardJonesPair.hpp"           // for LennardJonesPair
#include "morsePair.hpp"                  // for MorsePair
#include "nonCoulombPair.hpp"             // for NonCoulombPair
#include "nonCoulombPotential.hpp"        // for NonCoulombType, NonCoulombPo...
#include "nonCoulombicsSection.hpp"       // for NonCoulombicsSection
#include "parameterFileSection.hpp"       // for NonCoulombicsSection, parame...
#include "potential.hpp"                  // for Potential
#include "potentialSettings.hpp"          // for PotentialSettings
#include "simulationBox.hpp"              // for SimulationBox
#include "testParameterFileSection.hpp"   // for TestParameterFileSection
#include "throwWithMessage.hpp"           // for ASSERT_THROW_MSG

#include "gtest/gtest.h"   // for Message, TestPartResult, tes...
#include <gtest/gtest.h>   // for EXPECT_EQ, TestInfo (ptr only)
#include <memory>          // for allocator, shared_ptr
#include <string>          // for string, basic_string
#include <vector>          // for vector

using namespace readInput::parameterFile;

TEST_F(TestParameterFileSection, processSectionLennardJones)
{
    auto &potential = dynamic_cast<potential::ForceFieldNonCoulomb &>(_engine->getPotential().getNonCoulombPotential());

    std::vector<std::string> lineElements = {"0", "1", "1.22", "234.3", "324.3"};
    NonCoulombicsSection     nonCoulombicsSection;
    nonCoulombicsSection.processLJ(lineElements, *_engine);

    EXPECT_EQ(potential.getNonCoulombPairsVector().size(), 1);
    auto *pair = dynamic_cast<const potential::LennardJonesPair *>(potential.getNonCoulombPairsVector()[0].get());
    EXPECT_EQ(pair->getVanDerWaalsType1(), 0);
    EXPECT_EQ(pair->getVanDerWaalsType2(), 1);
    EXPECT_EQ(pair->getC6(), 1.22);
    EXPECT_EQ(pair->getC12(), 234.3);
    EXPECT_EQ(pair->getRadialCutOff(), 324.3);

    lineElements = {"0", "1", "1.22", "234.3"};
    nonCoulombicsSection.processLJ(lineElements, *_engine);
    EXPECT_EQ(potential.getNonCoulombPairsVector().size(), 2);
    auto *pair2 = dynamic_cast<const potential::LennardJonesPair *>(potential.getNonCoulombPairsVector()[1].get());
    EXPECT_EQ(pair2->getVanDerWaalsType1(), 0);
    EXPECT_EQ(pair2->getVanDerWaalsType2(), 1);
    EXPECT_EQ(pair2->getC6(), 1.22);
    EXPECT_EQ(pair2->getC12(), 234.3);
    EXPECT_EQ(pair2->getRadialCutOff(), 12.5);

    lineElements = {"1", "2", "1.0", "0", "2", "3.3"};
    EXPECT_THROW(nonCoulombicsSection.processLJ(lineElements, *_engine), customException::ParameterFileException);
}

TEST_F(TestParameterFileSection, processSectionBuckingham)
{
    auto &potential = dynamic_cast<potential::ForceFieldNonCoulomb &>(_engine->getPotential().getNonCoulombPotential());

    std::vector<std::string> lineElements = {"0", "1", "1.22", "234.3", "324.3", "435"};
    NonCoulombicsSection     nonCoulombicsSection;
    nonCoulombicsSection.processBuckingham(lineElements, *_engine);
    EXPECT_EQ(potential.getNonCoulombPairsVector().size(), 1);
    auto *pair = dynamic_cast<const potential::BuckinghamPair *>(potential.getNonCoulombPairsVector()[0].get());
    EXPECT_EQ(pair->getVanDerWaalsType1(), 0);
    EXPECT_EQ(pair->getVanDerWaalsType2(), 1);
    EXPECT_EQ(pair->getA(), 1.22);
    EXPECT_EQ(pair->getDRho(), 234.3);
    EXPECT_EQ(pair->getC6(), 324.3);
    EXPECT_EQ(pair->getRadialCutOff(), 435.0);

    lineElements = {"0", "1", "1.22", "234.3", "324.3"};
    nonCoulombicsSection.processBuckingham(lineElements, *_engine);
    EXPECT_EQ(potential.getNonCoulombPairsVector().size(), 2);
    auto *pair2 = dynamic_cast<const potential::BuckinghamPair *>(potential.getNonCoulombPairsVector()[1].get());
    EXPECT_EQ(pair2->getVanDerWaalsType1(), 0);
    EXPECT_EQ(pair2->getVanDerWaalsType2(), 1);
    EXPECT_EQ(pair2->getA(), 1.22);
    EXPECT_EQ(pair2->getDRho(), 234.3);
    EXPECT_EQ(pair2->getC6(), 324.3);
    EXPECT_EQ(pair2->getRadialCutOff(), 12.5);

    lineElements = {"1", "2", "1.0", "0", "2", "3.3", "345"};
    EXPECT_THROW(nonCoulombicsSection.processBuckingham(lineElements, *_engine), customException::ParameterFileException);
}

TEST_F(TestParameterFileSection, processSectionMorse)
{
    auto &potential = dynamic_cast<potential::ForceFieldNonCoulomb &>(_engine->getPotential().getNonCoulombPotential());

    std::vector<std::string> lineElements = {"0", "1", "1.22", "234.3", "324.3", "435"};
    NonCoulombicsSection     nonCoulombicsSection;
    nonCoulombicsSection.processMorse(lineElements, *_engine);
    EXPECT_EQ(potential.getNonCoulombPairsVector().size(), 1);
    auto *pair = dynamic_cast<const potential::MorsePair *>(potential.getNonCoulombPairsVector()[0].get());
    EXPECT_EQ(pair->getVanDerWaalsType1(), 0);
    EXPECT_EQ(pair->getVanDerWaalsType2(), 1);
    EXPECT_EQ(pair->getDissociationEnergy(), 1.22);
    EXPECT_EQ(pair->getWellWidth(), 234.3);
    EXPECT_EQ(pair->getEquilibriumDistance(), 324.3);
    EXPECT_EQ(pair->getRadialCutOff(), 435.0);

    lineElements = {"0", "1", "1.22", "234.3", "324.3"};
    nonCoulombicsSection.processMorse(lineElements, *_engine);
    EXPECT_EQ(potential.getNonCoulombPairsVector().size(), 2);
    auto *pair2 = dynamic_cast<const potential::MorsePair *>(potential.getNonCoulombPairsVector()[1].get());
    EXPECT_EQ(pair2->getVanDerWaalsType1(), 0);
    EXPECT_EQ(pair2->getVanDerWaalsType2(), 1);
    EXPECT_EQ(pair2->getDissociationEnergy(), 1.22);
    EXPECT_EQ(pair2->getWellWidth(), 234.3);
    EXPECT_EQ(pair2->getEquilibriumDistance(), 324.3);
    EXPECT_EQ(pair2->getRadialCutOff(), 12.5);

    lineElements = {"1", "2", "1.0", "0", "2", "3.3", "345"};
    EXPECT_THROW(nonCoulombicsSection.processMorse(lineElements, *_engine), customException::ParameterFileException);
}

TEST_F(TestParameterFileSection, processHeader)
{
    std::vector<std::string> lineElements = {"noncoulombics"};
    NonCoulombicsSection     nonCoulombicsSection;
    nonCoulombicsSection.processHeader(lineElements, *_engine);
    EXPECT_EQ(settings::PotentialSettings::getNonCoulombType(), settings::NonCoulombType::LJ);

    lineElements = {"noncoulombics", "lj"};
    nonCoulombicsSection.processHeader(lineElements, *_engine);
    EXPECT_EQ(settings::PotentialSettings::getNonCoulombType(), settings::NonCoulombType::LJ);

    lineElements = {"noncoulombics", "buckingham"};
    nonCoulombicsSection.processHeader(lineElements, *_engine);
    EXPECT_EQ(settings::PotentialSettings::getNonCoulombType(), settings::NonCoulombType::BUCKINGHAM);

    lineElements = {"noncoulombics", "morse"};
    nonCoulombicsSection.processHeader(lineElements, *_engine);
    EXPECT_EQ(settings::PotentialSettings::getNonCoulombType(), settings::NonCoulombType::MORSE);

    lineElements = {"noncoulombics", "lj", "dummy"};
    EXPECT_NO_THROW(nonCoulombicsSection.processHeader(lineElements, *_engine));

    lineElements = {"noncoulombics", "noValidType"};
    EXPECT_THROW(nonCoulombicsSection.processHeader(lineElements, *_engine), customException::ParameterFileException);
}

TEST_F(TestParameterFileSection, processSectionNonCoulombics)
{
    auto &potential = dynamic_cast<potential::ForceFieldNonCoulomb &>(_engine->getPotential().getNonCoulombPotential());

    std::vector<std::string> lineElements = {"0", "1", "1.22", "234.3", "324.3"};
    NonCoulombicsSection     nonCoulombicsSection;
    settings::PotentialSettings::setNonCoulombType(settings::NonCoulombType::LJ);
    nonCoulombicsSection.processSection(lineElements, *_engine);
    EXPECT_EQ(potential.getNonCoulombPairsVector().size(), 1);
    EXPECT_NO_THROW(dynamic_cast<const potential::LennardJonesPair *>(potential.getNonCoulombPairsVector()[0].get()));

    lineElements = {"0", "1", "1.22", "234.3", "324.3"};
    settings::PotentialSettings::setNonCoulombType(settings::NonCoulombType::BUCKINGHAM);
    EXPECT_NO_THROW(nonCoulombicsSection.processSection(lineElements, *_engine));
    EXPECT_EQ(potential.getNonCoulombPairsVector().size(), 2);
    EXPECT_NO_THROW(dynamic_cast<const potential::BuckinghamPair *>(potential.getNonCoulombPairsVector()[0].get()));

    lineElements = {"0", "1", "1.22", "234.3", "324.3"};
    settings::PotentialSettings::setNonCoulombType(settings::NonCoulombType::MORSE);
    EXPECT_NO_THROW(nonCoulombicsSection.processSection(lineElements, *_engine));
    EXPECT_EQ(potential.getNonCoulombPairsVector().size(), 3);
    EXPECT_NO_THROW(dynamic_cast<const potential::MorsePair *>(potential.getNonCoulombPairsVector()[0].get()));

    lineElements = {"0", "1", "1.22", "234.3", "324.3"};
    settings::PotentialSettings::setNonCoulombType(settings::NonCoulombType::LJ_9_12);
    EXPECT_THROW(nonCoulombicsSection.processSection(lineElements, *_engine), customException::ParameterFileException);
}

TEST_F(TestParameterFileSection, endedNormallyNonCoulombic)
{
    auto nonCoulombicsSection = NonCoulombicsSection();
    ASSERT_NO_THROW(nonCoulombicsSection.endedNormally(true));

    ASSERT_THROW_MSG(nonCoulombicsSection.endedNormally(false),
                     customException::ParameterFileException,
                     "Parameter file noncoulombics section ended abnormally!");
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return ::RUN_ALL_TESTS();
}