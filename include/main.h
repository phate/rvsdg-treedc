#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <string.h>
#include <vector>

#include "Graph.h"
class Graph;

#include "DFS.h"
#include "Heuristics.h"
#include "Kleinberg.h"
#include "Operations.h"
#include "Parse.h"
#include "run_dotfiles.h"

#include "UnitTest++/UnitTest++.h"

using namespace std;
#define ROOT_DIR string(getenv("HOME")) + "/rvsdg-treedc/"
