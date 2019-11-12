#include "main.h"

int main(int argc, char* argv[])
{
    if (argc == 2)
        parse_rvsdg_xml(argv[1]);
    else
        parse_rvsdg_xml(string(getenv("HOME")) + "/jlm/rvsdg.xml");
}
