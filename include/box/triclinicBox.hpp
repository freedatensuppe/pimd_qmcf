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

#ifndef _TRICLINIC_BOX_HPP_

#define _TRICLINIC_BOX_HPP_

#include "box.hpp"         // for Box
#include "constants.hpp"   // for _DEG_TO_RAD_

namespace simulationBox
{
    std::pair<linearAlgebra::Vec3D, linearAlgebra::Vec3D>
    calculateBoxDimensionsAndAnglesFromBoxMatrix(const linearAlgebra::tensor3D &boxMatrix);

    /**
     * @class TriclinicBox
     *
     * @brief This class represents the unit cell of a triclinic box
     *
     */
    class TriclinicBox : public Box
    {
      private:
        linearAlgebra::Vec3D    _boxAngles;
        linearAlgebra::tensor3D _boxMatrix{0.0};
        linearAlgebra::tensor3D _transformationMatrix{0.0};

        void calculateBoxMatrix();
        void calculateTransformationMatrix();

      public:
        [[nodiscard]] double               calculateVolume() override;
        [[nodiscard]] linearAlgebra::Vec3D calculateShiftVector(const linearAlgebra::Vec3D &position) const override;
        [[nodiscard]] linearAlgebra::Vec3D transformIntoOrthogonalSpace(const linearAlgebra::Vec3D &position) const override;
        [[nodiscard]] linearAlgebra::tensor3D
                                           transformIntoOrthogonalSpace(const linearAlgebra::tensor3D &position) const override;
        [[nodiscard]] linearAlgebra::Vec3D transformIntoSimulationSpace(const linearAlgebra::Vec3D &position) const override;
        [[nodiscard]] linearAlgebra::tensor3D
        transformIntoSimulationSpace(const linearAlgebra::tensor3D &position) const override;

        void applyPBC(linearAlgebra::Vec3D &position) const override;
        void scaleBox(const linearAlgebra::tensor3D &scalingTensor) override;

        void setBoxAngles(const linearAlgebra::Vec3D &boxAngles);
        void setBoxDimensions(const linearAlgebra::Vec3D &boxDimensions) override;

        [[nodiscard]] double cosAlpha() const { return ::cos(_boxAngles[0]); }
        [[nodiscard]] double cosBeta() const { return ::cos(_boxAngles[1]); }
        [[nodiscard]] double cosGamma() const { return ::cos(_boxAngles[2]); }
        [[nodiscard]] double sinAlpha() const { return ::sin(_boxAngles[0]); }
        [[nodiscard]] double sinBeta() const { return ::sin(_boxAngles[1]); }
        [[nodiscard]] double sinGamma() const { return ::sin(_boxAngles[2]); }

        [[nodiscard]] linearAlgebra::Vec3D    getBoxAngles() const override { return _boxAngles * constants::_RAD_TO_DEG_; }
        [[nodiscard]] linearAlgebra::tensor3D getBoxMatrix() const override { return _boxMatrix; }
        [[nodiscard]] linearAlgebra::tensor3D getTransformationMatrix() const { return _transformationMatrix; }
    };

}   // namespace simulationBox

#endif   // _TRICLINIC_BOX_HPP_