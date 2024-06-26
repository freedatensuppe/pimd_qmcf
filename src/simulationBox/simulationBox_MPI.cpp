#ifdef WITH_MPI

#include "simulationBox.hpp"

#include <algorithm>   // for for_each

using simulationBox::SimulationBox;

/**
 * @brief flattens positions of each atom into a single vector of doubles
 *
 * @return std::vector<double>
 */
std::vector<double> SimulationBox::flattenPositions()
{
    std::vector<double> positions;

    auto addPositions = [&positions](auto &atom)
    {
        const auto position = atom->getPosition();

        positions.push_back(position[0]);
        positions.push_back(position[1]);
        positions.push_back(position[2]);
    };

    std::ranges::for_each(_atoms, addPositions);

    return positions;
}

/**
 * @brief flattens velocities of each atom into a single vector of doubles
 *
 * @return std::vector<double>
 */
std::vector<double> SimulationBox::flattenVelocities()
{
    std::vector<double> velocities;

    auto addVelocities = [&velocities](auto &atom)
    {
        const auto velocity = atom->getVelocity();

        velocities.push_back(velocity[0]);
        velocities.push_back(velocity[1]);
        velocities.push_back(velocity[2]);
    };

    std::ranges::for_each(_atoms, addVelocities);

    return velocities;
}

/**
 * @brief flattens forces of each atom into a single vector of doubles
 *
 * @return std::vector<double>
 */
std::vector<double> SimulationBox::flattenForces()
{
    std::vector<double> forces;

    auto addForces = [&forces](auto &atom)
    {
        const auto force = atom->getForce();

        forces.push_back(force[0]);
        forces.push_back(force[1]);
        forces.push_back(force[2]);
    };

    std::ranges::for_each(_atoms, addForces);

    return forces;
}

/**
 * @brief de-flattens positions of each atom from a single vector of doubles
 *
 * @param positions
 */
void SimulationBox::deFlattenPositions(const std::vector<double> &positions)
{
    size_t index = 0;

    auto setPositions = [&positions, &index](auto &atom)
    {
        linearAlgebra::Vec3D position;

        position[0] = positions[index++];
        position[1] = positions[index++];
        position[2] = positions[index++];

        atom->setPosition(position);
    };

    std::ranges::for_each(_atoms, setPositions);
}

/**
 * @brief de-flattens velocities of each atom from a single vector of doubles
 *
 * @param velocities
 */
void SimulationBox::deFlattenVelocities(const std::vector<double> &velocities)
{
    size_t index = 0;

    auto setVelocities = [&velocities, &index](auto &atom)
    {
        linearAlgebra::Vec3D velocity;

        velocity[0] = velocities[index++];
        velocity[1] = velocities[index++];
        velocity[2] = velocities[index++];

        atom->setVelocity(velocity);
    };

    std::ranges::for_each(_atoms, setVelocities);
}

/**
 * @brief de-flattens forces of each atom from a single vector of doubles
 *
 * @param forces
 */
void SimulationBox::deFlattenForces(const std::vector<double> &forces)
{
    size_t index = 0;

    auto setForces = [&forces, &index](auto &atom)
    {
        linearAlgebra::Vec3D force;

        force[0] = forces[index++];
        force[1] = forces[index++];
        force[2] = forces[index++];

        atom->setForce(force);
    };

    std::ranges::for_each(_atoms, setForces);
}

#endif   // WITH_MPI