#include "edge.h"
#include "node.h"
#include "region.h"

#include "xml_parser.h"

#include "element.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string.h>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

#define ROOT_DIR string(getenv("HOME")) + "/rvsdg-treedc/xml_parser/dot_files/"
