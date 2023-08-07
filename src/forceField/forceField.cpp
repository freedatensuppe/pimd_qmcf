#include "forceField.hpp"

#include "exceptions.hpp"

#include <algorithm>

using namespace forceField;
using namespace std;

/**
 * @brief find bond type by id
 *
 * @param id
 * @return const BondType&
 *
 * @throws customException::TopologyException if bond type with id not found
 */
const BondType &ForceField::findBondTypeById(const size_t id) const
{
    auto isBondId = [id](const BondType &bondType) { return bondType.getId() == id; };

    if (const auto bondType = ranges::find_if(_bondTypes, isBondId); bondType != _bondTypes.end())
        return *bondType;
    else
        throw customException::TopologyException("Bond type with id " + to_string(id) + " not found.");
}

/**
 * @brief find angle type by id
 *
 * @param id
 * @return const AngleType&
 *
 * @throws customException::TopologyException if angle type with id not found
 */
const AngleType &ForceField::findAngleTypeById(const size_t id) const
{
    auto isAngleId = [id](const AngleType &angleType) { return angleType.getId() == id; };

    if (const auto angleType = ranges::find_if(_angleTypes, isAngleId); angleType != _angleTypes.end())
        return *angleType;
    else
        throw customException::TopologyException("Angle type with id " + to_string(id) + " not found.");
}

/**
 * @brief find dihedral type by id
 *
 * @param id
 * @return const DihedralType&
 *
 * @throws customException::TopologyException if dihedral type with id not found
 */
const DihedralType &ForceField::findDihedralTypeById(const size_t id) const
{
    auto isDihedralId = [id](const DihedralType &dihedralType) { return dihedralType.getId() == id; };

    if (const auto dihedralType = ranges::find_if(_dihedralTypes, isDihedralId); dihedralType != _dihedralTypes.end())
        return *dihedralType;
    else
        throw customException::TopologyException("Dihedral type with id " + to_string(id) + " not found.");
}

/**
 * @brief find improper dihedral type by id
 *
 * @param id
 * @return const DihedralType&
 *
 * @throws customException::TopologyException if improper dihedral type with id not found
 */
const DihedralType &ForceField::findImproperDihedralTypeById(const size_t id) const
{
    auto isImproperDihedralId = [id](const DihedralType &dihedralType) { return dihedralType.getId() == id; };

    if (const auto dihedralType = ranges::find_if(_improperDihedralTypes, isImproperDihedralId);
        dihedralType != _improperDihedralTypes.end())
        return *dihedralType;
    else
        throw customException::TopologyException("Improper dihedral type with id " + to_string(id) + " not found.");
}

/**
 * @brief delete all non-coulombic pairs that are not needed
 *
 * @details This function is used to delete all non-coulombic pairs that are not needed. This is the case if the
 *         non-coulombic pair contains a van der Waals type that is not used in the simulation box.
 *
 * @param externalGlobalVanDerWaalTypes
 */
void ForceField::deleteNotNeededNonCoulombicPairs(const vector<size_t> &externalGlobalVanDerWaalTypes)
{
    auto isNotNeededNonCoulombicPair = [&externalGlobalVanDerWaalTypes](const auto &nonCoulombicPair)
    {
        return ranges::find(externalGlobalVanDerWaalTypes, nonCoulombicPair->getVanDerWaalsType1()) ==
                   externalGlobalVanDerWaalTypes.end() ||
               ranges::find(externalGlobalVanDerWaalTypes, nonCoulombicPair->getVanDerWaalsType2()) ==
                   externalGlobalVanDerWaalTypes.end();
    };

    const auto ret = ranges::remove_if(_nonCoulombicPairsVector, isNotNeededNonCoulombicPair);

    _nonCoulombicPairsVector.erase(ret.begin(), ret.end());
}

/**
 * @brief determines internal global van der Waals types and sets them in the NonCoulombicPair objects
 *
 * @param _externalToInternalGlobalVDWTypes
 */
void ForceField::determineInternalGlobalVdwTypes(const map<size_t, size_t> &externalToInternalGlobalVDWTypes)
{
    auto setInternalVanDerWaalsType = [&externalToInternalGlobalVDWTypes](auto &nonCoulombicPair)
    {
        nonCoulombicPair->setInternalType1(externalToInternalGlobalVDWTypes.at(nonCoulombicPair->getVanDerWaalsType1()));
        nonCoulombicPair->setInternalType2(externalToInternalGlobalVDWTypes.at(nonCoulombicPair->getVanDerWaalsType2()));
    };

    ranges::for_each(_nonCoulombicPairsVector, setInternalVanDerWaalsType);
}

/**
 * @brief finds all non-coulombic pairs that are self-interaction pairs
 *
 * @details self interacting means that both internal or external types are the same
 *
 * @return vector<unique_ptr<NonCoulombicPair>>
 */
vector<unique_ptr<NonCoulombicPair>> ForceField::getSelfInteractionNonCoulombicPairs() const
{
    vector<unique_ptr<NonCoulombicPair>> selfInteractionNonCoulombicPairs;

    auto addSelfInteractionPairs = [&selfInteractionNonCoulombicPairs](const auto &nonCoulombicPair)
    {
        if (nonCoulombicPair->getInternalType1() == nonCoulombicPair->getInternalType2())
            selfInteractionNonCoulombicPairs.push_back(make_unique<NonCoulombicPair>(*nonCoulombicPair));
    };

    ranges::for_each(_nonCoulombicPairsVector, addSelfInteractionPairs);

    return selfInteractionNonCoulombicPairs;
}

/**
 * @brief fills the diagonal elements of the non-coulombic pairs matrix
 *
 * @param diagonalElements
 */
void ForceField::fillDiagonalElementsOfNonCoulombicPairsMatrix(vector<unique_ptr<NonCoulombicPair>> &diagonalElements)
{
    ranges::sort(diagonalElements,
                 [](const auto &nonCoulombicPair1, const auto &nonCoulombicPair2)
                 { return nonCoulombicPair1->getInternalType1() < nonCoulombicPair2->getInternalType1(); });

    auto numberOfDiagonalElements = diagonalElements.size();
    initNonCoulombicPairsMatrix(numberOfDiagonalElements);

    for (size_t i = 0; i < numberOfDiagonalElements; ++i)
        _nonCoulombicPairsMatrix(i, i) = move(diagonalElements[i]);
}

// TODO:
void ForceField::fillNonDiagonalElementsOfNonCoulombicPairsMatrix()
{
    const auto &[rows, cols] = _nonCoulombicPairsMatrix.shape();

    for (size_t i = 0; i < rows; ++i)
        for (size_t j = i + 1; j < cols; ++j)
        {
            auto nonCoulombicPair1 = findNonCoulombicPairByInternalTypes(i, j);
            auto nonCoulombicPair2 = findNonCoulombicPairByInternalTypes(j, i);

            if (nonCoulombicPair1 == nullopt && nonCoulombicPair2 == nullopt)
            {
                if (_mixingRule == MixingRule::NONE)
                {
                    throw customException::ParameterFileException(
                        "Not all combinations of global van der Waals types are defined in the parameter file - and no mixing "
                        "rules were chosen");
                }
            }
            else if (nonCoulombicPair1 != nullopt && nonCoulombicPair2 != nullopt)
            {
                if (**nonCoulombicPair1 != **nonCoulombicPair2)
                {
                    const auto vdwType1 = (*nonCoulombicPair1)->getVanDerWaalsType1();
                    const auto vdwType2 = (*nonCoulombicPair1)->getVanDerWaalsType2();
                    throw customException::ParameterFileException("Non-coulombic pairs with global van der Waals types " +
                                                                  to_string(vdwType1) + ", " + to_string(vdwType2) + " and " +
                                                                  to_string(vdwType1) + ", " + to_string(vdwType2) +
                                                                  " in the parameter file have different parameters");
                }
            }
            else if (nonCoulombicPair1 != nullopt)
            {
                _nonCoulombicPairsMatrix(i, j) = move(*nonCoulombicPair1);
                _nonCoulombicPairsMatrix(j, i) = move(*nonCoulombicPair1);
            }
            else
            {
                _nonCoulombicPairsMatrix(i, j) = move(*nonCoulombicPair2);
                _nonCoulombicPairsMatrix(j, i) = move(*nonCoulombicPair2);
            }
        }
}

/**
 * @brief finds a non coulombic pair by internal types
 *
 * @details if the non coulombic pair is not found, an empty optional is returned
 *          if the non coulombic pair is found twice an exception is thrown
 *
 * @param internalType1
 * @param internalType2
 * @return optional<unique_ptr<NonCoulombicPair>>
 *
 * @throws if the non coulombic pair is found twice
 */
optional<unique_ptr<NonCoulombicPair>> ForceField::findNonCoulombicPairByInternalTypes(size_t internalType1,
                                                                                       size_t internalType2) const
{
    auto findByInternalAtomTypes = [internalType1, internalType2](const auto &nonCoulombicPair)
    { return nonCoulombicPair->getInternalType1() == internalType1 && nonCoulombicPair->getInternalType2() == internalType2; };

    if (auto firstNonCoulombicPair = ranges::find_if(_nonCoulombicPairsVector, findByInternalAtomTypes);
        firstNonCoulombicPair != _nonCoulombicPairsVector.end())
    {
        if (auto secondCoulombicPair =
                ranges::find_if(firstNonCoulombicPair + 1, _nonCoulombicPairsVector.end(), findByInternalAtomTypes);
            secondCoulombicPair != _nonCoulombicPairsVector.end())
        {
            auto vdwType1 = (*firstNonCoulombicPair)->getVanDerWaalsType1();
            auto vdwType2 = (*firstNonCoulombicPair)->getVanDerWaalsType2();
            throw customException::ParameterFileException("Non coulombic pair with global van der waals types " +
                                                          to_string(vdwType1) + " and " + to_string(vdwType2) +
                                                          " is defined twice in the parameter file.");
        }
        else
        {
            return make_unique<NonCoulombicPair>(**firstNonCoulombicPair);
        }
    }
    else
        return nullopt;
}
