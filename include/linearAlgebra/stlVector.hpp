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

#ifndef _STL_VECTOR_HPP_

#define _STL_VECTOR_HPP_

#include "staticMatrix3x3.hpp"   // for StaticMatrix3x3

#include <algorithm>   // for max_element
#include <numeric>     // for accumulate
#include <vector>      // for vector

namespace std
{
    /**
     * @brief Calculates the sum of all elements in a vector
     *
     * @param vector
     *
     * @return sum of all elements in vector
     */
    template <typename T>
    T sum(const std::vector<T> &vector)
    {
        return std::accumulate(vector.begin(), vector.end(), T());
    }

    /**
     * @brief Calculates the mean of all elements in a vector
     *
     * @param vector
     *
     * @return mean of all elements in vector
     */
    template <typename T>
    double mean(const std::vector<T> &vector)
    {
        return sum(vector) / double(vector.size());
    }

    /**
     * @brief Calculates the maximum of all elements in a vector
     *
     * @param vector
     *
     * @return maximum of all elements in vector
     */
    template <typename T>
    T max(const std::vector<T> &vector)
    {
        return *std::ranges::max_element(vector);
    }

}   // namespace std

#endif   // _STL_VECTOR_HPP_