#include "exceptions.hpp"
#include "inputFileParser.hpp"

using namespace std;
using namespace readInput;
using namespace customException;

/**
 * @brief Construct a new Input File Parser Simulation Box:: Input File Parser Simulation Box object
 *
 * @param engine
 */
InputFileParserSimulationBox::InputFileParserSimulationBox(engine::Engine &engine) : InputFileParser(engine)
{
    addKeyword(string("rcoulomb"), bind_front(&InputFileParserSimulationBox::parseCoulombRadius, this), false);
    addKeyword(string("density"), bind_front(&InputFileParserSimulationBox::parseDensity, this), false);
}

/**
 * @brief parses the coulomb cutoff radius
 *
 * @param lineElements
 */
void InputFileParserSimulationBox::parseCoulombRadius(const vector<string> &lineElements, const size_t lineNumber)
{
    checkCommand(lineElements, lineNumber);

    const auto cutOff = stod(lineElements[2]);
    if (cutOff < 0.0)
        throw InputFileException("Coulomb radius cutoff must be positive - \"" + lineElements[2] + "\" at line " +
                                 to_string(lineNumber) + "in input file");

    _engine.getSimulationBox().setCoulombRadiusCutOff(cutOff);
}

/**
 * @brief parse density of simulation and set it in simulation box
 *
 * @param lineElements
 */
void InputFileParserSimulationBox::parseDensity(const vector<string> &lineElements, const size_t lineNumber)
{
    checkCommand(lineElements, lineNumber);
    _engine.getSimulationBox().setDensity(stod(lineElements[2]));
}