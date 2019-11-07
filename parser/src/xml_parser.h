#pragma once
#include "main.h"

#include "pugixml.hpp"

pugi::xml_document load_xml(const char source[]);
void traverse(pugi::xml_node& parent, Element* parent_element, unsigned depth);
