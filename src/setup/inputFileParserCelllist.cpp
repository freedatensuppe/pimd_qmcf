#include "exceptions.hpp"
#include "inputFileReader.hpp"

using namespace std;
using namespace setup;

void InputFileReader::parseCellListActivated(const vector<string> &lineElements)
{
    checkCommand(lineElements, _lineNumber);
    if (lineElements[2] == "on")
        _engine.getCellList().activate();
    else if (lineElements[2] == "off")
    {
        // default
    }
    else
    {
        auto message =
            "Invalid cell-list keyword \"" + lineElements[2] + "\" at line " + to_string(_lineNumber) + "in input file\n";
        message += R"(Possible keywords are "on" and "off")";
        throw InputFileException(message);
    }
}

void InputFileReader::parseNumberOfCells(const vector<string> &lineElements)
{
    checkCommand(lineElements, _lineNumber);
    _engine.getCellList().setNumberOfCells(stoi(lineElements[2]));
}