#include <algorithm>
#include <fstream>
#include <iostream>
#include <string.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>

#include "edge.h"
#include "node.h"
#include "region.h"

#include "xml_parser.h"

#include "element.h"

using namespace std;
#define DOT_FILES filesystem::current_path().parent_path().string() + "/dot_files/"
#define JLM_DIR filesystem::current_path().parent_path().parent_path().parent_path().string() + "/jlm/"
