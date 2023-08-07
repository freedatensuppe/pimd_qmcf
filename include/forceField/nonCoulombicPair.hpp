#ifndef _NON_COULOMBIC_PAIR_HPP_

#define _NON_COULOMBIC_PAIR_HPP_

#include "mathUtilities.hpp"

#include <cstddef>
#include <vector>

namespace forceField
{
    class NonCoulombicPair;
    class LennardJonesPair;
    class BuckinghamPair;
    class MorsePair;
}   // namespace forceField

/**
 * @class NonCoulombicPair
 *
 * @brief base class representing a pair of non-coulombic types
 *
 */
class forceField::NonCoulombicPair
{
  protected:
    size_t _vanDerWaalsType1;
    size_t _vanDerWaalsType2;
    size_t _internalType1;
    size_t _internalType2;

    double _cutOff;

  public:
    NonCoulombicPair(const size_t vanDerWaalsType1, const size_t vanDerWaalsType2, const double cutOff)
        : _vanDerWaalsType1(vanDerWaalsType1), _vanDerWaalsType2(vanDerWaalsType2), _cutOff(cutOff){};
    virtual ~NonCoulombicPair() = default;

    [[nodiscard]] bool operator==(const NonCoulombicPair &other) const
    {
        return _vanDerWaalsType1 == other._vanDerWaalsType1 && _vanDerWaalsType2 == other._vanDerWaalsType2 &&
               _internalType1 == other._internalType1 && _internalType2 == other._internalType2 &&
               utilities::compare(_cutOff, other._cutOff);
    }

    size_t setInternalType1(const size_t internalType1) { return _internalType1 = internalType1; }
    size_t setInternalType2(const size_t internalType2) { return _internalType2 = internalType2; }

    [[nodiscard]] size_t getVanDerWaalsType1() const { return _vanDerWaalsType1; }
    [[nodiscard]] size_t getVanDerWaalsType2() const { return _vanDerWaalsType2; }
    [[nodiscard]] size_t getInternalType1() const { return _internalType1; }
    [[nodiscard]] size_t getInternalType2() const { return _internalType2; }
    [[nodiscard]] double getCutOff() const { return _cutOff; }
};

/**
 * @class LennardJonesPair
 *
 * @brief represents a pair of Lennard-Jones types
 *
 */
class forceField::LennardJonesPair : public forceField::NonCoulombicPair
{
  private:
    double _c6;
    double _c12;

  public:
    LennardJonesPair(
        const size_t vanDerWaalsType1, const size_t vanDerWaalsType2, const double cutOff, const double c6, const double c12)
        : NonCoulombicPair(vanDerWaalsType1, vanDerWaalsType2, cutOff), _c6(c6), _c12(c12){};

    [[nodiscard]] bool operator==(const LennardJonesPair &other) const
    {
        return NonCoulombicPair::operator==(other) && utilities::compare(_c6, other._c6) && utilities::compare(_c12, other._c12);
    }

    [[nodiscard]] double getC6() const { return _c6; }
    [[nodiscard]] double getC12() const { return _c12; }
};

/**
 * @class BuckinghamPair
 *
 * @brief represents a pair of Buckingham types
 *
 */
class forceField::BuckinghamPair : public forceField::NonCoulombicPair
{
  private:
    double _a;
    double _dRho;
    double _c6;

  public:
    BuckinghamPair(const size_t vanDerWaalsType1,
                   const size_t vanDerWaalsType2,
                   const double cutOff,
                   const double a,
                   const double dRho,
                   const double c6)
        : NonCoulombicPair(vanDerWaalsType1, vanDerWaalsType2, cutOff), _a(a), _dRho(dRho), _c6(c6){};

    [[nodiscard]] bool operator==(const BuckinghamPair &other) const
    {
        return NonCoulombicPair::operator==(other) && utilities::compare(_a, other._a) &&
               utilities::compare(_dRho, other._dRho) && utilities::compare(_c6, other._c6);
    }

    [[nodiscard]] double getA() const { return _a; }
    [[nodiscard]] double getDRho() const { return _dRho; }
    [[nodiscard]] double getC6() const { return _c6; }
};

/**
 * @class MorsePair
 *
 * @brief represents a pair of Morse types
 *
 */
class forceField::MorsePair : public forceField::NonCoulombicPair
{
  private:
    double _dissociationEnergy;
    double _wellWidth;
    double _equilibriumDistance;

  public:
    MorsePair(const size_t vanDerWaalsType1,
              const size_t vanDerWaalsType2,
              const double cutOff,
              const double dissociationEnergy,
              const double wellWidth,
              const double equilibriumDistance)
        : NonCoulombicPair(vanDerWaalsType1, vanDerWaalsType2, cutOff), _dissociationEnergy(dissociationEnergy),
          _wellWidth(wellWidth), _equilibriumDistance(equilibriumDistance){};

    [[nodiscard]] bool operator==(const MorsePair &other) const
    {
        return NonCoulombicPair::operator==(other) && utilities::compare(_dissociationEnergy, other._dissociationEnergy) &&
               utilities::compare(_wellWidth, other._wellWidth) &&
               utilities::compare(_equilibriumDistance, other._equilibriumDistance);
    }

    [[nodiscard]] double getDissociationEnergy() const { return _dissociationEnergy; }
    [[nodiscard]] double getWellWidth() const { return _wellWidth; }
    [[nodiscard]] double getEquilibriumDistance() const { return _equilibriumDistance; }
};

#endif   // _NON_COULOMBIC_PAIR_HPP_