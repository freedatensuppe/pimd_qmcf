#include "testMolecule.hpp"

#include "gtest/gtest.h"   // for Message, TestPartResult

TEST_F(TestMolecule, calculateCenterOfMass)
{
    const linearAlgebra::Vec3D box          = {10.0, 10.0, 10.0};
    const linearAlgebra::Vec3D centerOfMass = {1.0 / 3.0, 1.0 / 2.0, 0.0};

    _molecule->calculateCenterOfMass(box);
    EXPECT_EQ(_molecule->getCenterOfMass(), centerOfMass);
}

TEST_F(TestMolecule, scaleAtoms)
{
    const linearAlgebra::Vec3D scale         = {1.0, 2.0, 3.0};
    const linearAlgebra::Vec3D atomPosition1 = _molecule->getAtomPosition(0);
    const linearAlgebra::Vec3D atomPosition2 = _molecule->getAtomPosition(1);
    const linearAlgebra::Vec3D atomPosition3 = _molecule->getAtomPosition(2);

    _molecule->calculateCenterOfMass({10.0, 10.0, 10.0});

    const auto                 centerOfMassBeforeScaling = _molecule->getCenterOfMass();
    const linearAlgebra::Vec3D shift                     = centerOfMassBeforeScaling * (scale - 1.0);

    _molecule->scale(scale);

    EXPECT_EQ(_molecule->getAtomPosition(0), atomPosition1 + shift);
    EXPECT_EQ(_molecule->getAtomPosition(1), atomPosition2 + shift);
    EXPECT_EQ(_molecule->getAtomPosition(2), atomPosition3 + shift);
}

TEST_F(TestMolecule, setAtomForceToZero)
{
    _molecule->setAtomForcesToZero();
    EXPECT_EQ(_molecule->getAtomForce(0), linearAlgebra::Vec3D());
    EXPECT_EQ(_molecule->getAtomForce(1), linearAlgebra::Vec3D());
    EXPECT_EQ(_molecule->getAtomForce(2), linearAlgebra::Vec3D());
}

TEST_F(TestMolecule, getNumberOfAtomTypes) { EXPECT_EQ(_molecule->getNumberOfAtomTypes(), 2); }

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return ::RUN_ALL_TESTS();
}
