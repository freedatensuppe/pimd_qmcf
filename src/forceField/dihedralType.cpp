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

#include "dihedralType.hpp"

#include "mathUtilities.hpp"

using namespace forceField;

/**
 * @brief operator overload for the comparison of two DihedralType objects
 *
 * @param other
 * @return true
 * @return false
 */
bool DihedralType::operator==(const DihedralType &other) const
{
    auto isEqual = _id == other._id;
    isEqual      = isEqual && utilities::compare(_forceConstant, other._forceConstant);
    isEqual      = isEqual && utilities::compare(_periodicity, other._periodicity);
    isEqual      = isEqual && utilities::compare(_phaseShift, other._phaseShift);

    return isEqual;
}