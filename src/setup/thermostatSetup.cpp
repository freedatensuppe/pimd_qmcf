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

#include "thermostatSetup.hpp"

#include "berendsenThermostat.hpp"           // for BerendsenThermostat
#include "constants/conversionFactors.hpp"   // for _PS_TO_FS_, _PER_CM_TO_HZ_
#include "engine.hpp"                        // for Engine
#include "exceptions.hpp"                    // for InputFileException
#include "langevinThermostat.hpp"            // for LangevinThermostat
#include "noseHooverThermostat.hpp"          // for NoseHooverThermostat
#include "thermostat.hpp"                    // for Thermostat
#include "thermostatSettings.hpp"            // for ThermostatSettings, ThermostatType
#include "velocityRescalingThermostat.hpp"   // for VelocityRescalingThermostat

#include <algorithm>    // for __for_each_fn, for_each
#include <cstddef>      // for size_t
#include <format>       // for format
#include <functional>   // for identity
#include <map>          // for map, operator==
#include <string>       // for string
#include <vector>       // for vector

using namespace setup;

/**
 * @brief wrapper for thermostat setup
 *
 * @details constructs a thermostat setup object and calls the setup function
 *
 * @param engine
 */
void setup::setupThermostat(engine::Engine &engine)
{
    engine.getStdoutOutput().writeSetup("thermostat");
    engine.getLogOutput().writeSetup("thermostat");

    ThermostatSetup thermostatSetup(engine);
    thermostatSetup.setup();
}

/**
 * @brief setup thermostat
 *
 * @details checks if a thermostat was set in the input file,
 * If a thermostat was selected than the user has to provide a target temperature for the thermostat.
 *
 * @note the base class Thermostat does not apply any temperature coupling to the system and therefore it represents the none
 * thermostat.
 *
 * @throws InputFileException if no temperature was set for the thermostat
 *
 */
void ThermostatSetup::setup()
{
    const auto thermostatType = settings::ThermostatSettings::getThermostatType();

    if (thermostatType != settings::ThermostatType::NONE)
        if (!settings::ThermostatSettings::isTemperatureSet())
            throw customException::InputFileException(
                std::format("Temperature not set for {} thermostat", settings::string(thermostatType)));

    if (thermostatType == settings::ThermostatType::BERENDSEN)
        _engine.makeThermostat(
            thermostat::BerendsenThermostat(settings::ThermostatSettings::getTargetTemperature(),
                                            settings::ThermostatSettings::getRelaxationTime() * constants::_PS_TO_FS_));

    else if (thermostatType == settings::ThermostatType::VELOCITY_RESCALING)
        _engine.makeThermostat(
            thermostat::VelocityRescalingThermostat(settings::ThermostatSettings::getTargetTemperature(),
                                                    settings::ThermostatSettings::getRelaxationTime() * constants::_PS_TO_FS_));

    else if (thermostatType == settings::ThermostatType::LANGEVIN)
        _engine.makeThermostat(thermostat::LangevinThermostat(settings::ThermostatSettings::getTargetTemperature(),
                                                              settings::ThermostatSettings::getFriction()));

    else if (thermostatType == settings::ThermostatType::NOSE_HOOVER)
    {
        const auto noseHooverChainLength = settings::ThermostatSettings::getNoseHooverChainLength();
        const auto noseHooverCouplingFrequency =
            settings::ThermostatSettings::getNoseHooverCouplingFrequency() * constants::_PER_CM_TO_HZ_;

        auto thermostat = thermostat::NoseHooverThermostat(settings::ThermostatSettings::getTargetTemperature(),
                                                           std::vector<double>(noseHooverChainLength + 1, 0.0),
                                                           std::vector<double>(noseHooverChainLength + 1, 0.0),
                                                           noseHooverCouplingFrequency);

        auto fillChi = [&thermostat, noseHooverChainLength](const auto pair)
        {
            if (pair.first > noseHooverChainLength)
                throw customException::InputFileException(std::format(
                    "Chi index {} is larger than the number of nose hoover chains {}", pair.first, noseHooverChainLength));

            thermostat.setChi(size_t(pair.first - 1), pair.second);
        };

        auto fillZeta = [&thermostat](const auto pair) { thermostat.setZeta(size_t(pair.first - 1), pair.second); };

        std::ranges::for_each(settings::ThermostatSettings::getChi(), fillChi);
        std::ranges::for_each(settings::ThermostatSettings::getZeta(), fillZeta);

        _engine.makeThermostat(thermostat);
    }
    else
        _engine.makeThermostat(thermostat::Thermostat());
}