#ifndef _TEST_TRAJECTORYOUTPUT_HPP_

#define _TEST_TRAJECTORYOUTPUT_HPP_

#include "atom.hpp"               // for Atom
#include "molecule.hpp"           // for Molecule
#include "simulationBox.hpp"      // for SimulationBox
#include "trajectoryOutput.hpp"   // for TrajectoryOutput
#include "vector3d.hpp"           // for Vec3D

#include <gtest/gtest.h>   // for Test
#include <memory>          // for __shared_ptr_access, shared_ptr, make_shared
#include <stdio.h>         // for remove

/**
 * @class TestTrajectoryOutput
 *
 * @brief test suite for trajectory output
 *
 */
class TestTrajectoryOutput : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        _trajectoryOutput = new output::TrajectoryOutput("default.xyz");
        _simulationBox    = new simulationBox::SimulationBox();

        _simulationBox->setBoxDimensions({10.0, 10.0, 10.0});
        _simulationBox->setBoxAngles({90.0, 90.0, 90.0});

        auto molecule1 = simulationBox::Molecule();

        auto atom1 = std::make_shared<simulationBox::Atom>();
        auto atom2 = std::make_shared<simulationBox::Atom>();

        molecule1.setNumberOfAtoms(2);

        atom1->setPosition(linearAlgebra::Vec3D(1.0, 1.0, 1.0));
        atom2->setPosition(linearAlgebra::Vec3D(1.0, 2.0, 3.0));
        atom1->setForce(linearAlgebra::Vec3D(1.0, 1.0, 1.0));
        atom2->setForce(linearAlgebra::Vec3D(2.0, 3.0, 4.0));
        atom1->setVelocity(linearAlgebra::Vec3D(1.0, 1.0, 1.0));
        atom2->setVelocity(linearAlgebra::Vec3D(3.0, 4.0, 5.0));
        atom1->setName("H");
        atom2->setName("O");
        atom1->setPartialCharge(1.0);
        atom2->setPartialCharge(-1.0);
        molecule1.setMoltype(1);
        molecule1.addAtom(atom1);
        molecule1.addAtom(atom2);

        auto molecule2 = simulationBox::Molecule();

        auto atom3 = std::make_shared<simulationBox::Atom>();

        molecule2.setNumberOfAtoms(1);

        atom3->setPosition(linearAlgebra::Vec3D(1.0, 1.0, 1.0));
        atom3->setForce(linearAlgebra::Vec3D(1.0, 1.0, 1.0));
        atom3->setVelocity(linearAlgebra::Vec3D(1.0, 1.0, 1.0));
        atom3->setName("Ar");
        atom3->setPartialCharge(0.0);
        molecule2.setMoltype(2);
        molecule2.addAtom(atom3);

        _simulationBox->addMolecule(molecule1);
        _simulationBox->addMolecule(molecule2);
        _simulationBox->addAtom(atom1);
        _simulationBox->addAtom(atom2);
        _simulationBox->addAtom(atom3);
    }

    void TearDown() override
    {
        delete _trajectoryOutput;
        delete _simulationBox;
        ::remove("default.xyz");
    }

    output::TrajectoryOutput     *_trajectoryOutput;
    simulationBox::SimulationBox *_simulationBox;
};

#endif   // _TEST_RSTFILEOUTPUT_HPP_