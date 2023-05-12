#include "testRstFileReader.hpp"
#include "exceptions.hpp"
#include "moldescriptorReader.hpp"

using namespace std;
using namespace ::testing;
using namespace Setup::RstFileReader;

TEST_F(TestRstFileReader, testDetermineSection)
{
    string filename = "examples/setup/h2o_qmcfc.rst";
    RstFileReader rstFileReader(filename, _engine);

    auto lineElements = vector<string>{"sTeP", "1"};
    auto section = rstFileReader.determineSection(lineElements);
    EXPECT_EQ(section->keyword(), "step");

    lineElements = vector<string>{"cHI"};
    section = rstFileReader.determineSection(lineElements);
    EXPECT_EQ(section->keyword(), "chi");

    lineElements = vector<string>{"Box"};
    section = rstFileReader.determineSection(lineElements);
    EXPECT_EQ(section->keyword(), "box");

    lineElements = vector<string>{"NOTAHEADERSECTION"};
    section = rstFileReader.determineSection(lineElements);
    EXPECT_EQ(section->keyword(), "");
}

TEST_F(TestRstFileReader, testFileNotFound)
{
    string filename = "examples/setup/FILENOTFOUND.rst";
    RstFileReader rstFileReader(filename, _engine);

    ASSERT_THROW(rstFileReader.read(), InputFileException);
}

// TODO: build combined test classes for such cases
TEST_F(TestRstFileReader, testRstFileReading)
{
    _engine._settings.setMoldescriptorFilename("examples/setup/moldescriptor.dat");
    MoldescriptorReader moldescriptor(_engine);

    string filename = "examples/setup/h2o-qmcf.rst";
    _engine._settings.setStartFilename(filename);

    moldescriptor.read();
    ASSERT_NO_THROW(readRstFile(_engine));
}

int main(int argc, char **argv)
{
    InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}