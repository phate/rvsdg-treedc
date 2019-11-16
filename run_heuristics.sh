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

if [[ $1 == "all" ]]; then

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
fi

PLOTS=gnuplots
PLOTGEN=plot.sh
filenr=0

if [[ $1 == "upper" ]]; then

RESULTS=$PLOTS/upper.results

for f in $SOURCE_XML/*; do
    find $DOTFILES -type f -name \*.dot -exec rm "{}" \;
    ./$XML_PARSER $f  >> $LOG
    printf "%d " $filenr >> $RESULTS
    printf "%s " $(basename -s '.xml' $f) >> $RESULTS
    ./$HEURISTICS $DOTFILES | grep "Highest" | grep -oP "(?<=max: )[^,]+" >> $RESULTS
    tail -n 1 $RESULTS
    filenr=$(($filenr+1))
done

cd $PLOTS && ./$PLOTGEN "upper"

fi

if [[ $1 == "gap" ]]; then

RESULTS=$PLOTS/gap.results

for f in $SOURCE_XML/*; do
    find $DOTFILES -type f -name \*.dot -exec rm "{}" \;
    ./$XML_PARSER $f  >> $LOG
    printf "%d " $filenr >> $RESULTS
    printf "%s " $(basename -s '.xml' $f) >> $RESULTS
    largest=$(./$HEURISTICS $DOTFILES | grep "Largest")
    max=$(echo $largest | grep -oP "(?<=max: )[^,]+")
    min=$(echo $largest | grep -oP "(?<=min: )[^,]+")
    gap=$(($max - $min))
    echo $gap $max-$min >> $RESULTS
    tail -n 1 $RESULTS
    filenr=$(($filenr+1))
done

cd $PLOTS && ./$PLOTGEN "gap"

fi
