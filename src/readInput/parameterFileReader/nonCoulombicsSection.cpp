#include "nonCoulombicsSection.hpp"

#include "buckinghamPair.hpp"         // for BuckinghamPair
#include "coulombPotential.hpp"       // for CoulombPotential
#include "engine.hpp"                 // for Engine
#include "exceptions.hpp"             // for ParameterFileException
#include "forceFieldNonCoulomb.hpp"   // for ForceFieldNonCoulomb
#include "lennardJonesPair.hpp"       // for LennardJonesPair
#include "morsePair.hpp"              // for MorsePair
#include "nonCoulombPotential.hpp"    // for NonCoulombPotential, NonCoulombType
#include "potential.hpp"              // for Potential
#include "stringUtilities.hpp"        // for toLowerCopy

#include <format>   // for format
#include <memory>   // for make_shared
#include <string>   // for stod, stoul, string, operator==

using namespace readInput::parameterFile;

/**
 * @brief processes the nonCoulombics header of the parameter file
 *
 * @note type of forceField can be given as second argument
 *       default is lj (Lennard Jones)
 *
 * @param line
 * @param engine
 *
 * @throw customException::ParameterFileException if number of elements in line is not 1 or 2
 * @throw customException::ParameterFileException if type of nonCoulombic is not lj, buckingham or morse
 */
void NonCoulombicsSection::processHeader(std::vector<std::string> &lineElements, engine::Engine &engine)
{
    auto &potential = engine.getPotential().getNonCoulombPotential();

    if (2 == lineElements.size())
    {
        const auto type = utilities::toLowerCopy(lineElements[1]);

        if (type == "lj")
            potential.setNonCoulombType(potential::NonCoulombType::LJ);
        else if (type == "buckingham")
            potential.setNonCoulombType(potential::NonCoulombType::BUCKINGHAM);
        else if (type == "morse")
            potential.setNonCoulombType(potential::NonCoulombType::MORSE);
        else
            throw customException::ParameterFileException(
                std::format("Invalid type of nonCoulombic in parameter file nonCoulombic "
                            "section at line {} - has to be lj, buckingham or morse!",
                            _lineNumber));
    }

    _nonCoulombType = potential.getNonCoulombType();
}

/**
 * @brief determines which nonCoulombic type is processed
 *
 * @param line
 * @param engine
 *
 * @throw customException::ParameterFileException if nonCoulombic type is not lj, buckingham or morse
 */
void NonCoulombicsSection::processSection(std::vector<std::string> &lineElements, engine::Engine &engine)
{
    switch (_nonCoulombType)
    {
    case potential::NonCoulombType::LJ: processLJ(lineElements, engine); break;
    case potential::NonCoulombType::BUCKINGHAM: processBuckingham(lineElements, engine); break;
    case potential::NonCoulombType::MORSE: processMorse(lineElements, engine); break;
    default:
        throw customException::ParameterFileException(std::format(
            "Wrong type of nonCoulombic in parameter file nonCoulombic section at line {}  - has to be lj, buckingham or morse!",
            _lineNumber));
    }
}

/**
 * @brief processes the LJ nonCoulombics section of the parameter file and adds the LJ pair to the nonCoulombic potential
 *
 * @details The line is expected to have the following format:
 * 1. global van der Waals type 1
 * 2. global van der Waals type 2
 * 3. c6
 * 4. c12
 * 5. cutOff (optional); if not given or -1, the global Coulomb radius cutOff is used
 *
 * @param line
 * @param engine
 *
 * @throw customException::ParameterFileException if number of elements in line is not 4 or 5
 */
void NonCoulombicsSection::processLJ(std::vector<std::string> &lineElements, engine::Engine &engine) const
{
    if (lineElements.size() != 4 && lineElements.size() != 5)
        throw customException::ParameterFileException(std::format(
            "Wrong number of arguments in parameter file in Lennard Jones nonCoulombics section at line {} - number of "
            "elements has to be 4 or 5!",
            _lineNumber));

    const auto atomType1 = stoul(lineElements[0]);
    const auto atomType2 = stoul(lineElements[1]);
    const auto c6        = stod(lineElements[2]);
    const auto c12       = stod(lineElements[3]);

    auto cutOff = 5 == lineElements.size() ? stod(lineElements[4]) : -1.0;

    cutOff = cutOff < 0.0 ? potential::CoulombPotential::getCoulombRadiusCutOff() : cutOff;

    auto &potential = dynamic_cast<potential::ForceFieldNonCoulomb &>(engine.getPotential().getNonCoulombPotential());
    potential.addNonCoulombicPair(std::make_shared<potential::LennardJonesPair>(atomType1, atomType2, cutOff, c6, c12));
}

/**
 * @brief processes the buckingham nonCoulombics section of the parameter file and adds the buckingham pair to the nonCoulombic
 * potential
 *
 * @details The line is expected to have the following format:
 * 1. global van der Waals type 1
 * 2. global van der Waals type 2
 * 3. a
 * 4. dRho
 * 5. c6
 * 6. cutOff (optional); if not given or -1, the global Coulomb radius cutOff is used
 *
 * @param line
 * @param engine
 *
 * @throw customException::ParameterFileException if number of elements in line is not 5 or 6
 */
void NonCoulombicsSection::processBuckingham(std::vector<std::string> &lineElements, engine::Engine &engine) const
{
    if (lineElements.size() != 5 && lineElements.size() != 6)
        throw customException::ParameterFileException(std::format(
            "Wrong number of arguments in parameter file in Lennard Jones nonCoulombics section at line {} - number of "
            "elements has to be 5 or 6!",
            _lineNumber));

    const auto atomType1 = stoul(lineElements[0]);
    const auto atomType2 = stoul(lineElements[1]);
    const auto a         = stod(lineElements[2]);
    const auto dRho      = stod(lineElements[3]);
    const auto c6        = stod(lineElements[4]);

    const auto cutOff = 6 == lineElements.size() ? stod(lineElements[5]) : -1.0;

    auto &potential = dynamic_cast<potential::ForceFieldNonCoulomb &>(engine.getPotential().getNonCoulombPotential());
    potential.addNonCoulombicPair(std::make_shared<potential::BuckinghamPair>(atomType1, atomType2, cutOff, a, dRho, c6));
}

/**
 * @brief processes the morse nonCoulombics section of the parameter file and adds the morse pair to the nonCoulombic
 * potential
 *
 * @details The line is expected to have the following format:
 * 1. global van der Waals type 1
 * 2. global van der Waals type 2
 * 3. dissociationEnergy
 * 4. wellWidth
 * 5. equilibriumDistance
 * 6. cutOff (optional); if not given or -1, the global Coulomb radius cutOff is used
 *
 * @param line
 * @param engine
 *
 * @throw customException::ParameterFileException if number of elements in line is not 5 or 6
 */
void NonCoulombicsSection::processMorse(std::vector<std::string> &lineElements, engine::Engine &engine) const
{
    if (lineElements.size() != 5 && lineElements.size() != 6)
        throw customException::ParameterFileException(std::format(
            "Wrong number of arguments in parameter file in Lennard Jones nonCoulombics section at line {} - number of "
            "elements has to be 5 or 6!",
            _lineNumber));

    const auto atomType1           = stoul(lineElements[0]);
    const auto atomType2           = stoul(lineElements[1]);
    const auto dissociationEnergy  = stod(lineElements[2]);
    const auto wellWidth           = stod(lineElements[3]);
    const auto equilibriumDistance = stod(lineElements[4]);

    const auto cutOff = 6 == lineElements.size() ? stod(lineElements[5]) : -1.0;

    auto &potential = dynamic_cast<potential::ForceFieldNonCoulomb &>(engine.getPotential().getNonCoulombPotential());
    potential.addNonCoulombicPair(
        std::make_shared<potential::MorsePair>(atomType1, atomType2, cutOff, dissociationEnergy, wellWidth, equilibriumDistance));
}