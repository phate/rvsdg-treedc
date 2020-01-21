#include "main.h"
bool debug = false;

int main(int argc, char* argv[])
{
    if (argc == 2)
        parse_rvsdg_xml(argv[1]);
    else { // assume debugging the program
        debug = true;
        parse_rvsdg_xml(JLM_DIR + "rvsdg.xml");
    }
}
