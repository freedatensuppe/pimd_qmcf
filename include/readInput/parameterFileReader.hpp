#ifndef _PARAMETER_FILE_READER_HPP_

#define _PARAMETER_FILE_READER_HPP_

#include "engine.hpp"
// #include "parameterFileSection.hpp"

#include <string>

namespace readInput
{
    class ParameterFileReader;
    // void readParameterFile(engine::Engine &);

}   // namespace readInput

/**
 * @class ParameterReader
 *
 * @brief reads parameter file and sets settings
 *
 */
class readInput::ParameterFileReader
{
  private:
    std::string     _filename;
    std::ifstream   _fp;
    engine::Engine &_engine;

    // std::vector<readInput::TopologySection *> _topologySections;

  public:
    ParameterFileReader(const std::string &filename, engine::Engine &engine);

    bool isNeeded() const;
    // void read();
    // readInput::TopologySection *determineSection(const std::vector<std::string> &);

    void setFilename(const std::string_view &filename) { _filename = filename; }
};

#endif   // _PARAMETER_FILE_READER_HPP_