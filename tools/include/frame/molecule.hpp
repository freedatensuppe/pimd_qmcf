#ifndef _MOLECULE_HPP_

#define _MOLECULE_HPP_

#include "atom.hpp"

#include <vector>

namespace frameTools
{
    class Molecule
    {
      private:
        size_t _nAtoms;

        double          _molMass      = 0.0;
        vector3d::Vec3D _centerOfMass = {0.0, 0.0, 0.0};

        std::vector<Atom *> _atoms;

      public:
        Molecule() = default;
        explicit Molecule(const size_t nAtoms) : _nAtoms(nAtoms) {}

        void calculateCenterOfMass(const vector3d::Vec3D &);

        vector3d::Vec3D getCenterOfMass() const { return _centerOfMass; }

        void addAtom(Atom *atom) { _atoms.push_back(atom); }
    };

}   // namespace frameTools

#endif   // _MOLECULE_HPP_