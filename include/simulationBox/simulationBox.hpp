#ifndef _SIMULATION_BOX_H_

#define _SIMULATION_BOX_H_

#include "box.hpp"
#include "defaults.hpp"
#include "molecule.hpp"

#include <string>
#include <vector>

using c_ul = const size_t;

namespace simulationBox {
    class SimulationBox;
}

/**
 * @class SimulationBox
 *
 * @brief
 *
 *  contains all particles and the simulation box
 *
 * @details
 *
 *  The SimulationBox class contains all particles and the simulation box.
 *  The atoms positions, velocities and forces are stored in the SimulationBox class.
 *  Additional molecular information is also stored in the SimulationBox class.
 *
 */
class simulationBox::SimulationBox {
  private:
    int    _waterType;
    int    _ammoniaType;
    int    _degreesOfFreedom = 0;
    double _rcCutOff         = _COULOMB_CUT_OFF_DEFAULT_;

    Box _box;

    std::vector<Molecule> _molecules;
    std::vector<Molecule> _moleculeTypes;

    std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> _guffCoefficients;
    std::vector<std::vector<std::vector<std::vector<double>>>>              _rncCutOffs;
    std::vector<std::vector<std::vector<std::vector<double>>>>              _coulombCoefficients;
    std::vector<std::vector<std::vector<std::vector<double>>>>              _cEnergyCutOffs;
    std::vector<std::vector<std::vector<std::vector<double>>>>              _cForceCutOffs;
    std::vector<std::vector<std::vector<std::vector<double>>>>              _ncEnergyCutOffs;
    std::vector<std::vector<std::vector<std::vector<double>>>>              _ncForceCutOffs;

  public:
    void addMolecule(const Molecule &molecule) { _molecules.push_back(molecule); };

    [[nodiscard]] size_t   getNumberOfAtoms() const;
    [[nodiscard]] int      getDegreesOfFreedom() const { return _degreesOfFreedom; };
    [[nodiscard]] Molecule findMoleculeType(const size_t moltype) const;

    void calculateDegreesOfFreedom();
    void calculateCenterOfMassMolecules();

    void resizeGuff(c_ul numberOfMoleculeTypes);
    void resizeGuff(c_ul m1, c_ul numberOfMoleulceTypes);
    void resizeGuff(c_ul m1, c_ul m2, c_ul numberOfAtoms);
    void resizeGuff(c_ul m1, c_ul m2, c_ul a1, c_ul numberOfAtoms);

    /***************************
     * standatd getter methods *
     ***************************/

    [[nodiscard]] int    getWaterType() const { return _waterType; };
    [[nodiscard]] int    getAmmoniaType() const { return _ammoniaType; };
    [[nodiscard]] double getRcCutOff() const { return _rcCutOff; };

    [[nodiscard]] size_t getNumberOfMolecules() const { return _molecules.size(); };

    [[nodiscard]] std::vector<Molecule> &getMolecules() { return _molecules; };
    [[nodiscard]] Molecule              &getMolecule(const size_t moleculeIndex) { return _molecules[moleculeIndex]; };
    [[nodiscard]] std::vector<Molecule> &getMoleculeTypes() { return _moleculeTypes; };
    [[nodiscard]] Molecule              &getMoleculeType(const size_t moleculeTypeIndex) { return _moleculeTypes[moleculeTypeIndex]; };

    [[nodiscard]] std::vector<double> &getGuffCoefficients(c_ul m1, c_ul m2, c_ul a1, c_ul a2) { return _guffCoefficients[m1 - 1][m2 - 1][a1][a2]; };

    [[nodiscard]] double getRncCutOff(c_ul m1, c_ul m2, c_ul a1, c_ul a2) { return _rncCutOffs[m1 - 1][m2 - 1][a1][a2]; };
    [[nodiscard]] double getCoulombCoefficient(c_ul m1, c_ul m2, c_ul a1, c_ul a2) { return _coulombCoefficients[m1 - 1][m2 - 1][a1][a2]; };
    [[nodiscard]] double getcEnergyCutOff(c_ul m1, c_ul m2, c_ul a1, c_ul a2) { return _cEnergyCutOffs[m1 - 1][m2 - 1][a1][a2]; };
    [[nodiscard]] double getcForceCutOff(c_ul m1, c_ul m2, c_ul a1, c_ul a2) { return _cForceCutOffs[m1 - 1][m2 - 1][a1][a2]; };
    [[nodiscard]] double getncEnergyCutOff(c_ul m1, c_ul m2, c_ul a1, c_ul a2) { return _ncEnergyCutOffs[m1 - 1][m2 - 1][a1][a2]; };
    [[nodiscard]] double getncForceCutOff(c_ul m1, c_ul m2, c_ul a1, c_ul a2) { return _ncForceCutOffs[m1 - 1][m2 - 1][a1][a2]; };

    /***************************
     * standatd setter methods *
     ***************************/

    void setWaterType(const int waterType) { _waterType = waterType; };
    void setAmmoniaType(const int ammoniaType) { _ammoniaType = ammoniaType; };
    void setRcCutOff(const double rcCutOff) { _rcCutOff = rcCutOff; };

    void setGuffCoefficients(c_ul m1, c_ul m2, c_ul a1, c_ul a2, const std::vector<double> &guffCoefficients) { _guffCoefficients[m1 - 1][m2 - 1][a1][a2] = guffCoefficients; }
    void setRncCutOff(c_ul m1, c_ul m2, c_ul a1, c_ul a2, const double rncCutOff) { _rncCutOffs[m1 - 1][m2 - 1][a1][a2] = rncCutOff; }
    void setCoulombCoefficient(c_ul m1, c_ul m2, c_ul a1, c_ul a2, const double coulombCoefficient) { _coulombCoefficients[m1 - 1][m2 - 1][a1][a2] = coulombCoefficient; }
    void setcEnergyCutOff(c_ul m1, c_ul m2, c_ul a1, c_ul a2, const double cEnergyCutOff) { _cEnergyCutOffs[m1 - 1][m2 - 1][a1][a2] = cEnergyCutOff; }
    void setcForceCutOff(c_ul m1, c_ul m2, c_ul a1, c_ul a2, const double cForceCutOff) { _cForceCutOffs[m1 - 1][m2 - 1][a1][a2] = cForceCutOff; }
    void setncEnergyCutOff(c_ul m1, c_ul m2, c_ul a1, c_ul a2, const double ncEnergyCutOff) { _ncEnergyCutOffs[m1 - 1][m2 - 1][a1][a2] = ncEnergyCutOff; }
    void setncForceCutOff(c_ul m1, c_ul m2, c_ul a1, c_ul a2, const double ncForceCutOff) { _ncForceCutOffs[m1 - 1][m2 - 1][a1][a2] = ncForceCutOff; }

    /**********************************************
     * Forwards the box methods to the box object *
     **********************************************/

    void applyPBC(Vec3D &position) const { _box.applyPBC(position); };
    void scaleBox(const Vec3D &scaleFactors) { _box.scaleBox(scaleFactors); };

    [[nodiscard]] double calculateVolume() { return _box.calculateVolume(); }
    [[nodiscard]] Vec3D  calculateBoxDimensionsFromDensity() const { return _box.calculateBoxDimensionsFromDensity(); }

    [[nodiscard]] double getMinimalBoxDimension() const { return _box.getMinimalBoxDimension(); }
    [[nodiscard]] bool   getBoxSizeHasChanged() const { return _box.getBoxSizeHasChanged(); }

    [[nodiscard]] double getDensity() const { return _box.getDensity(); }
    [[nodiscard]] double getTotalMass() const { return _box.getTotalMass(); }
    [[nodiscard]] double getTotalCharge() const { return _box.getTotalCharge(); }
    [[nodiscard]] double getVolume() const { return _box.getVolume(); }
    [[nodiscard]] Vec3D  getBoxDimensions() const { return _box.getBoxDimensions(); }
    [[nodiscard]] Vec3D  getBoxAngles() const { return _box.getBoxAngles(); }

    void setDensity(const double density) { _box.setDensity(density); }
    void setTotalMass(const double mass) { _box.setTotalMass(mass); }
    void setTotalCharge(const double charge) { _box.setTotalCharge(charge); }
    void setVolume(const double volume) { _box.setVolume(volume); }
    void setBoxDimensions(const Vec3D &boxDimensions) { _box.setBoxDimensions(boxDimensions); }
    void setBoxAngles(const Vec3D &boxAngles) { _box.setBoxAngles(boxAngles); }
};

#endif