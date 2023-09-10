#include "simulationBox.hpp"

#include "exceptions.hpp"

#include <algorithm>   // for sort, unique
#include <format>      // for format
#include <numeric>     // for accumulate

using namespace simulationBox;

/**
 * @brief calculates the number of atoms of all molecules in the simulation box
 */
size_t SimulationBox::getNumberOfAtoms() const
{
    auto accumulateFunc = [](const size_t sum, const Molecule &molecule) { return sum + molecule.getNumberOfAtoms(); };

    return accumulate(_molecules.begin(), _molecules.end(), 0UL, accumulateFunc);
}

/**
 * @brief finds molecule by moleculeType if (size_t)
 *
 * @param moleculeType
 * @return std::optional<Molecule &>
 */
std::optional<Molecule> SimulationBox::findMolecule(const size_t moleculeType)
{
    auto isMoleculeType = [moleculeType](const Molecule &mol) { return mol.getMoltype() == moleculeType; };

    if (const auto molecule = std::ranges::find_if(_molecules, isMoleculeType); molecule != _molecules.end())
        return *molecule;
    else
        return std::nullopt;
}

/**
 * @brief find moleculeType by moleculeType if (size_t)
 *
 * @param moleculeType
 * @return Molecule
 *
 * @throw RstFileException if molecule type not found
 */
MoleculeType &SimulationBox::findMoleculeType(const size_t moleculeType)
{
    auto isMoleculeType = [moleculeType](const auto &mol) { return mol.getMoltype() == moleculeType; };

    if (const auto molecule = std::ranges::find_if(_moleculeTypes, isMoleculeType); molecule != _moleculeTypes.end())
        return *molecule;
    else
        throw customException::RstFileException(std::format("Molecule type {} not found", moleculeType));
}

/**
 * @brief checks if molecule type exists by moleculeType id (size_t)
 *
 * @param moleculeType
 * @return true
 * @return false
 */
bool SimulationBox::moleculeTypeExists(const size_t moleculeType) const
{
    auto isMoleculeType = [moleculeType](const auto &mol) { return mol.getMoltype() == moleculeType; };

    return std::ranges::find_if(_moleculeTypes, isMoleculeType) != _moleculeTypes.end();
}

/**
 * @brief find molecule type by string id
 *
 * @details return an optional - if moleculeType found it returns the moleculeType as a size_t otherwise it returns nullopt
 *
 * @param moleculeType
 * @return optional<size_t>
 */
std::optional<size_t> SimulationBox::findMoleculeTypeByString(const std::string &moleculeType) const
{
    auto isMoleculeName = [&moleculeType](const auto &mol) { return mol.getName() == moleculeType; };

    if (const auto molecule = std::ranges::find_if(_moleculeTypes, isMoleculeName); molecule != _moleculeTypes.end())
        return molecule->getMoltype();
    else
        return std::nullopt;
}

/**
 * @brief find molecule by atom index
 *
 * @details return a pair of a pointer to the molecule and the index of the atom in the molecule
 *
 * @param atomIndex
 * @return pair<Molecule *, size_t>
 */
std::pair<Molecule *, size_t> SimulationBox::findMoleculeByAtomIndex(const size_t atomIndex)
{
    size_t sum = 0;

    for (auto &molecule : _molecules)
    {
        sum += molecule.getNumberOfAtoms();

        if (sum >= atomIndex)
            return std::make_pair(&molecule, atomIndex - (sum - molecule.getNumberOfAtoms()) - 1);
    }

    throw customException::UserInputException(
        std::format("Atom index {} out of range - total number of atoms: {}", atomIndex, sum));
}

/**
 * @brief find necessary molecule types
 *
 * @details The user can specify more molecule types in the moldescriptor file than actually necessary in the simulation. This
 * function returns only the molecule types which are also present in the simulation box in _molecules.
 *
 * @return std::vector<Molecule>
 */
std::vector<Molecule> SimulationBox::findNecessaryMoleculeTypes()
{
    std::vector<Molecule> necessaryMoleculeTypes;

    auto searchMoleculeTypes = [&necessaryMoleculeTypes](const Molecule &molecule)
    {
        auto predicate = [&molecule](const auto moleculeType) { return molecule.getMoltype() == moleculeType.getMoltype(); };

        if (std::ranges::find_if(necessaryMoleculeTypes, predicate) == necessaryMoleculeTypes.end())
            necessaryMoleculeTypes.push_back(molecule);
    };

    std::ranges::for_each(_molecules, searchMoleculeTypes);

    return necessaryMoleculeTypes;
}

/**
 * @brief set partial charges of molecules from molecule types
 *
 * @throw UserInputException if molecule type not found in _moleculeTypes
 *
 */
void SimulationBox::setPartialChargesOfMoleculesFromMoleculeTypes()
{
    auto setPartialCharges = [&moleculeTypes = _moleculeTypes](Molecule &molecule)
    {
        auto predicate = [&molecule](const auto moleculeType) { return molecule.getMoltype() == moleculeType.getMoltype(); };

        if (const auto moleculeType = std::ranges::find_if(moleculeTypes, predicate); moleculeType != moleculeTypes.end())
            molecule.setPartialCharges(moleculeType->getPartialCharges());
        else
            throw customException::UserInputException(
                std::format("Molecule type {} not found in molecule types", molecule.getMoltype()));
    };

    std::ranges::for_each(_molecules, setPartialCharges);
}

/**
 * @brief resize internal global vdw types of all molecules
 *
 * TODO: remove this one should be unnecessary
 */
void SimulationBox::resizeInternalGlobalVDWTypes()
{
    auto resizeInternalGlobalVDWTypes = [](Molecule &molecule) {};

    std::ranges::for_each(_molecules, resizeInternalGlobalVDWTypes);
}

/**
 * @brief make external to internal global vdw types map
 *
 * @details the function consists of multiple steps:
 * 1) fill the external global vdw types vector with all external global vdw types from all molecules
 * 2) sort and erase duplicates
 * 3) fill the external to internal global vdw types map - internal vdw types are defined via increasing indices
 * 4) set the internal global vdw types for all molecules
 *
 */
void SimulationBox::setupExternalToInternalGlobalVdwTypesMap()
{
    /******************************************************************************************************
     * 1) fill the external global vdw types vector with all external global vdw types from all molecules *
     ******************************************************************************************************/

    auto fillExternalGlobalVdwTypes = [&externalGlobalVdwTypes = _externalGlobalVdwTypes](auto &molecule)
    {
        externalGlobalVdwTypes.insert(externalGlobalVdwTypes.end(),
                                      molecule.getExternalGlobalVDWTypes().begin(),
                                      molecule.getExternalGlobalVDWTypes().end());
    };

    std::ranges::for_each(_moleculeTypes, fillExternalGlobalVdwTypes);

    /********************************
     * 2) sort and erase duplicates *
     *******************************/

    std::ranges::sort(_externalGlobalVdwTypes);
    const auto duplicates = std::ranges::unique(_externalGlobalVdwTypes);
    _externalGlobalVdwTypes.erase(duplicates.begin(), duplicates.end());

    /*****************************************************************************************************************
     * 3) fill the external to internal global vdw types map - internal vdw types are defined via increasing indices *
     *****************************************************************************************************************/

    // c++23 with std::ranges::views::enumerate
    const size_t size = _externalGlobalVdwTypes.size();
    for (size_t i = 0; i < size; ++i)
        _externalToInternalGlobalVDWTypes.try_emplace(_externalGlobalVdwTypes[i], i);

    /**********************************************************
     * 4) set the internal global vdw types for all molecules *
     * ********************************************************/

    auto setInternalGlobalVdwTypes = [&externalToInternalGlobalVDWTypes = _externalToInternalGlobalVDWTypes](auto &molecule)
    {
        for (size_t i = 0; i < molecule.getNumberOfAtoms(); ++i)
            molecule.getAtom(i)->setInternalGlobalVDWType(
                externalToInternalGlobalVDWTypes.at(molecule.getAtom(i)->getExternalGlobalVDWType()));
    };

    std::ranges::for_each(_molecules, setInternalGlobalVdwTypes);
    // std::ranges::for_each(_moleculeTypes, setInternalGlobalVdwTypes); TODO: check if necessary
}

/**
 * @brief calculate degrees of freedom
 *
 * @TODO: at the moment only implemented for 3D periodic systems
 *
 */
void SimulationBox::calculateDegreesOfFreedom()
{
    auto accumulateFunc = [](const size_t sum, const Molecule &molecule) { return sum + molecule.getDegreesOfFreedom(); };

    _degreesOfFreedom = accumulate(_molecules.begin(), _molecules.end(), 0UL, accumulateFunc) - 3;
}

/**
 * @brief calculate center of mass of all molecules
 *
 */
void SimulationBox::calculateCenterOfMassMolecules()
{
    std::ranges::for_each(_molecules,
                          [&box = _box](Molecule &molecule) { molecule.calculateCenterOfMass(box.getBoxDimensions()); });
}

/**
 * @brief checks if the coulomb radius cut off is smaller than half of the minimal box dimension
 *
 * @throw UserInputException if coulomb radius cut off is larger than half of the minimal box dimension
 */
void SimulationBox::checkCoulombRadiusCutOff(customException::ExceptionType exceptionType) const
{
    if (getMinimalBoxDimension() < 2.0 * _coulombRadiusCutOff)
    {
        const auto *message = "Coulomb radius cut off is larger than half of the minimal box dimension";
        if (exceptionType == customException::ExceptionType::MANOSTATEXCEPTION)
            throw customException::ManostatException(message);
        else
            throw customException::UserInputException(message);
    }
}