#ifndef _PHYSICAL_DATA_H_

#define _PHYSICAL_DATA_H_

#include <vector>
#include <functional>

#include "simulationBox.hpp"

/**
 * @class PhysicalData
 *
 * @brief PhysicalData is a class for output data storage
 *
 */
class PhysicalData
{
private:
    std::vector<double> _momentumVector = {0.0, 0.0, 0.0};
    double _momentum = 0.0;
    double _averageMomentum = 0.0;

    double _averageTemperature = 0.0;

    std::vector<double> _kineticEnergyAtomicVector = {0.0, 0.0, 0.0};
    std::vector<double> _kineticEnergyMolecularVector = {0.0, 0.0, 0.0};
    double _kineticEnergy = 0.0;
    double _averageKineticEnergy = 0.0;

    double _coulombEnergy = 0.0;
    double _averageCoulombEnergy = 0.0;

    double _nonCoulombEnergy = 0.0;
    double _averageNonCoulombEnergy = 0.0;

public:
    void calculateKineticEnergyAndMomentum(SimulationBox &);

    void resetAverageData();

    // standard getter and setters
    void setMomentumVector(const std::vector<double> &momentumVector) { _momentumVector = momentumVector; }

    double getMomentum() const { return _momentum; }
    double getAverageMomentum() const { return _averageMomentum; }

    void addAverageTemperature(double temperature) { _averageTemperature += temperature; }
    double getAverageTemperature() const { return _averageTemperature; }

    void setKineticEnergy(double kineticEnergy) { _kineticEnergy = kineticEnergy; }
    double getKineticEnergy() const { return _kineticEnergy; }

    void addAverageKineticEnergy(double kineticEnergy) { _averageKineticEnergy += kineticEnergy; }
    double getAverageKineticEnergy() const { return _averageKineticEnergy; }

    void setCoulombEnergy(double coulombEnergy) { _coulombEnergy = coulombEnergy; }
    double getCoulombEnergy() const { return _coulombEnergy; }

    void addAverageCoulombEnergy(double averageCoulombEnergy) { _averageCoulombEnergy += averageCoulombEnergy; }
    double getAverageCoulombEnergy() const { return _averageCoulombEnergy; }

    void setNonCoulombEnergy(double nonCoulombEnergy) { _nonCoulombEnergy = nonCoulombEnergy; }
    double getNonCoulombEnergy() const { return _nonCoulombEnergy; }

    void addAverageNonCoulombEnergy(double nonCoulombEnergy) { _averageNonCoulombEnergy += nonCoulombEnergy; }
    double getAverageNonCoulombEnergy() const { return _averageNonCoulombEnergy; }
};

#endif // _PHYSICAL_DATA_H_