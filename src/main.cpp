#include "main.h"

int main(int argc, char* argv[])
{
    UnitTest::RunAllTests();

    if (argc == 2)
        run_dotfiles(argv[1]);
    else {
        string root_dir = ROOT_DIR;
        run_dotfiles(root_dir + "/xml_parser/dot_files");
    }
}
