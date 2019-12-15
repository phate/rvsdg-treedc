# rvsdg-xmlparser

Parser of the RVSDG XML output from `jlm-print --j2rx`,
producing a corresponding [dotfile](https://graphviz.gitlab.io/) for each region in the graph.

Uses the pugixml library to parse the XML, which can be acquired via apt as
`libpugixml-dev`, or via other means as described on https://pugixml.org/.

Running
-------------------

Call the program with an input XML filepath as argument to produce
corresponding dotfiles and place them in the `dot_files` folder.

    cd build
    cmake .. && make
    ./main path/to/file.xml

To perform this translation, we map nodes and edges from the XML to the graph
by considering inputs to be edges into, and outputs edges out of their
corresponding nodes. Arguments and results of a region are modeled as the entry
and exit nodes of the graph respectively.
