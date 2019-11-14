#include "main.h"

int main(int argc, char* argv[])
{
    UnitTest::RunAllTests();

    if (argc == 2)
        run_dotfiles(argv[1]);
    else
        run_dotfiles(ROOT_DIR + "xml_parser/dot_files");
}
