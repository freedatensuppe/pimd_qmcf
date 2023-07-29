#include "testBondConstraint.hpp"

/**
 * @brief tests calculation of bond constraint ref bond length
 *
 */
TEST_F(TestBondConstraint, calcRefBondLength)
{
    _bondConstraint->calculateConstraintBondRef(*_box);
    EXPECT_EQ(_bondConstraint->getShakeDistanceRef(), vector3d::Vec3D(0.0, -1.0, -2.0));
}

/**
 * @brief tests calculation of bond constraint delta bond length
 *
 */
TEST_F(TestBondConstraint, calculateDistanceDelta)
{
    _bondConstraint->calculateDistanceDelta(*_box);
    const auto distanceSquared         = normSquared(vector3d::Vec3D(0.0, -1.0, -2.0));
    const auto targetBondLengthSquared = _bondConstraint->getTargetBondLength() * _bondConstraint->getTargetBondLength();
    EXPECT_EQ(_bondConstraint->calculateDistanceDelta(*_box),
              0.5 * (targetBondLengthSquared - distanceSquared) / targetBondLengthSquared);
}

/**
 * @brief tests application of shake algorithm to bond constraint
 */
TEST_F(TestBondConstraint, applyShake)
{
    _bondConstraint->calculateConstraintBondRef(*_box);
    const auto delta = _bondConstraint->calculateDistanceDelta(*_box);

    const auto shakeForce = delta / (1.0 + 0.5) / normSquared(vector3d::Vec3D(0.0, -1.0, -2.0));
    const auto dpos       = shakeForce * vector3d::Vec3D(0.0, -1.0, -2.0);
    const auto timestep   = 2.0;

    EXPECT_FALSE(_bondConstraint->applyShake(*_box, 0.0, timestep));

    EXPECT_EQ(_box->getMolecules()[0].getAtomPosition(0), vector3d::Vec3D(1.0, 1.0, 1.0) + dpos);
    EXPECT_EQ(_box->getMolecules()[0].getAtomPosition(1), vector3d::Vec3D(1.0, 2.0, 3.0) - 0.5 * dpos);

    EXPECT_EQ(_box->getMolecules()[0].getAtomVelocity(0), vector3d::Vec3D(0.0, 0.0, 0.0) + dpos / timestep);
    EXPECT_EQ(_box->getMolecules()[0].getAtomVelocity(1), vector3d::Vec3D(1.0, 1.0, 1.0) - 0.5 * dpos / timestep);

    EXPECT_TRUE(_bondConstraint->applyShake(*_box, 1000.0, timestep));
}

/**
 * @brief tests calculation of bond constraint delta velocity
 *
 */
TEST_F(TestBondConstraint, calculateVelocityDelta)
{
    _bondConstraint->calculateConstraintBondRef(*_box);
    _bondConstraint->calculateDistanceDelta(*_box);
    const auto scalarProduct = dot(vector3d::Vec3D(-1.0, -1.0, -1.0), vector3d::Vec3D(0.0, -1.0, -2.0));
    EXPECT_EQ(_bondConstraint->calculateVelocityDelta(), -scalarProduct / (1.0 + 0.5) / 5.0);
}

/**
 * @brief tests application of rattle algorithm to bond constraint
 */
TEST_F(TestBondConstraint, applyRattle)
{
    _bondConstraint->calculateConstraintBondRef(*_box);
    const auto delta = _bondConstraint->calculateVelocityDelta();
    const auto dv    = delta * _bondConstraint->getShakeDistanceRef();

    EXPECT_FALSE(_bondConstraint->applyRattle(0.0));
    EXPECT_EQ(_box->getMolecules()[0].getAtomVelocity(0), vector3d::Vec3D(0.0, 0.0, 0.0) + dv);
    EXPECT_EQ(_box->getMolecules()[0].getAtomVelocity(1), vector3d::Vec3D(1.0, 1.0, 1.0) - 0.5 * dv);

    EXPECT_TRUE(_bondConstraint->applyRattle(1000.0));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return ::RUN_ALL_TESTS();
}