#include "element.h"

Element::~Element()
{
    for (auto it : children) {
        delete it;
    }

    for (auto it : edges) {
        delete it;
    }
}

void print_depth(unsigned depth)
{
    for (unsigned i = 0; i < depth; ++i)
        cout << "  ";
}
