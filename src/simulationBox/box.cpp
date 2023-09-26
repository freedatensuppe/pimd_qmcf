/*****************************************************************************
<GPL_HEADER>

    PIMD-QMCF
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

#include "box.hpp"

#include "constants/conversionFactors.hpp"   // for _KG_PER_LITER_TO_AMU_PER_ANGSTROM_CUBIC_

#include <cmath>   // for cos, M_PI, cbrt, sqrt

using namespace simulationBox;

/**
 * @brief Calculate the volume of the box
 *
 * @details
 *  The volume is calculated using the formula:
 *  V = a * b * c * sqrt(1 - cos(alpha)^2 - cos(beta)^2 - cos(gamma)^2 + 2 *
 * cos(alpha) * cos(beta) * cos(gamma)) where a, b, c are the box dimensions and
 * alpha, beta, gamma are the box angles.
 *
 *  The volume is stored in the _volume attribute and returned.
 *  The density is also calculated and stored in the _density attribute.
 *
 * @return volume
 */
double Box::calculateVolume()
{
    _volume = _boxDimensions[0] * _boxDimensions[1] * _boxDimensions[2];

    const double cos_alpha = ::cos(_boxAngles[0] * M_PI / 180.0);
    const double cos_beta  = ::cos(_boxAngles[1] * M_PI / 180.0);
    const double cos_gamma = ::cos(_boxAngles[2] * M_PI / 180.0);

    _volume *= ::sqrt(1.0 - cos_alpha * cos_alpha - cos_beta * cos_beta - cos_gamma * cos_gamma +
                      2.0 * cos_alpha * cos_beta * cos_gamma);

    return _volume;
}

/**
 * @brief Calculate the box dimensions from the density
 *
 * @return vector<double>
 */
linearAlgebra::Vec3D Box::calculateBoxDimensionsFromDensity()
{
    _volume = _totalMass / (_density * constants::_KG_PER_LITER_TO_AMU_PER_ANGSTROM_CUBIC_);

    return linearAlgebra::Vec3D(::cbrt(_volume));
}

/**
 * @brief applies the periodic boundary conditions
 *
 * @TODO: implement this for non-orthogonal boxes
 *
 * @param position
 */
void Box::applyPBC(linearAlgebra::Vec3D &position) const { position -= _boxDimensions * round(position / _boxDimensions); }

/**
 * @brief scales the cell dimensions and recalculates the volume
 *
 * @param scalingFactors
 */
void Box::scaleBox(const linearAlgebra::Vec3D &scalingFactors)
{
    _boxDimensions *= scalingFactors;
    calculateVolume();
}