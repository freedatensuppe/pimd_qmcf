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

#ifndef _COULOMB_POTENTIAL_HPP_

#define _COULOMB_POTENTIAL_HPP_

#include "defaults.hpp"   // for _COULOMB_CUT_OFF_DEFAULT_

#include <cstddef>   // for size_t
#include <utility>   // for pair
#include <vector>    // for vector

namespace potential
{
    using c_ul     = const size_t;
    using vector4d = std::vector<std::vector<std::vector<std::vector<double>>>>;

    /**
     * @class CoulombPotential
     *
     * @brief CoulombPotential is a base class for all coulomb type potentials
     *
     * @details it inherits in general the coulombRadiusCutOff, coulombEnergyCutOff and coulombForceCutOff to all coulomb type
     * potentials
     *
     * @details available for:
     * - shifted potential
     * - wolf summation as long range correction
     *
     */
    class CoulombPotential
    {
      protected:
        inline static double _coulombRadiusCutOff = defaults::_COULOMB_CUT_OFF_DEFAULT_;
        inline static double _coulombEnergyCutOff = 1 / _coulombRadiusCutOff;
        inline static double _coulombForceCutOff  = 1 / (_coulombRadiusCutOff * _coulombRadiusCutOff);

      public:
        virtual ~CoulombPotential() = default;
        explicit CoulombPotential(const double coulombRadiusCutOff);

        virtual std::pair<double, double> calculate(const double, const double) const = 0;

        static void setCoulombRadiusCutOff(const double coulombRadiusCutOff);

        /***************************
         *                         *
         * standard getter methods *
         *                         *
         ***************************/

        static void setCoulombEnergyCutOff(const double coulombEnergyCutOff) { _coulombEnergyCutOff = coulombEnergyCutOff; }
        static void setCoulombForceCutOff(const double coulombForceCutOff) { _coulombForceCutOff = coulombForceCutOff; }

        /***************************
         *                         *
         * standard getter methods *
         *                         *
         ***************************/

        [[nodiscard]] static double getCoulombRadiusCutOff() { return _coulombRadiusCutOff; }
        [[nodiscard]] static double getCoulombEnergyCutOff() { return _coulombEnergyCutOff; }
        [[nodiscard]] static double getCoulombForceCutOff() { return _coulombForceCutOff; }
    };

}   // namespace potential

#endif   // _COULOMB_POTENTIAL_HPP_