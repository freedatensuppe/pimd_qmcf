#ifndef _SIMULATION_BOX_SETUP_HPP_

#define _SIMULATION_BOX_SETUP_HPP_

namespace engine
{
    class Engine;
}   // namespace engine

namespace setup
{
    void setupSimulationBox(engine::Engine &);

    /**
     * @class SetupSimulationBox
     *
     * @brief Setup simulation box
     *
     */
    class SimulationBoxSetup
    {
      private:
        engine::Engine &_engine;

      public:
        explicit SimulationBoxSetup(engine::Engine &engine) : _engine(engine){};

        void setup();

        void setAtomNames();
        void setAtomTypes();
        void setExternalVDWTypes();
        void setPartialCharges();

        void setAtomMasses();
        void setAtomicNumbers();

        void calculateMolMasses();
        void calculateTotalMass();
        void calculateTotalCharge();

        void checkBoxSettings();
        void checkRcCutoff();

        void initVelocities();
    };

}   // namespace setup

#endif   // _SIMULATION_BOX_SETUP_HPP_