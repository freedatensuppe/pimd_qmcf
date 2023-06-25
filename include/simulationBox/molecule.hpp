#ifndef _MOLECULE_H_

#define _MOLECULE_H_

#include "vector3d.hpp"

#include <map>
#include <string>
#include <vector>

namespace simulationBox
{
    class Molecule;
}

/**
 * @class Molecule
 *
 * @brief containing all information about a molecule
 */
class simulationBox::Molecule
{
  private:
    std::string _name;
    size_t      _moltype;
    size_t      _numberOfAtoms;

    double _charge;
    double _molMass;

    std::vector<std::string> _atomNames;
    std::vector<std::string> _atomTypeNames;

    std::vector<int>         _globalVDWTypes;
    std::vector<int>         _atomicNumbers;
    std::vector<size_t>      _externalAtomTypes;
    std::vector<size_t>      _atomTypes;
    std::map<size_t, size_t> _externalToInternalAtomTypes;

    std::vector<double> _partialCharges;
    std::vector<double> _masses;

    std::vector<vector3d::Vec3D> _positions;
    std::vector<vector3d::Vec3D> _velocities;
    std::vector<vector3d::Vec3D> _forces;
    std::vector<vector3d::Vec3D> _shiftForces;

    vector3d::Vec3D _centerOfMass = vector3d::Vec3D(0.0, 0.0, 0.0);

  public:
    Molecule() = default;
    explicit Molecule(const std::string_view name) : _name(name){};
    explicit Molecule(const size_t moltype) : _moltype(moltype){};

    void calculateCenterOfMass(const vector3d::Vec3D &);
    void scale(const vector3d::Vec3D &);
    void scaleVelocities(double scaleFactor);

    size_t getNumberOfAtomTypes();
    void   resizeAtomShiftForces() { _shiftForces.resize(_forces.size()); }

    void addAtomName(const std::string &atomName) { _atomNames.push_back(atomName); }
    void addAtomTypeName(const std::string &atomTypeName) { _atomTypeNames.push_back(atomTypeName); }
    void addAtomType(const size_t atomType) { _atomTypes.push_back(atomType); }
    void addExternalAtomType(const size_t atomType) { _externalAtomTypes.push_back(atomType); }
    void addExternalToInternalAtomTypeElement(const size_t externalAtomType, size_t internalAtomType)
    {
        _externalToInternalAtomTypes.try_emplace(externalAtomType, internalAtomType);
    }

    /************************
     * standatd add methods *
     ************************/

    void addPartialCharge(const double partialCharge) { _partialCharges.push_back(partialCharge); }
    void addGlobalVDWType(const int globalVDWType) { _globalVDWTypes.push_back(globalVDWType); }
    void addAtomMass(const double mass) { _masses.push_back(mass); }
    void addAtomicNumber(const int atomicNumber) { _atomicNumbers.push_back(atomicNumber); }

    void addAtomPosition(const vector3d::Vec3D &position) { _positions.push_back(position); }
    void addAtomVelocity(const vector3d::Vec3D &velocity) { _velocities.push_back(velocity); }
    void addAtomForce(const vector3d::Vec3D &force) { _forces.push_back(force); }
    void addAtomForce(const size_t index, const vector3d::Vec3D &force) { _forces[index] += force; }
    void addAtomShiftForce(const size_t index, const vector3d::Vec3D &shiftForce) { _shiftForces[index] += shiftForce; }

    /***************************
     * standatd getter methods *
     ***************************/

    size_t getMoltype() const { return _moltype; }
    size_t getNumberOfAtoms() const { return _numberOfAtoms; }
    size_t getAtomType(const size_t index) const { return _atomTypes[index]; }
    size_t getInternalAtomType(const size_t externalAtomType) { return _externalToInternalAtomTypes[externalAtomType]; }
    size_t getExternalAtomType(const size_t index) const { return _externalAtomTypes[index]; }

    int getGlobalVDWType(const size_t index) const { return _globalVDWTypes[index]; }
    int getAtomicNumber(const size_t index) const { return _atomicNumbers[index]; }

    double getCharge() const { return _charge; }
    double getMolMass() const { return _molMass; }
    double getPartialCharge(const size_t index) const { return _partialCharges[index]; }
    double getAtomMass(const size_t index) const { return _masses[index]; }

    std::string getName() const { return _name; }
    std::string getAtomName(const size_t index) const { return _atomNames[index]; }
    std::string getAtomTypeName(const size_t index) const { return _atomTypeNames[index]; }

    vector3d::Vec3D getAtomPosition(const size_t index) const { return _positions[index]; }
    vector3d::Vec3D getAtomVelocity(const size_t index) const { return _velocities[index]; }
    vector3d::Vec3D getAtomForce(const size_t index) const { return _forces[index]; }
    vector3d::Vec3D getAtomShiftForce(const size_t index) const { return _shiftForces[index]; }
    vector3d::Vec3D getCenterOfMass() const { return _centerOfMass; }

    std::vector<size_t> getExternalAtomTypes() const { return _externalAtomTypes; }

    /***************************
     * standatd setter methods *
     ***************************/

    void setName(const std::string_view name) { _name = name; }
    void setMoltype(const size_t moltype) { _moltype = moltype; }
    void setCharge(const double charge) { _charge = charge; }
    void setMolMass(const double molMass) { _molMass = molMass; }
    void setNumberOfAtoms(const size_t numberOfAtoms) { _numberOfAtoms = numberOfAtoms; }

    void setAtomPositions(const size_t index, const vector3d::Vec3D &position) { _positions[index] = position; }
    void setAtomVelocity(const size_t index, const vector3d::Vec3D &velocity) { _velocities[index] = velocity; }
    void setAtomForce(const size_t index, const vector3d::Vec3D &force) { _forces[index] = force; }
    void setAtomShiftForces(const size_t index, const vector3d::Vec3D &shiftForce) { _shiftForces[index] = shiftForce; }
    void setAtomForcesToZero() { std::fill(_forces.begin(), _forces.end(), vector3d::Vec3D(0.0, 0.0, 0.0)); }
};

#endif
