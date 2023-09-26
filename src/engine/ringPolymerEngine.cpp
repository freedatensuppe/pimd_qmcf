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

#include "ringPolymerEngine.hpp"

#include "atom.hpp"                                  // for Atom
#include "constants/internalConversionFactors.hpp"   // for _RPMD_PREFACTOR_
#include "engineOutput.hpp"                          // for EngineOutput
#include "outputFileSettings.hpp"                    // for OutputFileSettings
#include "physicalData.hpp"                          // for PhysicalData
#include "ringPolymerSettings.hpp"                   // for RingPolymerSettings
#include "thermostatSettings.hpp"                    // for ThermostatSettings
#include "timings.hpp"                               // for Timings
#include "vector3d.hpp"                              // for Vector3D, normSquared

#include <algorithm>    // for __for_each_fn
#include <functional>   // for identity
#include <stddef.h>     // for size_t

using engine::Engine;
using engine::RingPolymerEngine;

/**
 * @brief writes the ring polymer output files.
 *
 */
void RingPolymerEngine::writeOutput()
{
    Engine::writeOutput();

    if (0 == _step % settings::OutputFileSettings::getOutputFrequency())
    {
        const auto step0 = _timings.getStepCount();

        _engineOutput.writeRingPolymerRstFile(_ringPolymerBeads, step0 + _step);
        _engineOutput.writeRingPolymerXyzFile(_ringPolymerBeads);
        _engineOutput.writeRingPolymerVelFile(_ringPolymerBeads);
        _engineOutput.writeRingPolymerForceFile(_ringPolymerBeads);
        _engineOutput.writeRingPolymerChargeFile(_ringPolymerBeads);
    }
}

/**
 * @brief coupling step of ring polymers
 *
 */
void RingPolymerEngine::coupleRingPolymerBeads()
{
    const auto numberOfBeads = settings::RingPolymerSettings::getNumberOfBeads();
    const auto temperature   = settings::ThermostatSettings::getTargetTemperature();
    const auto rpmd_factor   = constants::_RPMD_PREFACTOR_ * numberOfBeads * numberOfBeads * temperature * temperature;

    std::vector<double> ringPolymerEnergy(numberOfBeads, 0.0);

    for (size_t i = 0; i < numberOfBeads; ++i)
    {
        auto &bead1 = _ringPolymerBeads[i];
        auto &bead2 = _ringPolymerBeads[(i + 1) % numberOfBeads];

        for (size_t j = 0, numberOfAtoms = bead1.getNumberOfAtoms(); j < numberOfAtoms; ++j)
        {
            auto &atom1 = bead1.getAtom(j);
            auto &atom2 = bead2.getAtom(j);

            const auto deltaPosition = atom2.getPosition() - atom1.getPosition();

            const auto forceConstant = rpmd_factor * atom1.getMass();
            const auto force         = forceConstant * deltaPosition;

            ringPolymerEnergy[i] += 0.5 * forceConstant * normSquared(deltaPosition);

            atom1.addForce(force);
            atom2.addForce(-force);
        }
    }

    _physicalData.setRingPolymerEnergy(ringPolymerEnergy);
}

/**
 * @brief combining all beads into one simulation box
 *
 * @details coords, velocities and forces are averaged over all beads
 *
 */
void RingPolymerEngine::combineBeads()
{
    const auto numberOfBeads = settings::RingPolymerSettings::getNumberOfBeads();

    std::ranges::for_each(_simulationBox.getAtoms(),
                          [](auto &atom)
                          {
                              atom->setPosition({0.0, 0.0, 0.0});
                              atom->setVelocity({0.0, 0.0, 0.0});
                              atom->setForce({0.0, 0.0, 0.0});
                          });

    auto addCoordinates = [this, numberOfBeads](auto &bead)
    {
        for (size_t i = 0; i < bead.getNumberOfAtoms(); ++i)
        {
            auto &atom = bead.getAtom(i);

            _simulationBox.getAtom(i).addPosition(atom.getPosition() / double(numberOfBeads));
            _simulationBox.getAtom(i).addVelocity(atom.getVelocity() / double(numberOfBeads));
            _simulationBox.getAtom(i).addForce(atom.getForce() / double(numberOfBeads));
        }
    };

    std::ranges::for_each(_ringPolymerBeads, addCoordinates);
}