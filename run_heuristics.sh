#!/bin/bash
# Generate dotfiles from the RVSDG xml files in $SOURCE_XML and then run heuristics on the generated graphs
# Results are logged to $RESULTS and the output from the parser and heuristics to $LOG

SOURCE_XML=$HOME/polybench-jlm/gen_llvm/xml
DOTFILES=xml_parser/dot_files
XML_PARSER=xml_parser/build/main
HEURISTICS=build/main
LOG=run_heuristics.log
RESULTS=run_heuristics.results

:> $LOG
:> $RESULTS

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

# Functions for generating gnuplots of results

PLOTS=gnuplots
PLOTGEN=plot.sh
filenr=0

PLOT=upper
if [[ $1 == $PLOT ]]; then

echo Generating upper treewidth bound per benchmark plot ...
RESULTS=$PLOTS/$PLOT.results
:> $RESULTS

for f in $SOURCE_XML/*; do
    find $DOTFILES -type f -name \*.dot -exec rm "{}" \;
    ./$XML_PARSER $f  >> $LOG

    printf "%d " $filenr >> $RESULTS
    printf "%s " $(basename -s '.xml' $f) >> $RESULTS
    ./$HEURISTICS $DOTFILES |\
        grep "Highest" |\
        grep -oP "(?<=max: )[^,]+" \
        >> $RESULTS

    tail -n 1 $RESULTS
    filenr=$(($filenr+1))
done

$(cd $PLOTS && ./$PLOTGEN $PLOT)

fi

PLOT=gap
if [[ $1 == $PLOT ]]; then

echo Generating largest treewidth gap per benchmark plot ...
RESULTS=$PLOTS/$PLOT.results
:> $RESULTS

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

$(cd $PLOTS && ./$PLOTGEN $PLOT)

fi

PLOT=nodes-upper
if [[ $1 == $PLOT ]]; then

echo Generating upper bound treewidth per number of nodes plot ...
RESULTS=$PLOTS/$PLOT.results
:> $RESULTS

for f in $SOURCE_XML/*; do
    find $DOTFILES -type f -name \*.dot -exec rm "{}" \;
    ./$XML_PARSER $f  >> $LOG

    stats=$(./$HEURISTICS $DOTFILES | grep "Number of nodes\|minor_min_width_heuristic")
    echo $stats | grep -oE '[0-9]+' | paste - - -d" " >> $RESULTS
done

$(cd $PLOTS && ./$PLOTGEN $PLOT)

echo Generating average number of nodes per upper bound plot ...
AVG_RESULTS=$PLOTS/$PLOT-avg.results
:> $AVG_RESULTS

max=$(awk '{if (max < $2) max = $2} END {print max}' $RESULTS)
for i in $(seq $max); do
    avg=$(awk -v i="$i" '$2 ~ i {total+=$1;lines+=1} END {print total/lines}' $RESULTS)
    echo $i $avg >> $AVG_RESULTS
done

$(cd $PLOTS && ./$PLOTGEN $PLOT-avg)

fi

PLOT=nodes-gap
if [[ $1 == $PLOT ]]; then

echo Generating treewidth gap per number of nodes plot ...
RESULTS=$PLOTS/$PLOT.results
:> $RESULTS

for f in $SOURCE_XML/*; do
    find $DOTFILES -type f -name \*.dot -exec rm "{}" \;
    ./$XML_PARSER $f  >> $LOG

    while read -r line; do
        read -ra line<<<"$line"
        gap=$((${line[2]} - ${line[1]}))
        echo ${line[0]} $gap >> $RESULTS
    done <<< \
        $(./$HEURISTICS $DOTFILES | \
        grep "Number of nodes\|minor_min_width_heuristic\|min_fill_heuristic" | \
        grep -oE '[0-9]+' | paste - - - -d" ")
done

$(cd $PLOTS && ./$PLOTGEN $PLOT)

echo Generating average number of nodes per treewidth gap plot ...
AVG_RESULTS=$PLOTS/$PLOT-avg.results
:> $AVG_RESULTS

max=$(awk '{if (max < $2) max = $2} END {print max}' $RESULTS)
for i in $(seq $max); do
    avg=$(awk -v i="$i" '$2 ~ i {total+=$1;lines+=1} END {print total/lines}' $RESULTS)
    echo $i $avg >> $AVG_RESULTS
done

$(cd $PLOTS && ./$PLOTGEN $PLOT-avg)

fi
