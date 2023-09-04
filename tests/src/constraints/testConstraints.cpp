#include "testConstraints.hpp"

#include "exceptions.hpp"         // for ShakeException
#include "throwWithMessage.hpp"   // for EXPECT_THROW_MSG
#include "timingsSettings.hpp"    // for TimingsSettings

#include "gmock/gmock.h"   // for DoubleNear, ElementsAre, MakePredica...
#include "gtest/gtest.h"   // for Message, TestPartResult, InitGoogleTest
#include <string>          // for basic_string

/**
 * @brief tests calculation of all bond constraints ref bond lengths
 *
 */
TEST_F(TestConstraints, calcRefBondLengths)
{
    _constraints->calculateConstraintBondRefs(*_box);
    EXPECT_EQ(_constraints->getBondConstraints()[0].getShakeDistanceRef(), linearAlgebra::Vec3D(0.0, -1.0, -2.0));
    EXPECT_EQ(_constraints->getBondConstraints()[1].getShakeDistanceRef(), linearAlgebra::Vec3D(1.0, -2.0, -3.0));
}

/**
 * @brief test apply shake algorithm to all bond constraints
 *
 */
TEST_F(TestConstraints, applyShake_converged)
{
    _constraints->setShakeMaxIter(1000);
    _constraints->setShakeTolerance(1.0e-4);
    _constraints->calculateConstraintBondRefs(*_box);

    settings::TimingsSettings::setTimeStep(2.0);

    EXPECT_NO_THROW(_constraints->applyShake(*_box));

    EXPECT_THAT(_box->getMolecules()[0].getAtomPosition(0),
                testing::ElementsAre(
                    testing::DoubleNear(1.0, 1e-5), testing::DoubleNear(1.23165, 1e-5), testing::DoubleNear(1.46331, 1e-5)));
    EXPECT_THAT(_box->getMolecules()[0].getAtomPosition(1),
                testing::ElementsAre(
                    testing::DoubleNear(1.0, 1e-5), testing::DoubleNear(1.76835, 1e-5), testing::DoubleNear(2.53669, 1e-5)));
    EXPECT_THAT(_box->getMolecules()[0].getAtomVelocity(0),
                testing::ElementsAre(
                    testing::DoubleNear(0.0, 1e-5), testing::DoubleNear(0.115827, 1e-5), testing::DoubleNear(0.231654, 1e-5)));
    EXPECT_THAT(_box->getMolecules()[0].getAtomVelocity(1),
                testing::ElementsAre(
                    testing::DoubleNear(1.0, 1e-5), testing::DoubleNear(0.884173, 1e-5), testing::DoubleNear(0.768346, 1e-5)));
}

/**
 * @brief test apply shake algorithm to all bond constraints and it does not converge
 *
 */
TEST_F(TestConstraints, applyShake_notConverged)
{
    _constraints->setShakeMaxIter(10);
    _constraints->setShakeTolerance(0.0);
    _constraints->calculateConstraintBondRefs(*_box);

    settings::TimingsSettings::setTimeStep(2.0);

    EXPECT_THROW_MSG(_constraints->applyShake(*_box), customException::ShakeException, "Shake algorithm did not converge.");
}

/**
 * @brief test apply shake algorithm to all bond constraints and it does not converge but is deactivated
 *
 */
TEST_F(TestConstraints, applyShake_notConverged_deactivated)
{
    _constraints->setShakeMaxIter(10);
    _constraints->setShakeTolerance(0.0);
    _constraints->calculateConstraintBondRefs(*_box);

    settings::TimingsSettings::setTimeStep(2.0);

    _constraints->deactivate();

    EXPECT_NO_THROW(_constraints->applyShake(*_box));
}

/**
 * @brief test apply rattle algorithm to all bond constraints
 *
 */
TEST_F(TestConstraints, applyRattle_converged)
{
    _constraints->setRattleMaxIter(1000);
    _constraints->setRattleTolerance(1.0e-4);
    _constraints->calculateConstraintBondRefs(*_box);

    EXPECT_NO_THROW(_constraints->applyRattle());

    EXPECT_THAT(
        _box->getMolecules()[0].getAtomVelocity(0),
        testing::ElementsAre(testing::DoubleNear(0.0, 1e-5), testing::DoubleNear(0.3, 1e-5), testing::DoubleNear(0.6, 1e-5)));
    EXPECT_THAT(
        _box->getMolecules()[0].getAtomVelocity(1),
        testing::ElementsAre(testing::DoubleNear(1.0, 1e-5), testing::DoubleNear(0.7, 1e-5), testing::DoubleNear(0.4, 1e-5)));
}

/**
 * @brief test apply rattle algorithm to all bond constraints and it does not converge
 *
 */
TEST_F(TestConstraints, applyRattle_notConverged)
{
    _constraints->setRattleMaxIter(10);
    _constraints->setRattleTolerance(0.0);
    _constraints->calculateConstraintBondRefs(*_box);

    EXPECT_THROW_MSG(_constraints->applyRattle(), customException::ShakeException, "Rattle algorithm did not converge.");
}

/**
 * @brief test apply rattle algorithm to all bond constraints and it does not converge but is deactivated
 *
 */
TEST_F(TestConstraints, applyRattle_notConverged_deactivated)
{
    _constraints->setRattleMaxIter(10);
    _constraints->setRattleTolerance(0.0);
    _constraints->calculateConstraintBondRefs(*_box);

    _constraints->deactivate();

    EXPECT_NO_THROW(_constraints->applyRattle());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return ::RUN_ALL_TESTS();
}