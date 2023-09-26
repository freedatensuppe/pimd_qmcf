/*****************************************************************************
<GPL_HEADER>

    PIMD-QMCF
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

#ifndef _INPUT_FILE_PARSER_FILES_HPP_

#define _INPUT_FILE_PARSER_FILES_HPP_

#include "inputFileParser.hpp"   // for InputFileParser

#include <cstddef>   // for size_t
#include <string>    // for string
#include <vector>    // for vector

namespace engine
{
    class Engine;   // Forward declaration
}

namespace readInput
{
    /**
     * @class InputFileParserNonCoulomb inherits from InputFileParser
     *
     * @brief Parses the non-Coulomb type commands in the input file
     *
     */
    class InputFileParserFiles : public InputFileParser
    {
      public:
        explicit InputFileParserFiles(engine::Engine &);

        void parseIntraNonBondedFile(const std::vector<std::string> &lineElements, const size_t lineNumber);
        void parseTopologyFilename(const std::vector<std::string> &lineElements, const size_t lineNumber);
        void parseParameterFilename(const std::vector<std::string> &lineElements, const size_t lineNumber);
        void parseStartFilename(const std::vector<std::string> &lineElements, const size_t lineNumber);
        void parseRingPolymerStartFilename(const std::vector<std::string> &lineElements, const size_t lineNumber);
        void parseMoldescriptorFilename(const std::vector<std::string> &lineElements, const size_t lineNumber);
        void parseGuffDatFilename(const std::vector<std::string> &lineElements, const size_t lineNumber);

        [[noreturn]] void parseGuffPath(const std::vector<std::string> &, const size_t);
    };

}   // namespace readInput

#endif   // _INPUT_FILE_PARSER_FILES_HPP_