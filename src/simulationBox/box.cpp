#include "box.hpp"

#include "constants.hpp"
#include "exceptions.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;
using namespace simulationBox;

/**
 * @brief Set the Box Dimensions in Box object
 *
 * @param boxDimensions
 *
 * @throw RstFileException if any of the dimensions is negative
 */
void Box::setBoxDimensions(const Vec3D &boxDimensions)
{
    for (auto &dimension : boxDimensions)
        if (dimension < 0.0) throw RstFileException("Box dimensions must be positive - dimension = " + to_string(dimension));

    _boxDimensions = boxDimensions;
}

/**
 * @brief Set the Box Angles in Box object
 *
 * @param boxAngles
 *
 * @throw RstFileException if any of the angles is negative or greater than 90°
 */
void Box::setBoxAngles(const Vec3D &boxAngles)
{
    for (auto &angle : boxAngles)
        if ((angle < 0.0) || (angle > 90.0))
            throw RstFileException("Box angles must be positive and smaller than 90° - angle = " + to_string(angle));

    _boxAngles = boxAngles;
}

/**
 * @brief Set the Density in Box object
 *
 * @param density
 *
 * @throw InputFileException if density is negative
 */
void Box::setDensity(const double density)
{
    if (density < 0.0) throw InputFileException("Density must be positive - density = " + to_string(density));

    _density = density;
}

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

    const double cos_alpha = cos(_boxAngles[0] * M_PI / 180.0);
    const double cos_beta  = cos(_boxAngles[1] * M_PI / 180.0);
    const double cos_gamma = cos(_boxAngles[2] * M_PI / 180.0);

    _volume *=
        sqrt(1.0 - cos_alpha * cos_alpha - cos_beta * cos_beta - cos_gamma * cos_gamma + 2.0 * cos_alpha * cos_beta * cos_gamma);

    return _volume;
}

/**
 * @brief Calculate the box dimensions from the density
 *
 * @return vector<double>
 */
Vec3D Box::calculateBoxDimensionsFromDensity() const
{
    const double volume     = _totalMass / (_density * _KG_PER_LITER_TO_AMU_PER_ANGSTROM_CUBIC_);
    const double cellLenght = cbrt(volume);

    return Vec3D(cellLenght, cellLenght, cellLenght);
}

/**
 * @brief calculates the distance between two atoms
 *
 * @param point1
 * @param point2
 * @param dxyz
 * @return double distance
 */
double Box::calculateDistance(const Vec3D &point1, const Vec3D &point2, Vec3D &dxyz)
{
    dxyz = point1 - point2;

    applyPBC(dxyz);

    return norm(dxyz);
}

/**
 * @brief calculates the distance squared between two atoms
 *
 * @param point1
 * @param point2
 * @param dxyz
 * @return double
 */
double Box::calculateDistanceSquared(const Vec3D &point1, const Vec3D &point2, Vec3D &dxyz)
{
    dxyz = point1 - point2;

    applyPBC(dxyz);

    return normSquared(dxyz);
}

/**
 * @brief applies the periodic boundary conditions
 *
 * @param dxyz
 */
void Box::applyPBC(Vec3D &dxyz) const { dxyz -= _boxDimensions * round(dxyz / _boxDimensions); }

/**
 * @brief scales the cell dimensions
 *
 * @param scaleFactors
 */
void Box::scaleBox(const Vec3D &scaleFactors)
{
    _boxDimensions *= scaleFactors;
    calculateVolume();
}