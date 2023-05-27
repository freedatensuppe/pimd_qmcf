#ifndef _ENGINE_H_

#define _ENGINE_H_

#include <vector>
#include <memory>

#include "settings.hpp"
#include "timings.hpp"
#include "output.hpp"
#include "physicalData.hpp"
#include "simulationBox.hpp"
#include "integrator.hpp"
#include "celllist.hpp"
#include "potential.hpp"
#include "thermostat.hpp"

/**
 * @class Engine
 *
 * @brief Contains all the information needed to run the simulation
 *
 */
class Engine
{
private:
    SimulationBox _simulationBox;

public:
    Settings _settings;
    Timings _timings;
    PhysicalData _physicalData;
    CellList _cellList;

    std::unique_ptr<Integrator> _integrator;
    std::unique_ptr<Potential> _potential = std::make_unique<PotentialBruteForce>();
    std::unique_ptr<Thermostat> _thermostat = std::make_unique<Thermostat>();

    std::unique_ptr<EnergyOutput> _energyOutput = std::make_unique<EnergyOutput>("default.en");
    std::unique_ptr<TrajectoryOutput> _xyzOutput = std::make_unique<TrajectoryOutput>("default.xyz");
    std::unique_ptr<TrajectoryOutput> _velOutput = std::make_unique<TrajectoryOutput>("default.vel");
    std::unique_ptr<TrajectoryOutput> _forceOutput = std::make_unique<TrajectoryOutput>("default.force");
    std::unique_ptr<LogOutput> _logOutput = std::make_unique<LogOutput>("default.log");
    std::unique_ptr<StdoutOutput> _stdoutOutput = std::make_unique<StdoutOutput>("stdout");
    std::unique_ptr<RstFileOutput> _rstFileOutput = std::make_unique<RstFileOutput>("default.rst");
    std::unique_ptr<ChargeOutput> _chargeOutput = std::make_unique<ChargeOutput>("default.chg");
    std::unique_ptr<InfoOutput> _infoOutput = std::make_unique<InfoOutput>("default.info");

    void run();
    void takeStep();

    // standard getter and setters
    SimulationBox &getSimulationBox() { return _simulationBox; };
};

#endif