#include <filesystem>
#include <iostream>

#ifdef WITH_MPI
#include <mpi.h>
#endif

#include "celllist.hpp"
#include "commandLineArgs.hpp"
#include "engine.hpp"
#include "setup.hpp"

using namespace std;
using namespace setup;
using namespace engine;

int pimd_qmcf(int argc, char *argv[])
{
    // TODO: cleanup this piece of code when knowing how to do it properly
    vector<string> arguments(argv, argv + argc);
    auto           commandLineArgs = CommandLineArgs(argc, arguments);
    commandLineArgs.detectFlags();

    auto engine = Engine();

    setupEngine(commandLineArgs.getInputFileName(), engine);

    /*
        HERE STARTS THE MAIN LOOP
    */

    engine.run();

    /*
        HERE ENDS THE MAIN LOOP
    */

    return EXIT_SUCCESS;
}

// main wrapper
int main(int argc, char *argv[])
{
#ifdef WITH_MPI
    MPI_Init(&argc, &argv);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
#endif
    try
    {
        pimd_qmcf(argc, argv);
    }
    catch (const exception &e)
    {
        cout << "Exception: " << e.what() << endl;
#ifdef WITH_MPI
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
#endif
    }

#ifdef WITH_MPI
    for (int i = 1; i < size; i++)
    {
        auto path = "procid_pimd-qmcf_" + to_string(i);
        filesystem::remove_all(path);
    }
    MPI_Finalize();
#endif

    return EXIT_SUCCESS;
}