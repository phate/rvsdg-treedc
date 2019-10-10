#!/bin/bash

# Converts a graph in the graph6 format to a corresponding graph in the dotfiles format

# showg is a tool to manipulate graphs in the graph6 format, used to extract a list of edges from the graph
# source: http://users.cecs.anu.edu.au/~bdm/data/formats.html
if ! [ -x "$(command -v ./showg)"  ]; then
    echo "showg missing, installing ..."
    wget http://users.cecs.anu.edu.au/~bdm/data/showg_versions/showg_linux64 -O showg
    chmod +x ./showg
fi

if ! [ -n "${1}" ]; then
    echo "Missing arg: graph6"
    exit 1
fi

# If the graph6 string is retrieved directly from the URL, the string is percent encoded, but only using the %3F = ? character.
# Doing a full percent decoding will clash with the graph6 format and may destroy the string it self.
graph=$(echo "${1}" | sed "s/%3F/?/g")

if [ -n "${2}" ]; then
    outfile="${2}"
else
    outfile="graph.dot"
fi

printf "Converting graph6: %s to dotfile in %s\n" "${graph:0:20}" "${outfile}"
edges=$(echo "${graph}" | ./showg -e)
if [ $? -ne 0 ]; then
    echo "showg parsing of graph failed"
    exit 1
else
    echo "${edges}" | python3 g6todot.py > "${outfile}"
fi
