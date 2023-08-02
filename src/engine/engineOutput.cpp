#include "engineOutput.hpp"

using namespace engine;

/**
 * @brief wrapper for energy file output function
 *
 * @param step
 * @param physicalData
 */
void EngineOutput::writeEnergyFile(const size_t step, const physicalData::PhysicalData &physicalData)
{
    _energyOutput->write(step, physicalData);
}

/**
 * @brief wrapper for xyz file output function
 *
 * @param simulationBox
 */
void EngineOutput::writeXyzFile(simulationBox::SimulationBox &simulationBox) { _xyzOutput->writeXyz(simulationBox); }

/**
 * @brief wrapper for velocity file output function
 *
 * @param simulationBox
 */
void EngineOutput::writeVelFile(simulationBox::SimulationBox &simulationBox) { _velOutput->writeVelocities(simulationBox); }

/**
 * @brief wrapper for force file output function
 *
 * @param simulationBox
 */
void EngineOutput::writeForceFile(simulationBox::SimulationBox &simulationBox) { _forceOutput->writeForces(simulationBox); }

/**
 * @brief wrapper for charge file output function
 *
 * @param simulationBox
 */
void EngineOutput::writeChargeFile(simulationBox::SimulationBox &simulationBox) { _chargeOutput->writeCharges(simulationBox); }

/**
 * @brief wrapper for info file output function
 *
 * @param time
 * @param physicalData
 */
void EngineOutput::writeInfoFile(const double time, const physicalData::PhysicalData &physicalData)
{
    _infoOutput->write(time, physicalData);
}

/**
 * @brief wrapper for restart file output function
 *
 * @param simulationBox
 * @param step
 */
void EngineOutput::writeRstFile(simulationBox::SimulationBox &simulationBox, const size_t step)
{
    _rstFileOutput->write(simulationBox, step);
}