#include "main.h"

int main()
{
    pugi::xml_document doc = load_xml("/home/sigvesn/jlm/rvsdg-real.xml");
    pugi::xml_node rvsdg = doc.child("rvsdg");
    Element root("rvsdg");
    traverse(rvsdg, &root, 0);

    cout << "\n\n"
         << root;

    cout << "\n\n";
    root.dot_print();
}
