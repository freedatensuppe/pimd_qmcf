#include "constants.hpp"               // for _AMU_PER_ANGSTROM_CUBIC_TO_KG_P...
#include "engine.hpp"                  // for Engine
#include "exceptions.hpp"              // for MolDescriptorException, InputFi...
#include "molecule.hpp"                // for Molecule
#include "simulationBox.hpp"           // for SimulationBox
#include "simulationBoxSettings.hpp"   // for SimulationBoxSettings
#include "simulationBoxSetup.hpp"      // for SimulationBoxSetup, setupSimula...
#include "testSetup.hpp"               // for TestSetup
#include "vector3d.hpp"                // for Vec3D

#include "gtest/gtest.h"   // for Message, TestPartResult, testing
#include <cmath>           // for cbrt
#include <gtest/gtest.h>   // for CmpHelperFloatingPointEQ, TestI...
#include <string>          // for allocator, basic_string
#include <vector>          // for vector

using namespace setup;

TEST_F(TestSetup, testSetAtomMasses)
{
    simulationBox::Molecule molecule(1);
    molecule.setNumberOfAtoms(3);
    molecule.addAtomName("C");
    molecule.addAtomName("H");
    molecule.addAtomName("O");

    _engine.getSimulationBox().getMolecules().push_back(molecule);
    SimulationBoxSetup simulationBoxSetup(_engine);
    simulationBoxSetup.setAtomMasses();

    EXPECT_DOUBLE_EQ(_engine.getSimulationBox().getMolecules()[0].getAtomMass(0), 12.0107);
    EXPECT_DOUBLE_EQ(_engine.getSimulationBox().getMolecules()[0].getAtomMass(1), 1.00794);
    EXPECT_DOUBLE_EQ(_engine.getSimulationBox().getMolecules()[0].getAtomMass(2), 15.9994);
}

TEST_F(TestSetup, testSetAtomMassesThrowsError)
{
    simulationBox::Molecule molecule(1);
    molecule.setNumberOfAtoms(3);
    molecule.addAtomName("H");
    molecule.addAtomName("D");
    molecule.addAtomName("NotAnAtomName");

    _engine.getSimulationBox().getMolecules().push_back(molecule);
    SimulationBoxSetup simulationBoxSetup(_engine);
    ASSERT_THROW(simulationBoxSetup.setAtomMasses(), customException::MolDescriptorException);
}

TEST_F(TestSetup, testSetAtomicNumbers)
{
    simulationBox::Molecule molecule(1);
    molecule.setNumberOfAtoms(3);
    molecule.addAtomName("C");
    molecule.addAtomName("H");
    molecule.addAtomName("O");

    _engine.getSimulationBox().getMolecules().push_back(molecule);
    SimulationBoxSetup simulationBoxSetup(_engine);
    simulationBoxSetup.setAtomicNumbers();

    EXPECT_DOUBLE_EQ(_engine.getSimulationBox().getMolecules()[0].getAtomicNumber(0), 6);
    EXPECT_DOUBLE_EQ(_engine.getSimulationBox().getMolecules()[0].getAtomicNumber(1), 1);
    EXPECT_DOUBLE_EQ(_engine.getSimulationBox().getMolecules()[0].getAtomicNumber(2), 8);
}

TEST_F(TestSetup, testSetAtomicNumbersThrowsError)
{
    simulationBox::Molecule molecule(1);
    molecule.setNumberOfAtoms(3);
    molecule.addAtomName("H");
    molecule.addAtomName("D");
    molecule.addAtomName("NotAnAtomName");

    _engine.getSimulationBox().getMolecules().push_back(molecule);
    SimulationBoxSetup simulationBoxSetup(_engine);
    ASSERT_THROW(simulationBoxSetup.setAtomicNumbers(), customException::MolDescriptorException);
}

TEST_F(TestSetup, testSetTotalMass)
{
    simulationBox::Molecule molecule1(1);
    molecule1.setNumberOfAtoms(3);
    molecule1.addAtomName("C");
    molecule1.addAtomName("H");
    molecule1.addAtomName("O");

    simulationBox::Molecule molecule2(2);
    molecule2.setNumberOfAtoms(2);
    molecule2.addAtomName("H");
    molecule2.addAtomName("H");

    _engine.getSimulationBox().getMolecules().push_back(molecule1);
    _engine.getSimulationBox().getMolecules().push_back(molecule2);
    SimulationBoxSetup simulationBoxSetup(_engine);
    simulationBoxSetup.setAtomMasses();
    simulationBoxSetup.calculateMolMasses();
    simulationBoxSetup.calculateTotalMass();

    EXPECT_DOUBLE_EQ(_engine.getSimulationBox().getTotalMass(), 12.0107 + 3 * 1.00794 + 15.9994);
}

TEST_F(TestSetup, testSetMolMass)
{
    simulationBox::Molecule molecule1(1);
    molecule1.setNumberOfAtoms(3);
    molecule1.addAtomName("C");
    molecule1.addAtomName("H");
    molecule1.addAtomName("O");

    simulationBox::Molecule molecule2(2);
    molecule2.setNumberOfAtoms(2);
    molecule2.addAtomName("H");
    molecule2.addAtomName("H");

    _engine.getSimulationBox().getMolecules().push_back(molecule1);
    _engine.getSimulationBox().getMolecules().push_back(molecule2);
    SimulationBoxSetup simulationBoxSetup(_engine);
    simulationBoxSetup.setAtomMasses();
    simulationBoxSetup.calculateMolMasses();

    EXPECT_DOUBLE_EQ(_engine.getSimulationBox().getMolecules()[0].getMolMass(), 12.0107 + 1 * 1.00794 + 15.9994);
}

TEST_F(TestSetup, testSetTotalCharge)
{
    simulationBox::Molecule molecule1(1);
    molecule1.setNumberOfAtoms(3);
    molecule1.addAtomName("C");
    molecule1.addAtomName("H");
    molecule1.addAtomName("O");

    molecule1.addPartialCharge(0.1);
    molecule1.addPartialCharge(0.2);
    molecule1.addPartialCharge(-0.4);

    _engine.getSimulationBox().getMolecules().push_back(molecule1);
    SimulationBoxSetup simulationBoxSetup(_engine);
    simulationBoxSetup.calculateTotalCharge();

    EXPECT_DOUBLE_EQ(_engine.getSimulationBox().getTotalCharge(), -0.1);
}

TEST_F(TestSetup, testNoDensityNoBox)
{
    settings::SimulationBoxSettings::setDensitySet(false);
    settings::SimulationBoxSettings::setBoxSet(false);
    SimulationBoxSetup simulationBoxSetup(_engine);
    ASSERT_THROW(simulationBoxSetup.checkBoxSettings(), customException::UserInputException);
}

TEST_F(TestSetup, testNoDensity)
{
    _engine.getSimulationBox().setTotalMass(6000);
    _engine.getSimulationBox().setBoxDimensions({10.0, 20.0, 30.0});
    settings::SimulationBoxSettings::setDensitySet(false);
    settings::SimulationBoxSettings::setBoxSet(true);
    SimulationBoxSetup simulationBoxSetup(_engine);
    simulationBoxSetup.checkBoxSettings();

    EXPECT_DOUBLE_EQ(_engine.getSimulationBox().getVolume(), 6000.0);
    EXPECT_DOUBLE_EQ(_engine.getSimulationBox().getDensity(), constants::_AMU_PER_ANGSTROM_CUBIC_TO_KG_PER_LITER_CUBIC_);
}

TEST_F(TestSetup, testNoBox)
{
    _engine.getSimulationBox().setTotalMass(6000);
    _engine.getSimulationBox().setDensity(constants::_AMU_PER_ANGSTROM_CUBIC_TO_KG_PER_LITER_CUBIC_);
    settings::SimulationBoxSettings::setBoxSet(false);
    settings::SimulationBoxSettings::setDensitySet(true);
    SimulationBoxSetup simulationBoxSetup(_engine);
    simulationBoxSetup.checkBoxSettings();

    EXPECT_DOUBLE_EQ(_engine.getSimulationBox().getVolume(), 6000.0);
    EXPECT_DOUBLE_EQ(_engine.getSimulationBox().getBoxDimensions()[2], cbrt(6000.0));
}

TEST_F(TestSetup, testBoxAndDensitySet)
{
    _engine.getSimulationBox().setTotalMass(6000);
    _engine.getSimulationBox().setDensity(12341243.1234);   // this should be ignored
    _engine.getSimulationBox().setBoxDimensions({10.0, 20.0, 30.0});
    settings::SimulationBoxSettings::setDensitySet(true);
    settings::SimulationBoxSettings::setBoxSet(true);
    SimulationBoxSetup simulationBoxSetup(_engine);
    simulationBoxSetup.checkBoxSettings();

    EXPECT_DOUBLE_EQ(_engine.getSimulationBox().getVolume(), 6000.0);
    EXPECT_DOUBLE_EQ(_engine.getSimulationBox().getDensity(), constants::_AMU_PER_ANGSTROM_CUBIC_TO_KG_PER_LITER_CUBIC_);
}

TEST_F(TestSetup, testResizeAtomShiftForces)
{

    simulationBox::Molecule molecule1(1);
    molecule1.setNumberOfAtoms(2);

    simulationBox::Molecule molecule2(2);
    molecule2.setNumberOfAtoms(1);

    _engine.getSimulationBox().addMolecule(molecule1);
    _engine.getSimulationBox().addMolecule(molecule2);

    SimulationBoxSetup simulationBoxSetup(_engine);
    simulationBoxSetup.resizeAtomShiftForces();

    EXPECT_EQ(_engine.getSimulationBox().getMolecules()[0].getAtomShiftForces().size(), 2);
    EXPECT_EQ(_engine.getSimulationBox().getMolecules()[1].getAtomShiftForces().size(), 1);
}

TEST_F(TestSetup, testCheckRcCutoff)
{
    _engine.getSimulationBox().setBoxDimensions({10.0, 20.0, 30.0});
    _engine.getSimulationBox().setCoulombRadiusCutOff(14.0);
    SimulationBoxSetup simulationBoxSetup(_engine);
    EXPECT_THROW(simulationBoxSetup.checkRcCutoff(), customException::InputFileException);

    SimulationBoxSetup simulationBox2Setup(_engine);
    _engine.getSimulationBox().setCoulombRadiusCutOff(4.0);
    EXPECT_NO_THROW(simulationBox2Setup.checkRcCutoff());
}

/**
 * @brief testing full setup of simulation box
 *
 * @TODO: this test is not complete, it only tests the functions that are called in the setup
 *
 */
TEST_F(TestSetup, testFullSetup)
{
    simulationBox::Molecule molecule1(1);
    molecule1.setNumberOfAtoms(3);
    molecule1.addAtomName("C");
    molecule1.addAtomName("H");
    molecule1.addAtomName("O");
    molecule1.addPartialCharge(0.1);
    molecule1.addPartialCharge(0.2);
    molecule1.addPartialCharge(-0.4);

    simulationBox::Molecule molecule2(2);
    molecule2.setNumberOfAtoms(2);
    molecule2.addAtomName("H");
    molecule2.addAtomName("H");
    molecule2.addPartialCharge(0.1);
    molecule2.addPartialCharge(0.2);

    _engine.getSimulationBox().getMolecules().push_back(molecule1);
    _engine.getSimulationBox().getMolecules().push_back(molecule2);

    _engine.getSimulationBox().setTotalMass(33.0);
    _engine.getSimulationBox().setDensity(12341243.1234);   // this should be ignored
    _engine.getSimulationBox().setBoxDimensions({10.0, 20.0, 30.0});
    _engine.getSimulationBox().setCoulombRadiusCutOff(4.0);

    EXPECT_NO_THROW(setupSimulationBox(_engine));
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return ::RUN_ALL_TESTS();
}