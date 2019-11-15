#!/bin/bash
# Generate dotfiles from the RVSDG xml files in $SOURCE_XML and then run heuristics on the generated graphs
# Results are logged to $RESULTS and the output from the parser and heuristics to $LOG

SOURCE_XML=$HOME/polybench-jlm/gen_llvm/xml
DOTFILES=xml_parser/dot_files
XML_PARSER=xml_parser/build/main
HEURISTICS=build/main
LOG=run_heuristics.log
RESULTS=run_heuristics.results

echo '' > $LOG
echo '' > $RESULTS

graphs=0

for f in $SOURCE_XML/*; do
    echo Running on benchmark: $f | tee -a $RESULTS
    find $DOTFILES -type f -name \*.dot -exec rm "{}" \;
    ./$XML_PARSER $f  >> $LOG
    ./$HEURISTICS $DOTFILES | \
        awk -v results=$RESULTS "/Highest|Lowest|Largest/{print>>"results"} 1" \
        >> $LOG

    tail -n 3 $RESULTS
    echo "" | tee -a $RESULTS

    gen_graphs=$(find $DOTFILES -type f -name \*.dot | wc -l)
    graphs=$((graphs+gen_graphs))
done

echo Numbers of graphs analyzed: $graphs | tee -a $RESULTS
