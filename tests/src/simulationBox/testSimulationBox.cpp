#include "testSimulationBox.hpp"

#include "exceptions.hpp"
#include "throwWithMessage.hpp"

using namespace std;

/**
 * @brief tests resizeGuff function
 *
 */
TEST_F(TestSimulationBox, resizeGuff)
{
    _simulationBox->resizeGuff(2);
    EXPECT_EQ(_simulationBox->getGuffCoefficients().size(), 2);
    EXPECT_EQ(_simulationBox->getNonCoulombRadiusCutOffs().size(), 2);
    EXPECT_EQ(_simulationBox->getCoulombCoefficients().size(), 2);
    EXPECT_EQ(_simulationBox->getCoulombEnergyCutOffs().size(), 2);
    EXPECT_EQ(_simulationBox->getCoulombForceCutOffs().size(), 2);

    _simulationBox->resizeGuff(0, 2);
    EXPECT_EQ(_simulationBox->getGuffCoefficients()[0].size(), 2);
    EXPECT_EQ(_simulationBox->getNonCoulombRadiusCutOffs()[0].size(), 2);
    EXPECT_EQ(_simulationBox->getCoulombCoefficients()[0].size(), 2);
    EXPECT_EQ(_simulationBox->getCoulombEnergyCutOffs()[0].size(), 2);
    EXPECT_EQ(_simulationBox->getCoulombForceCutOffs()[0].size(), 2);

    _simulationBox->resizeGuff(0, 0, 2);
    EXPECT_EQ(_simulationBox->getGuffCoefficients()[0][0].size(), 2);
    EXPECT_EQ(_simulationBox->getNonCoulombRadiusCutOffs()[0][0].size(), 2);
    EXPECT_EQ(_simulationBox->getCoulombCoefficients()[0][0].size(), 2);
    EXPECT_EQ(_simulationBox->getCoulombEnergyCutOffs()[0][0].size(), 2);
    EXPECT_EQ(_simulationBox->getCoulombForceCutOffs()[0][0].size(), 2);

    _simulationBox->resizeGuff(0, 0, 0, 2);
    EXPECT_EQ(_simulationBox->getGuffCoefficients()[0][0][0].size(), 2);
    EXPECT_EQ(_simulationBox->getNonCoulombRadiusCutOffs()[0][0][0].size(), 2);
    EXPECT_EQ(_simulationBox->getCoulombCoefficients()[0][0][0].size(), 2);
    EXPECT_EQ(_simulationBox->getCoulombEnergyCutOffs()[0][0][0].size(), 2);
    EXPECT_EQ(_simulationBox->getCoulombForceCutOffs()[0][0][0].size(), 2);
}

/**
 * @brief tests numberOfAtoms function
 *
 */
TEST_F(TestSimulationBox, numberOfAtoms) { EXPECT_EQ(_simulationBox->getNumberOfAtoms(), 5); }

/**
 * @brief tests calculateDegreesOfFreedom function
 *
 */
TEST_F(TestSimulationBox, calculateDegreesOfFreedom)
{
    _simulationBox->calculateDegreesOfFreedom();
    EXPECT_EQ(_simulationBox->getDegreesOfFreedom(), 12);
}

/**
 * @brief tests calculateCenterOfMass function
 *
 */
TEST_F(TestSimulationBox, centerOfMassOfMolecules)
{
    _simulationBox->calculateCenterOfMassMolecules();

    auto molecules = _simulationBox->getMolecules();

    EXPECT_EQ(molecules[0].getCenterOfMass(), linearAlgebra::Vec3D(1 / 3.0, 0.5, 0.0));
    EXPECT_EQ(molecules[1].getCenterOfMass(), linearAlgebra::Vec3D(2 / 3.0, 0.0, 0.0));
}

/**
 * @brief tests findMoleculeType function
 *
 */
TEST_F(TestSimulationBox, findMoleculeType)
{
    const auto molecule = _simulationBox->findMoleculeType(1);
    EXPECT_EQ(molecule.getMoltype(), 1);

    EXPECT_THROW(_simulationBox->findMoleculeType(3), customException::RstFileException);
}

/**
 * @brief tests findMoleculeByAtomIndex function
 *
 */
TEST_F(TestSimulationBox, findMoleculeByAtomIndex)
{
    const auto &[molecule1, atomIndex1] = _simulationBox->findMoleculeByAtomIndex(3);
    EXPECT_EQ(molecule1, &(_simulationBox->getMolecules()[0]));
    EXPECT_EQ(atomIndex1, 2);

    const auto &[molecule2, atomIndex2] = _simulationBox->findMoleculeByAtomIndex(4);
    EXPECT_EQ(molecule2, &(_simulationBox->getMolecules()[1]));
    EXPECT_EQ(atomIndex2, 0);

    EXPECT_THROW(_simulationBox->findMoleculeByAtomIndex(6), customException::UserInputException);
}

/**
 * @brief tests checkCoulombRadiusCutoff function if the radius cut off is larger than half of the minimal box
 */
TEST_F(TestSimulationBox, checkCoulombRadiusCutoff)
{
    _simulationBox->setCoulombRadiusCutOff(1.0);
    _simulationBox->setBoxDimensions({1.99, 10.0, 10.0});

    EXPECT_THROW_MSG(_simulationBox->checkCoulombRadiusCutOff(customException::ExceptionType::USERINPUTEXCEPTION),
                     customException::UserInputException,
                     "Coulomb radius cut off is larger than half of the minimal box dimension");

    EXPECT_THROW_MSG(_simulationBox->checkCoulombRadiusCutOff(customException::ExceptionType::MANOSTATEXCEPTION),
                     customException::ManostatException,
                     "Coulomb radius cut off is larger than half of the minimal box dimension");

    _simulationBox->setBoxDimensions({10.0, 1.99, 10.0});

    EXPECT_THROW_MSG(_simulationBox->checkCoulombRadiusCutOff(customException::ExceptionType::USERINPUTEXCEPTION),
                     customException::UserInputException,
                     "Coulomb radius cut off is larger than half of the minimal box dimension");

    _simulationBox->setBoxDimensions({10.0, 10.0, 1.99});

    EXPECT_THROW_MSG(_simulationBox->checkCoulombRadiusCutOff(customException::ExceptionType::USERINPUTEXCEPTION),
                     customException::UserInputException,
                     "Coulomb radius cut off is larger than half of the minimal box dimension");
}

/**
 * @brief tests setup external to internal global vdw types map
 *
 */
TEST_F(TestSimulationBox, setupExternalToInternalGlobalVdwTypesMap)
{
    auto *molecule1 = &(_simulationBox->getMolecule(0));
    auto *molecule2 = &(_simulationBox->getMolecule(1));

    molecule1->addExternalGlobalVDWType(1);
    molecule1->addExternalGlobalVDWType(3);
    molecule1->addExternalGlobalVDWType(5);

    molecule2->addExternalGlobalVDWType(3);
    molecule2->addExternalGlobalVDWType(5);

    _simulationBox->setupExternalToInternalGlobalVdwTypesMap();

    EXPECT_EQ(_simulationBox->getExternalGlobalVdwTypes().size(), 3);
    EXPECT_EQ(_simulationBox->getExternalGlobalVdwTypes(), std::vector<size_t>({1, 3, 5}));

    EXPECT_EQ(_simulationBox->getExternalToInternalGlobalVDWTypes().size(), 3);
    EXPECT_EQ(_simulationBox->getExternalToInternalGlobalVDWTypes().at(1), 0);
    EXPECT_EQ(_simulationBox->getExternalToInternalGlobalVDWTypes().at(3), 1);
    EXPECT_EQ(_simulationBox->getExternalToInternalGlobalVDWTypes().at(5), 2);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return ::RUN_ALL_TESTS();
}
