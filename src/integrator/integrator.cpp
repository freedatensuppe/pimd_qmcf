#include "integrator.hpp"

#include "constants.hpp"

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;
using namespace simulationBox;
using namespace integrator;
using namespace config;

/**
 * @brief integrates the velocities of a single atom
 *
 * @param timestep
 * @param molecule
 * @param i
 */
void Integrator::integrateVelocities(const double timestep, Molecule &molecule, const size_t i) const
{
    auto       velocities = molecule.getAtomVelocity(i);
    const auto forces     = molecule.getAtomForce(i);
    const auto mass       = molecule.getAtomMass(i);

    velocities += timestep * forces / mass * _V_VERLET_VELOCITY_FACTOR_;

    molecule.setAtomVelocity(i, velocities);
}

/**
 * @brief integrates the positions of a single atom
 *
 * @param timestep
 * @param molecule
 * @param i
 * @param simBox
 */
void Integrator::integratePositions(const double timestep, Molecule &molecule, const size_t i, const SimulationBox &simBox) const
{
    auto       positions  = molecule.getAtomPosition(i);
    const auto velocities = molecule.getAtomVelocity(i);

    positions += timestep * velocities * _FS_TO_S_;
    applyPBC(simBox, positions);

    molecule.setAtomPositions(i, positions);
}

/**
 * @brief applies first half step of velocity verlet algorithm
 *
 * @param simBox
 */
void VelocityVerlet::firstStep(SimulationBox &simBox)
{
    const auto box = simBox.getBoxDimensions();

    for (auto &molecule : simBox.getMolecules())
    {
        const size_t numberOfAtoms = molecule.getNumberOfAtoms();

        for (size_t i = 0; i < numberOfAtoms; ++i)
        {
            integrateVelocities(_dt, molecule, i);
            integratePositions(_dt, molecule, i, simBox);
        }

        molecule.calculateCenterOfMass(box);
        molecule.setAtomForcesToZero();
    }
}

/**
 * @brief applies second half step of velocity verlet algorithm
 *
 * @param simBox
 */
void VelocityVerlet::secondStep(SimulationBox &simBox)
{
    for (auto &molecule : simBox.getMolecules())
    {
        const size_t numberOfAtoms = molecule.getNumberOfAtoms();

        for (size_t i = 0; i < numberOfAtoms; ++i)
        {
            integrateVelocities(_dt, molecule, i);
        }
    }
}