#ifndef _BOX_HPP_

#define _BOX_HPP_

#include "vector3d.hpp"

namespace simulationBox
{
    /**
     * @class Box
     *
     * @brief This class stores all the information about the box.
     *
     * @TODO: think of a way to implement non orthogonal boxes - maybe inheritance?
     *
     */
    class Box
    {
      private:
        linearAlgebra::Vec3D _boxDimensions;
        linearAlgebra::Vec3D _boxAngles = {90.0, 90.0, 90.0};

        double _totalMass;
        double _totalCharge;
        double _density;
        double _volume;

        bool _boxSizeHasChanged = false;

      public:
        double                             calculateVolume();
        [[nodiscard]] linearAlgebra::Vec3D calculateBoxDimensionsFromDensity();

        void applyPBC(linearAlgebra::Vec3D &position) const;
        void scaleBox(const linearAlgebra::Vec3D &scalingFactors);

        /********************
         * standard getters *
         ********************/

        [[nodiscard]] double getMinimalBoxDimension() const { return minimum(_boxDimensions); }

        [[nodiscard]] linearAlgebra::Vec3D getBoxDimensions() const { return _boxDimensions; }
        [[nodiscard]] linearAlgebra::Vec3D getBoxAngles() const { return _boxAngles; }
        [[nodiscard]] double               getTotalMass() const { return _totalMass; }
        [[nodiscard]] double               getTotalCharge() const { return _totalCharge; }
        [[nodiscard]] double               getDensity() const { return _density; }
        [[nodiscard]] double               getVolume() const { return _volume; }
        [[nodiscard]] bool                 getBoxSizeHasChanged() const { return _boxSizeHasChanged; }

        /********************
         * standard setters *
         ********************/

        void setBoxDimensions(const linearAlgebra::Vec3D &boxDimensions) { _boxDimensions = boxDimensions; }
        void setBoxAngles(const linearAlgebra::Vec3D &boxAngles) { _boxAngles = boxAngles; }
        void setDensity(const double density) { _density = density; }
        void setTotalMass(const double totalMass) { _totalMass = totalMass; }
        void setTotalCharge(const double totalCharge) { _totalCharge = totalCharge; }
        void setVolume(const double volume) { _volume = volume; }
        void setBoxSizeHasChanged(const bool boxSizeHasChanged) { _boxSizeHasChanged = boxSizeHasChanged; }
    };

}   // namespace simulationBox

#endif   // _BOX_HPP_