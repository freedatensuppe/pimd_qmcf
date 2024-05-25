/*****************************************************************************
<GPL_HEADER>

    PQ
    Copyright (C) 2023-now  Jakob Gamper

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

<GPL_HEADER>
******************************************************************************/

#include <gtest/gtest.h>   // for Test, EXPECT_EQ, TestInfo (pt...

#include <memory>   // for allocator

#include "coulombPotential.hpp"          // for CoulombPotential, potential
#include "coulombShiftedPotential.hpp"   // for CoulombShiftedPotential
#include "gtest/gtest.h"                 // for Message, TestPartResult

using namespace potential;

/**
 * @brief tests the general constructor of a coulombPotential
 */
TEST(TestCoulombPotential, constructor)
{
    auto potential = CoulombShiftedPotential(2.0);
    EXPECT_EQ(potential.getCoulombRadiusCutOff(), 2.0);
    EXPECT_EQ(potential.getCoulombEnergyCutOff(), 0.5);
    EXPECT_EQ(potential.getCoulombForceCutOff(), 0.25);
}

/**
 * @brief tests the setCoulombRadiusCutOff function
 */
TEST(TestCoulombPotential, setCoulombRadiusCutOff)
{
    CoulombPotential::setCoulombRadiusCutOff(2.0);

    EXPECT_EQ(CoulombPotential::getCoulombRadiusCutOff(), 2.0);
    EXPECT_EQ(CoulombPotential::getCoulombEnergyCutOff(), 0.5);
    EXPECT_EQ(CoulombPotential::getCoulombForceCutOff(), 0.25);
}