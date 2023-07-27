#include "topologyReader.hpp"

#include "exceptions.hpp"
#include "stringUtilities.hpp"

using namespace std;
using namespace setup;
using namespace StringUtilities;

/**
 * @brief constructor
 *
 * @param filename
 * @param engine
 */
TopologyReader::TopologyReader(const string &filename, engine::Engine &engine)
    : _filename(filename), _fp(filename), _engine(engine)
{
    _topologySections.push_back(new ShakeSection());
}

/**
 * @brief checks if reading topology file is needed
 *
 * @return true if shake is activated
 * @return false
 */
bool TopologyReader::isNeeded() const
{
    if (_engine.getConstraints().isActivated()) return true;

    return false;
}

/**
 * @brief reads topology file
 */
void TopologyReader::read()
{
    string         line;
    vector<string> lineElements;
    int            lineNumber = 1;

    if (!isNeeded()) return;

    if (_filename.empty()) throw customException::InputFileException("Topology file needed for requested simulation setup");

    if (!filesystem::exists(_filename))
        throw customException::InputFileException("Topolgoy file \"" + _filename + "\"" + " File not found");

    while (getline(_fp, line))
    {
        line         = removeComments(line, "#");
        lineElements = splitString(line);

        if (lineElements.empty())
        {
            lineNumber++;
            continue;
        }

        auto section = determineSection(lineElements);
        section->setLineNumber(lineNumber++);
        cout << "Hello" << endl;
        section->setFp(&_fp);
        // cout << "Processing topology section \"" << section->keyword() << "\"\n";
        section->process(lineElements, _engine);
        lineNumber = section->getLineNumber();
    }
}

/**
 * @brief determines which section of the topology file the header line belongs to
 *
 * @param lineElements
 * @return TopologySection*
 */
TopologySection *TopologyReader::determineSection(const vector<string> &lineElements)
{
    for (auto section = _topologySections.begin(); section != _topologySections.end(); section++)
        if ((*section)->keyword() == to_lower_copy(lineElements[0]))
        {
            _topologySections.erase(section);
            return *section;
        };

    throw customException::TopologyException("Unknown or already passed keyword \"" + lineElements[0] + "\" in topology file");
}

/**
 * @brief constructs a TopologyReader and reads topology file
 *
 * @param filename
 * @param engine
 */
void setup::readTopologyFile(engine::Engine &engine)
{
    TopologyReader topologyReader(engine.getSettings().getTopologyFilename(), engine);
    topologyReader.read();
}