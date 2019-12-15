# rvsdg-treedc

Tool to read a RVSDG represented as a dotfile, load it into an in-memory graph
representation and run a set of heuristic treewidth algorithms on the graph.
Implements and reports on heuristics described in [Googate & Detchter](https://arxiv.org/abs/1207.4109).

Includes a tool to generate dotfiles from XML representation created by the
`jlm` compiler. See more in `xml_parser/README.md`

Building
-------------------

Depends on the unittest-cpp framework: [Installation instructions](https://github.com/unittest-cpp/unittest-cpp/wiki/Building-Using-CMake)

    mkdir build
    cd build && cmake ..
    make

Running
-------------------
By default, parses and runs heuristics on every dotfile in `xml_parser/dotfiles`.
Takes an optional argument to run on another directory.

    ./main xml_parser/dotfiles

The `run_heuristics.sh` script generates dotfiles from the RVSDG xml files in a source folder runs the heuristics on the generated graphs.
The output from the heuristics are aggregated and logged to `run_heuristics.log`.
The script also supports aggregating the output from the heuristics and feeding it into a set of gnuplot files to generate visualisations of the output.

Testing
-------------------

A set of sanity tests in `test/tests.cpp` are setup to run on the default target `main`, and can be used in development.

Autogenerated tests for the heuristics can be created by the tools in `gen_tests`:

    cd gen_tests
    ./gen_tests.sh

this reads the `test_config.txt` file and autogenerates unittest-cpp tests for each graph with bounds found.
The graphs are encoded in the [graph6 format](http://users.cecs.anu.edu.au/~bdm/data/formats.txt) and are (manually) retrieved from the [ToTo treewidth database](https://treedecompositions.com/#) along with the graphs upper and lower bounds on treewidth (verified as exact solutions or verified as best upper/lower bounds by other users).

To get the graph6 string from a chosen graph in the database, it can be retrieved it from the URL itself - e.g., `https://treedecompositions.com/#/graph/FrSXg`, where the graph6 string is `FrSXg` - or by using the database query to return up to 15 graphs and exporting them to a csv file. This can be further converted to a `test_config` file by using the conf_from_csv.py tool:

    python3 conf_from_csv.py Overview.csv test_config.txt

The current tests in the config are mix of graphs with high bounds gap with an increasing number of vertices.

After the tests are generated, they can be run with the `heurisitc_tests` target:

    mkdir build
    cd build && cmake ..
    make
    ./heurisitc_tests
