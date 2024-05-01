/*****************************************************************************
<GPL_HEADER>

    PQ
    Copyright (C) 2023-now  Jakob Gamper

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

<GPL_HEADER>
******************************************************************************/

#include "qmmmSetup.hpp"

#include <cstddef>       // for size_t
#include <string>        // for string
#include <string_view>   // for string_view
#include <vector>        // for vector

#include "engine.hpp"         // for QMMMMDEngine
#include "exceptions.hpp"     // for InputFileException
#include "fileSettings.hpp"   // for FileSettings
#include "qmmmSettings.hpp"   // for QMMMSettings
#include "qmmmmdEngine.hpp"   // for QMMMEngine
#include "selection.hpp"      // for select

using setup::QMMMSetup;

/**
 * @brief wrapper to build QMMMSetup object and call setup
 *
 * @param engine
 */
void setup::setupQMMM(engine::QMMMMDEngine &engine)
{
    engine.getStdoutOutput().writeSetup("QMMM setup");
    engine.getLogOutput().writeSetup("QMMM setup");

    QMMMSetup qmmmSetup(engine);
    qmmmSetup.setup();
}

/**
 * @brief setup QMMM-MD
 *
 */
void QMMMSetup::setup() { setupQMCenter(); }

void QMMMSetup::setupQMCenter()
{
    std::string restartFileName       = settings::FileSettings::getStartFileName();
    std::string moldescriptorFileName = settings::FileSettings::getMolDescriptorFileName();
    std::string qmCenterString        = settings::QMMMSettings::getQMCenterString();

#ifdef WITH_PYTHON_BINDINGS
    std::vector<int> qmCenter =
        pq_python::select(qmCenterString, restartFileName, moldescriptorFileName);
#else
    throw customException::InputFileException(
        "Python is not enabled, cannot use QM center selection"
    );
#endif
}