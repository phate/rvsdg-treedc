#!/bin/bash

# Tool for auto generating heuristic tests
# reads test_config.txt which contains for each line:
# graph in graph6 format, upper bound, lower bound

i=0
while read -r test; do

    read -r -a t <<< "${test}"

    # skip empty line
	[ -z "${test}" ] && continue

    fname="graph${i}" && (( i++  ))
    graph="${t[0]}"
    ub="${t[1]}"
    lb="${t[2]}"

    # skip line starting with "#"
    [ "${graph:0:1}" == "#" ] && continue

    # create dotfile from graph6 in config line
    bash g6todot.sh "${graph}" "../test/heuristics/dot_files/${fname}.dot"

    # if successful, create test
    if [ $? -eq 0 ]; then
        python3 gen_test.py ${fname} "${graph:0:20}" $lb $ub
    fi

    echo

done < test_config.txt
