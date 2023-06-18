#ifndef _MOLDESCRIPTOR_READER_H_

#define _MOLDESCRIPTOR_READER_H_

#include "engine.hpp"

#include <fstream>
#include <string>
#include <vector>

namespace setup
{
    class MoldescriptorReader;
    void readMolDescriptor(Engine &);
}   // namespace setup

/**
 * @class MoldescriptorReader
 *
 * @brief Reads a moldescriptor file
 *
 */
class setup::MoldescriptorReader
{
  private:
    int               _lineNumber;
    const std::string _filename;
    std::ifstream     _fp;

    Engine &_engine;

  public:
    explicit MoldescriptorReader(Engine &engine);

    void read();
    void processMolecule(std::vector<std::string> &);
    void convertExternalToInternalAtomtypes(simulationBox::Molecule &) const;
};

#endif