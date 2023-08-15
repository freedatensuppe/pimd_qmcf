#include "potentialSetup.hpp"

#include "coulombShiftedPotential.hpp"
#include "coulombWolf.hpp"
#include "forceFieldNonCoulomb.hpp"
#include "guffNonCoulomb.hpp"

#include <ranges>

using namespace std;
using namespace setup;
using namespace potential;

/**
 * @brief wrapper for setup potential
 *
 */
void setup::setupPotential(engine::Engine &engine)
{
    PotentialSetup potentialSetup(engine);
    potentialSetup.setup();
}

/**
 * @brief sets all nonBonded potential types
 *
 */
void PotentialSetup::setup()
{
    setupCoulomb();
    setupNonCoulomb();

    if (!_engine.isForceFieldNonCoulombicsActivated())
        return;

    setupNonCoulombicPairs();
}

/**
 * @brief sets coulomb potential type
 *
 * @param coulombType
 */
void PotentialSetup::setupCoulomb()
{
    const auto coulombRadiusCutOff = _engine.getSimulationBox().getCoulombRadiusCutOff();
    auto       wolfParameter       = _engine.getSettings().getWolfParameter();

    if (_engine.getSettings().getCoulombLongRangeType() == "none")
        _engine.getPotential().makeCoulombPotential(CoulombShiftedPotential(coulombRadiusCutOff));
    else if (_engine.getSettings().getCoulombLongRangeType() == "wolf")
    {
        _engine.getPotential().makeCoulombPotential(CoulombWolf(coulombRadiusCutOff, wolfParameter));
    }

    if (_engine.getForceFieldPtr()->isNonCoulombicActivated())
        _engine.getPotential().getCoulombPotential().setCoulombPreFactorToForceField();
    else
        _engine.getPotential().getCoulombPotential().setCoulombPreFactorToGuff();
}

/**
 * @brief sets nonCoulomb potential type
 *
 */
void PotentialSetup::setupNonCoulomb()
{
    if (_engine.getForceFieldPtr()->isNonCoulombicActivated())
        _engine.getPotential().makeNonCoulombPotential(potential::ForceFieldNonCoulomb());
    else
        _engine.getPotential().makeNonCoulombPotential(potential::GuffNonCoulomb());
}

void PotentialSetup::setupNonCoulombicPairs()
{
    ranges::for_each(_engine.getPotential().getNonCoulombicPairsVector(),
                     [](auto &nonCoulombicPair)
                     {
                         const auto &[energy, force] =
                             nonCoulombicPair->calculateEnergyAndForce(nonCoulombicPair->getRadialCutOff());
                         nonCoulombicPair->setEnergyCutOff(energy);
                         nonCoulombicPair->setForceCutOff(force);
                     });

    _engine.getSimulationBox().setupExternalToInternalGlobalVdwTypesMap();
    _engine.getPotential().determineInternalGlobalVdwTypes(_engine.getSimulationBox().getExternalToInternalGlobalVDWTypes());

    const auto numberOfGlobalVdwTypes           = _engine.getSimulationBox().getExternalGlobalVdwTypes().size();
    auto       selfInteractionNonCoulombicPairs = _engine.getPotential().getSelfInteractionNonCoulombicPairs();

    if (selfInteractionNonCoulombicPairs.size() != numberOfGlobalVdwTypes)
        throw customException::ParameterFileException(
            "Not all self interacting non coulombics were set in the noncoulombics section of the parameter file");

    ranges::sort(selfInteractionNonCoulombicPairs,
                 [](const auto &nonCoulombicPair1, const auto &nonCoulombicPair2)
                 { return nonCoulombicPair1->getInternalType1() < nonCoulombicPair2->getInternalType1(); });

    for (size_t i = 0; i < numberOfGlobalVdwTypes; ++i)
        if (selfInteractionNonCoulombicPairs[i]->getInternalType1() != i)
            throw customException::ParameterFileException(
                "Not all self interacting non coulombics were set in the noncoulombics section of the parameter file");

    _engine.getPotential().fillDiagonalElementsOfNonCoulombicPairsMatrix(selfInteractionNonCoulombicPairs);
    _engine.getPotential().fillNonDiagonalElementsOfNonCoulombicPairsMatrix();
}