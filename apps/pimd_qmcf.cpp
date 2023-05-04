#include <iostream>

#include "rstFileReader.hpp"
#include "simulationBox.hpp"
#include "commandLineArgs.hpp"
#include "inputFileReader.hpp"
#include "settings.hpp"
#include "output.hpp"

#include "initStatic.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    // FIXME: cleanup this piece of code when knowing how to do it properly
    vector<string> arguments(argv, argv + argc);
    auto commandLineArgs = CommandLineArgs(argc, arguments);
    commandLineArgs.detectFlags();

    auto settings = Settings();
    readInputFile(commandLineArgs.getInputFileName(), settings);

    auto simulationBox = read_rst(settings.getStartFilename(), settings);

    cout << "Step count: " << settings._timings.getStepCount() << endl;
    cout << "Timestep: " << settings._timings.getTimestep() << endl;
    cout << "Job type: " << settings._jobType.getJobType() << endl;
    cout << "Number of steps: " << settings._timings.getNumberOfSteps() << endl;

    cout << "Output frequency: " << Output::getOutputFreq() << endl;

    for (auto &output : settings._output)
        cout << "Output file name: " << output.getFilename() << endl;

    return 0;
}