#ifndef _SETTINGS_HPP_

#define _SETTINGS_HPP_

#include "defaults.hpp"
#include "timings.hpp"

#include <string>
#include <vector>

namespace settings
{
    class Settings;
}

/**
 * @class Settings
 *
 * @brief
 *  Stores the settings of the simulation
 *  Additionally it stores all information needed for later setup of the simulation
 *
 */
class settings::Settings
{
  private:
    // resetKineticsSettings for later setup
    size_t _nScale = 0;
    size_t _fScale = 0;
    size_t _nReset = 0;
    size_t _fReset = 0;

    // filenames and paths for later setup
    std::string _startFilename;
    std::string _moldescriptorFilename = defaults::_MOLDESCRIPTOR_FILENAME_DEFAULT_;   // for backward compatibility
    std::string _guffPath              = ".";                                          // not backward compatible
    std::string _guffDatFilename       = defaults::_GUFF_FILENAME_DEFAULT_;
    std::string _topologyFilename      = "";
    std::string _parameterFilename     = "";

    std::string _jobtype;

    // thermostat settings for later setup
    std::pair<bool, std::string> _thermostat;   // pair.first = check if thermostat was set
    std::pair<bool, double>      _temperature;
    std::pair<bool, double>      _relaxationTime = std::make_pair(false, 0.1);   // pay attention here default value in ps

    // manostat settings for later setup
    std::pair<bool, std::string> _manostat;   // pair.first = check if thermostat was set
    std::pair<bool, double>      _pressure;
    std::pair<bool, double>      _tauManostat     = std::make_pair(false, 1.0);   // pay attention here default value in ps
    double                       _compressibility = defaults::_COMPRESSIBILITY_WATER_DEFAULT_;   // 4.5e-5

    // shake settings for later setup
    double _shakeTolerance  = defaults::_SHAKE_TOLERANCE_DEFAULT_;    // 1e-8
    size_t _shakeMaxIter    = defaults::_SHAKE_MAX_ITER_DEFAULT_;     // 20
    double _rattleTolerance = defaults::_RATTLE_TOLERANCE_DEFAULT_;   // 1e-8
    size_t _rattleMaxIter   = defaults::_RATTLE_MAX_ITER_DEFAULT_;    // 20

    // coulomb long range settings for later setup
    std::string _coulombLongRangeType = defaults::_COULOMB_LONG_RANGE_TYPE_DEFAULT_;   // none
    double      _wolfParameter        = defaults::_WOLF_PARAMETER_DEFAULT_;            // 0.25

    // noncoulomb settings for later setup
    std::string _nonCoulombType = defaults::_NONCOULOMB_TYPE_DEFAULT_;   // none

  public:
    /********************
     * standard getters *
     ********************/

    [[nodiscard]] std::string getStartFilename() const { return _startFilename; }
    [[nodiscard]] std::string getMoldescriptorFilename() const { return _moldescriptorFilename; }
    [[nodiscard]] std::string getGuffPath() const { return _guffPath; }
    [[nodiscard]] std::string getGuffDatFilename() const { return _guffDatFilename; }
    [[nodiscard]] std::string getTopologyFilename() const { return _topologyFilename; }
    [[nodiscard]] std::string getParameterFilename() const { return _parameterFilename; }

    [[nodiscard]] std::string getJobtype() const { return _jobtype; }

    [[nodiscard]] std::string getThermostat() const { return _thermostat.second; }
    [[nodiscard]] bool        getThermostatSet() const { return _thermostat.first; }
    [[nodiscard]] double      getTemperature() const { return _temperature.second; }
    [[nodiscard]] bool        getTemperatureSet() const { return _temperature.first; }
    [[nodiscard]] double      getRelaxationTime() const { return _relaxationTime.second; }
    [[nodiscard]] bool        getRelaxationTimeSet() const { return _relaxationTime.first; }

    [[nodiscard]] std::string getManostat() const { return _manostat.second; }
    [[nodiscard]] bool        getManostatSet() const { return _manostat.first; }
    [[nodiscard]] double      getPressure() const { return _pressure.second; }
    [[nodiscard]] bool        getPressureSet() const { return _pressure.first; }
    [[nodiscard]] double      getTauManostat() const { return _tauManostat.second; }
    [[nodiscard]] bool        getTauManostatSet() const { return _tauManostat.first; }
    [[nodiscard]] double      getCompressibility() const { return _compressibility; }

    [[nodiscard]] size_t getNScale() const { return _nScale; }
    [[nodiscard]] size_t getFScale() const { return _fScale; }
    [[nodiscard]] size_t getNReset() const { return _nReset; }
    [[nodiscard]] size_t getFReset() const { return _fReset; }

    [[nodiscard]] double getShakeTolerance() const { return _shakeTolerance; }
    [[nodiscard]] size_t getShakeMaxIter() const { return _shakeMaxIter; }
    [[nodiscard]] double getRattleTolerance() const { return _rattleTolerance; }
    [[nodiscard]] size_t getRattleMaxIter() const { return _rattleMaxIter; }

    [[nodiscard]] std::string getCoulombLongRangeType() const { return _coulombLongRangeType; }
    [[nodiscard]] double      getWolfParameter() const { return _wolfParameter; }

    [[nodiscard]] std::string getNonCoulombType() const { return _nonCoulombType; }

    /********************
     * standard setters *
     ********************/

    void setStartFilename(const std::string_view startFilename) { _startFilename = startFilename; }
    void setMoldescriptorFilename(const std::string_view filename) { _moldescriptorFilename = filename; }
    void setGuffPath(const std::string_view guffPath) { _guffPath = guffPath; }
    void setGuffDatFilename(const std::string_view guffDatFilename) { _guffDatFilename = guffDatFilename; }
    void setTopologyFilename(const std::string_view topologyFilename) { _topologyFilename = topologyFilename; }
    void setParameterFilename(const std::string_view parameterFilename) { _parameterFilename = parameterFilename; }

    void setJobtype(const std::string_view jobtype) { _jobtype = jobtype; }

    void setThermostat(const std::string_view thermostat) { _thermostat = std::make_pair(true, thermostat); }
    void setTemperature(const double temperature);
    void setRelaxationTime(const double relaxationTime);

    void setManostat(const std::string_view manostat) { _manostat = std::make_pair(true, manostat); }
    void setPressure(const double pressure) { _pressure = std::make_pair(true, pressure); }
    void setTauManostat(const double tauManostat);
    void setCompressibility(const double compressibility) { _compressibility = compressibility; }

    void setNScale(const size_t nScale) { _nScale = nScale; }
    void setFScale(const size_t fScale) { _fScale = fScale; }
    void setNReset(const size_t nReset) { _nReset = nReset; }
    void setFReset(const size_t fReset) { _fReset = fReset; }

    void setShakeTolerance(const double shakeTolerance) { _shakeTolerance = shakeTolerance; }
    void setShakeMaxIter(const size_t shakeMaxIter) { _shakeMaxIter = shakeMaxIter; }
    void setRattleTolerance(const double rattleTolerance) { _rattleTolerance = rattleTolerance; }
    void setRattleMaxIter(const size_t rattleMaxIter) { _rattleMaxIter = rattleMaxIter; }

    void setCoulombLongRangeType(const std::string_view coulombLongRangeType) { _coulombLongRangeType = coulombLongRangeType; }
    void setWolfParameter(const double wolfParameter) { _wolfParameter = wolfParameter; }

    void setNonCoulombType(const std::string_view nonCoulombType) { _nonCoulombType = nonCoulombType; }
};

#endif   // _SETTINGS_HPP_