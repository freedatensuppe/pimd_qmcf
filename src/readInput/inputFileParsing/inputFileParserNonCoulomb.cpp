#include "inputFileParserNonCoulomb.hpp"

#include <memory>

using namespace std;
using namespace readInput;
using namespace customException;

/**
 * @brief Construct a new Input File Parser Non Coulomb Type:: Input File Parser Non Coulomb Type object
 *
 * @param engine
 */
InputFileParserNonCoulomb::InputFileParserNonCoulomb(engine::Engine &engine) : InputFileParser(engine)
{
    addKeyword(string("noncoulomb"), bind_front(&InputFileParserNonCoulomb::parseNonCoulombType, this), false);
    addKeyword(string("intra-nonBonded_file"), bind_front(&InputFileParserNonCoulomb::parseIntraNonBondedFile, this), false);
    addKeyword(string("intra-nonBonded_type"), bind_front(&InputFileParserNonCoulomb::parseIntraNonBondedType, this), false);
}

/**
 * @brief Parse the nonCoulombic type of the guff.dat file
 *
 * @details possible options are "none", "lj" and "buck"
 *
 * @param lineElements
 *
 * @throws InputFileException if invalid nonCoulomb type
 */
void InputFileParserNonCoulomb::parseNonCoulombType(const vector<string> &lineElements, const size_t lineNumber)
{
    checkCommand(lineElements, lineNumber);
    if (lineElements[2] == "none")
        _engine.getSettings().setNonCoulombType("none");
    else if (lineElements[2] == "lj")
        _engine.getSettings().setNonCoulombType("lj");
    else if (lineElements[2] == "buck")
        _engine.getSettings().setNonCoulombType("buck");
    else
        throw InputFileException(
            format("Invalid nonCoulomb type \"{}\" at line {} in input file. Possible options are: lj, buck and none",
                   lineElements[2],
                   lineNumber));
}

/**
 * @brief Parse the name of the file containing the intraNonBonded combinations
 *
 * @param lineElements
 * @param lineNumber
 */
void InputFileParserNonCoulomb::parseIntraNonBondedFile(const vector<string> &lineElements, const size_t lineNumber)
{
    checkCommand(lineElements, lineNumber);
    _engine.getSettings().setIntraNonBondedFilename(lineElements[2]);
}

/**
 * @brief Parse the type of the intraNonBonded combinations (none, guff or force-field)
 *
 * @param lineElements
 * @param lineNumber
 */
void InputFileParserNonCoulomb::parseIntraNonBondedType(const vector<string> &lineElements, const size_t lineNumber)
{
    checkCommand(lineElements, lineNumber);
    if (lineElements[2] == "none")
        _engine.getIntraNonBonded().deactivate();
    else if (lineElements[2] == "guff")
    {
        _engine.makeIntraNonBonded(intraNonBonded::IntraNonBondedForceField());
        _engine.getIntraNonBonded().activate();
    }
    else if (lineElements[2] == "force-field")
    {
        _engine.makeIntraNonBonded(intraNonBonded::IntraNonBondedForceField());
        _engine.getIntraNonBonded().activate();
    }
    else
        throw InputFileException(format(
            "Invalid intraNonBonded type \"{}\" at line {} in input file. Possible options are: guff, force-field and none",
            lineElements[2],
            lineNumber));
}