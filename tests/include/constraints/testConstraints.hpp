#ifndef _TEST_CONSTRAINTS_HPP_

#define _TEST_CONSTRAINTS_HPP_

#include "bondConstraint.hpp"   // for BondConstraint
#include "constraints.hpp"      // for Constraints
#include "molecule.hpp"         // for Molecule
#include "simulationBox.hpp"    // for SimulationBox
#include "vector3d.hpp"         // for Vec3D

#include <gtest/gtest.h>   // for Test
#include <vector>          // for vector

/**
 * @class TestConstraints
 *
 * @brief Fixture for constraint tests.
 *
 */
class TestConstraints : public ::testing::Test
{
  protected:
    virtual void SetUp()
    {
        auto molecule1 = simulationBox::Molecule();
        molecule1.setNumberOfAtoms(3);

        auto atom1 = std::make_shared<simulationBox::Atom>();
        auto atom2 = std::make_shared<simulationBox::Atom>();
        auto atom3 = std::make_shared<simulationBox::Atom>();

        atom1->setPosition(linearAlgebra::Vec3D(1.0, 1.0, 1.0));
        atom2->setPosition(linearAlgebra::Vec3D(1.0, 2.0, 3.0));
        atom3->setPosition(linearAlgebra::Vec3D(2.0, 0.0, 0.0));
        atom1->setVelocity(linearAlgebra::Vec3D(0.0, 0.0, 0.0));
        atom2->setVelocity(linearAlgebra::Vec3D(1.0, 1.0, 1.0));
        atom3->setVelocity(linearAlgebra::Vec3D(2.0, 2.0, 2.0));
        atom1->setMass(1.0);
        atom2->setMass(1.0);
        atom3->setMass(1.0);

        molecule1.addAtom(atom1);
        molecule1.addAtom(atom2);
        molecule1.addAtom(atom3);

        auto molecule2 = simulationBox::Molecule();
        molecule2.setNumberOfAtoms(2);

        auto atom4 = std::make_shared<simulationBox::Atom>();
        auto atom5 = std::make_shared<simulationBox::Atom>();

        atom4->setPosition(linearAlgebra::Vec3D(1.0, 1.0, 1.0));
        atom5->setPosition(linearAlgebra::Vec3D(1.0, 2.0, 3.0));
        atom4->setVelocity(linearAlgebra::Vec3D(0.0, 0.0, 0.0));
        atom5->setVelocity(linearAlgebra::Vec3D(1.0, 1.0, 1.0));
        atom4->setMass(1.0);
        atom5->setMass(1.0);

        molecule2.addAtom(atom4);
        molecule2.addAtom(atom5);

        _box = new simulationBox::SimulationBox();
        _box->addMolecule(molecule1);
        _box->addMolecule(molecule2);
        _box->setBoxDimensions(linearAlgebra::Vec3D(10.0, 10.0, 10.0));

        _constraints = new constraints::Constraints();

        auto bondConstraint1 = constraints::BondConstraint(&(_box->getMolecules()[0]), &(_box->getMolecules()[0]), 0, 1, 1.2);
        auto bondConstraint2 = constraints::BondConstraint(&(_box->getMolecules()[0]), &(_box->getMolecules()[1]), 2, 1, 1.3);

        _constraints->addBondConstraint(bondConstraint1);
        _constraints->addBondConstraint(bondConstraint2);

        _constraints->activate();
    }

    virtual void TearDown()
    {
        delete _box;
        delete _constraints;
    }

    simulationBox::SimulationBox *_box;
    constraints::Constraints     *_constraints;
};

#endif   // _TEST_CONSTRAINTS_HPP_