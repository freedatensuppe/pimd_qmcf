#include "inputFileReader.hpp"

#include <iostream>

using namespace std;
using namespace setup;

/**
 * @brief parse timestep of simulation and set it in timings
 *
 * @param lineElements
 */
void InputFileReader::parseTimestep(const vector<string> &lineElements)
{
    checkCommand(lineElements, _lineNumber);
    _engine.getTimings().setTimestep(stod(lineElements[2]));
}

/**
 * @brief parse number of steps of simulation and set it in timings
 *
 * @param lineElements
 */
void InputFileReader::parseNumberOfSteps(const vector<string> &lineElements)
{
    checkCommand(lineElements, _lineNumber);
    _engine.getTimings().setNumberOfSteps(stoi(lineElements[2]));
}